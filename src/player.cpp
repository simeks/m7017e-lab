#include "common.h"

#include "player.h"
#include "pipeline.h"
#include "bus.h"
#include "qt/playerwindow.h"

Player::Player(PlayerWindow* window) :
	_pipeline(NULL), 
	_playing(false),
	_rate(PLAYBACK_NORMAL),
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

void Player::ToggleRewind()
{
	if(_rate == PLAYBACK_REWIND)
	{
		_pipeline->SetRate(1.0);
		_rate = PLAYBACK_NORMAL;
	}
	else
	{
		_pipeline->SetRate(-1.0);
		_rate = PLAYBACK_REWIND;
	}
}
void Player::ToggleFastForward()
{
	if(_rate == PLAYBACK_FASTFORWARD)
	{
		_pipeline->SetRate(1.0);
		_rate = PLAYBACK_NORMAL;
	}
	else
	{
		_pipeline->SetRate(2.0);
		_rate = PLAYBACK_FASTFORWARD;
	}
}

void Player::PlayMedia(const std::string& file_path)
{
	// An URI should be in the format "file:///<path to file>"
	//	therefore we need to append "file:///" here as file_name is only the actual file path.

	std::string file_uri = "file:///";
	file_uri += file_path;
	
	_pipeline->SetUri(file_uri.c_str());
	Play();

	// Retrieve the file name from the full file path
	size_t pos = file_path.rfind("/") + 1;
	std::string file_name = file_path.substr(pos);

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

void Player::Seek(int position)
{
	int64_t pos = int64_t(position);
	pos *= 1000000000;
	if(!_pipeline->Seek(pos))
	{
		debug::Printf("Failed to retrieve new position.");
	}
}