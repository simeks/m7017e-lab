#include "common.h"

#include "player.h"
#include "pipeline.h"
#include "bus.h"
#include "qt/playerwindow.h"

Player::Player() : _pipeline(NULL)
{
	_pipeline = new Pipeline();
}
Player::~Player()
{
	delete _pipeline;
	_pipeline = NULL;
}

void Player::SetVideoOutput(uintptr_t window_handle)
{
	_pipeline->SetOutput(window_handle);
}

void Player::Play()
{
	_pipeline->SetState(GST_STATE_PLAYING);
}

void Player::Pause()
{
	_pipeline->SetState(GST_STATE_PAUSED);
}

void Player::Stop()
{
	_pipeline->SetState(GST_STATE_NULL);
}

void Player::ReWind()
{

}

void Player::FastForward()
{

}

void Player::FullScreen()
{

}

void Player::PlayMedia(QString file_name)
{
	// An URI should be in the format "file:///<path to file>"
	//	therefore we need to append "file:///" here as file_name is only the actual file path.

	std::string file_uri = "file:///";
	file_uri += file_name.toLocal8Bit().constData();
	
	_pipeline->SetUri(file_uri.c_str());
	_pipeline->SetState(GST_STATE_PLAYING);
}

void Player::Tick()
{
}

