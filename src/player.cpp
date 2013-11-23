#include "common.h"

#include "player.h"
#include "pipeline.h"
#include "bus.h"
#include "qt/playerwindow.h"

Player::Player(PlayerWindow* window) :
	_pipeline(NULL), 
	_playing(false),
	_window(window)
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
	_playing = true;
	_pipeline->SetState(GST_STATE_PLAYING);
}

void Player::Pause()
{
	_playing = false;
	_pipeline->SetState(GST_STATE_PAUSED);
}

void Player::Stop()
{
	_playing = false;
	_pipeline->SetState(GST_STATE_NULL);
}

void Player::ReWind()
{

}

void Player::FastForward()
{

}

void Player::PlayMedia(const std::string& file_name)
{
	// An URI should be in the format "file:///<path to file>"
	//	therefore we need to append "file:///" here as file_name is only the actual file path.

	std::string file_uri = "file:///";
	file_uri += file_name;
	
	_pipeline->SetUri(file_uri.c_str());
	Play();

	_window->SetTrackName(file_name);
}

int64_t Player::GetDuration()
{
	int64_t duration = 0;
	if(!_pipeline->QueryDuration(&duration))
	{
		debug::Printf("Failed to retrieve duration.");
	}
	return duration;
}
int64_t Player::GetTimeElapsed()
{
	int64_t position = 0;
	if(!_pipeline->QueryPosition(&position))
	{
		debug::Printf("Failed to retrieve position.");
	}
	return position;
}
bool Player::IsPlaying() const
{
	return _playing;
}

void Player::Tick()
{
	_pipeline->Tick();
}

void Player::EndOfStream()
{
	_playing = false;

	_window->StreamEnded();
}
void Player::Error(const std::string& msg)
{
	_playing = false;

	_window->PrintError(msg);
	_window->StreamEnded();
}