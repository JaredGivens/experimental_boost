#include "ws_session.hh"
#include <iostream>

shared_state::shared_state(std::string doc_root) : doc_root_(std::move(doc_root)) {}

void shared_state::join(ws_session *session)
{
	std::lock_guard<std::mutex> lock(mutex_);
	sessions_.insert(session);
}

void shared_state::leave(ws_session *session)
{
	std::lock_guard<std::mutex> lock(mutex_);
	sessions_.erase(session);
}

// Broadcast a message to all websocket client sessions
void shared_state::send(std::string message)
{
	// Put the message in a shared pointer so we can re-use it for each client
	auto const ss = boost::make_shared<std::string const>(std::move(message));

	// Make a local list of all the weak pointers representing
	// the sessions, so we can do the actual sending without
	// holding the mutex:
	std::vector<boost::weak_ptr<ws_session>> v;
	{
		std::lock_guard<std::mutex> lock(mutex_);
		v.reserve(sessions_.size());
		for (auto p : sessions_)
		{
			v.emplace_back(p->weak_from_this());
		}
	}

	// For each session in our local list, try to acquire a strong
	// pointer. If successful, then send the message on that session.
	for (auto const &wp : v)
	{
		if (auto sp = wp.lock())
		{
			sp->send(ss);
		}
	}
}

ws_session::ws_session(
	tcp::socket &&socket,
	boost::shared_ptr<shared_state> const &state)
	: ws_(std::move(socket)), state_(state)
{
}

ws_session::
	~ws_session()
{
	// Remove this session from the list of active sessions
	state_->leave(this);
}

void ws_session::
	fail(beast::error_code ec, char const *what)
{
	// Don't report these
	if (ec == net::error::operation_aborted || ec == ws::error::closed)
	{
		return;
	}

	std::cerr << what << ": " << ec.message() << "\n";
}

void ws_session::on_accept(beast::error_code ec)
{
	// Handle the error, if any
	if (ec)
	{
		return fail(ec, "accept");
	}

	// Add this session to the list of active sessions
	state_->join(this);

	// Read a message
	ws_.async_read(buffer_, beast::bind_front_handler(
								&ws_session::on_read,
								shared_from_this()));
}

void ws_session::on_read(beast::error_code ec, std::size_t)
{
	// Handle the error, if any
	if (ec)
	{
		return fail(ec, "read");
	}

	// Send to all connections
	state_->send(beast::buffers_to_string(buffer_.data()));

	// Clear the buffer
	buffer_.consume(buffer_.size());

	// Read another message
	ws_.async_read(
		buffer_,
		beast::bind_front_handler(
			&ws_session::on_read,
			shared_from_this()));
}

void ws_session::
	send(boost::shared_ptr<std::string const> const &ss)
{
	// Post our work to the strand, this ensures
	// that the members of `this` will not be
	// accessed concurrently.

	net::post(
		ws_.get_executor(),
		beast::bind_front_handler(
			&ws_session::on_send,
			shared_from_this(),
			ss));
}

void ws_session::
	on_send(boost::shared_ptr<std::string const> const &ss)
{
	// Always add to queue
	queue_.push_back(ss);

	// Are we already writing?
	if (queue_.size() > 1)
		return;

	// We are not currently writing, so send this immediately
	ws_.async_write(
		net::buffer(*queue_.front()),
		beast::bind_front_handler(
			&ws_session::on_write,
			shared_from_this()));
}

void ws_session::
	on_write(beast::error_code ec, std::size_t)
{
	// Handle the error, if any
	if (ec)
		return fail(ec, "write");

	// Remove the string from the queue
	queue_.erase(queue_.begin());

	// Send the next message if any
	if (!queue_.empty())
		ws_.async_write(
			net::buffer(*queue_.front()),
			beast::bind_front_handler(
				&ws_session::on_write,
				shared_from_this()));
}
