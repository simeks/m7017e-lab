#include "common.h"

#include "player.h"
#include "pipeline.h"
#include "bus.h"
#include "qt/playerwindow.h"

Player::Player() : _pipeline(NULL)
{
	_pipeline = new Pipeline();

	// Register this as a listener so that we get callbacks and call handle things like errors and EOS.
	_pipeline->SetListener(this);
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
	_pipeline->Tick();
}


bool Player::QueryDuration()
{
	return _pipeline->QueryDuration(&duration);
}

bool Player::QueryPosition()
{
	return _pipeline->QueryPosition(&timeElapsed);
}

int64_t Player::GetDuration()
{
	return duration;
}
int64_t Player::GetTimeElapsed()
{
	return  timeElapsed;
}

void Player::EndOfStream()
{
	// Handle end of stream here.


	debug::Printf("[EOS]");
}
void Player::Error(const std::string& msg)
{
	// Handle errors here.


	debug::Printf("[Error] %s", msg.c_str());
}