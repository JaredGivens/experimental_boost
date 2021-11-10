
#ifndef EXPERIMENTAL_BOOST_SRC_SERVER_WS_SESSION_HH
#define EXPERIMENTAL_BOOST_SRC_SERVER_WS_SESSION_HH

#include <unordered_set>
#include <boost/asio.hpp>
#include <boost/beast.hpp>

namespace beast = boost::beast;
namespace http = beast::http;
namespace ws = beast::websocket;
namespace net = boost::asio;
using tcp = net::ip::tcp;

class ws_session;

class shared_state
{
	std::string const doc_root_;

	// This mutex synchronizes all access to sessions_
	std::mutex mutex_;

	// Keep a list of all the connected clients
	std::unordered_set<ws_session *> sessions_;

public:
	explicit shared_state(std::string doc_root);

	std::string const &doc_root() const noexcept { return doc_root_; }

	void join(ws_session *session);
	void leave(ws_session *session);
	void send(std::string message);
};

class ws_session : public boost::enable_shared_from_this<ws_session>
{
	beast::flat_buffer buffer_;
	ws::stream<beast::tcp_stream> ws_;
	boost::shared_ptr<shared_state> state_;
	std::vector<boost::shared_ptr<std::string const>> queue_;

	void fail(beast::error_code ec, char const *what);
	void on_accept(beast::error_code ec);
	void on_read(beast::error_code ec, std::size_t bytes_transferred);
	void on_write(beast::error_code ec, std::size_t bytes_transferred);

public:
	ws_session(
		tcp::socket &&socket,
		boost::shared_ptr<shared_state> const &state);

	~ws_session();

	template <class Body, class Allocator>
	void
	run(http::request<Body, http::basic_fields<Allocator>> req);

	// Send a message
	void
	send(boost::shared_ptr<std::string const> const &ss);

private:
	void
	on_send(boost::shared_ptr<std::string const> const &ss);
};

template <class Body, class Allocator>
void ws_session::
	run(http::request<Body, http::basic_fields<Allocator>> req)
{
	// Set suggested timeout settings for the websocket
	ws_.set_option(
		ws::stream_base::timeout::suggested(
			beast::role_type::server));

	// Set a decorator to change the Server of the handshake
	ws_.set_option(ws::stream_base::decorator(
		[](ws::response_type &res)
		{
			res.set(http::field::server,
					std::string(BOOST_BEAST_VERSION_STRING) +
						" websocket-chat-multi");
		}));

	// Accept the websocket handshake
	ws_.async_accept(
		req,
		beast::bind_front_handler(
			&ws_session::on_accept,
			shared_from_this()));
}

#endif