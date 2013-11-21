#include "common.h"

#include "player.h"
#include "pipeline.h"
#include "bus.h"

Player::Player() : _pipeline(NULL)
{
	_pipeline = new Pipeline("playbin2");

}
Player::~Player()
{
	delete _pipeline;
	_pipeline = NULL;
}


