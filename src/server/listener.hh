#ifndef EXPERIMENTAL_BOOST_SRC_SERVER_LISTENER_HH
#define EXPERIMENTAL_BOOST_SRC_SERVER_LISTENER_HH

#include "http_session.hh"

// Accepts incoming connections and launches the sessions
class listener : public boost::enable_shared_from_this<listener>
{
	net::io_context &ioc_;
	tcp::acceptor acceptor_;
	boost::shared_ptr<shared_state> state_;

	void fail(beast::error_code ec, char const *what);
	void on_accept(beast::error_code ec, tcp::socket socket);

public:
	listener(
		net::io_context &ioc,
		tcp::endpoint endpoint,
		boost::shared_ptr<shared_state> const &state);

	// Start accepting incoming connections
	void run();
};

#endif