#include "game_session.hh"
#include <boost/make_unique.hpp>

game_session::game_session() : data_(boost::make_unique<float>())
{
	auto raw_data = data_.get();
	for (uint8_t i = 0; i < 4; ++i)
	{
		raw_data[i] = i;
	}
}

void game_session::update()
{
}
