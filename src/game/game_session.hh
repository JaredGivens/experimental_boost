#ifndef EXPERIMENTAL_BOOST_SRC_GAME_GAME_SESSION
#define EXPERIMENTAL_BOOST_SRC_GAME_GAME_SESSION

#include <memory>

class game_session
{
public:
	game_session();
	void update();
	std::unique_ptr<float> data_;
};

#endif