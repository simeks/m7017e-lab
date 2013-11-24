#include "common.h"

#include "player.h"
#include "pipeline.h"
#include "bus.h"
#include "qt/playerwindow.h"

Player::Player(PlayerWindow* window) :
	_pipeline(NULL), 
	_playing(false),
	_rate(PLAYBACK_NORMAL),
	_window(window),
	_playlist_iterator(_playlist.CreateIterator())
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
	_pipeline->SetState(GST_STATE_READY);
	_window->StreamEnded();

	// Restart our playlist
	_playlist_iterator = _playlist.CreateIterator();
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

void Player::PlayNext()
{
	if(!_playlist_iterator.End())
		PlayMedia(_playlist_iterator.Next());
}

int Player::GetDuration()
{
	int64_t duration = 0;
	if(!_pipeline->QueryDuration(&duration))
	{
		debug::Printf("[Warning] Failed to retrieve duration.\n");
	}

	// Convert time into milliseconds before returning
	int time_sec = int(duration / 1000000);

	return time_sec;
}
int Player::GetTimeElapsed()
{
	int64_t position = 0;
	if(!_pipeline->QueryPosition(&position))
	{
		debug::Printf("[Warning] Failed to retrieve position.\n");
	}

	// Convert time into seconds before returning
	int time_sec = int(position / 1000000);

	return time_sec;
}
bool Player::IsPlaying() const
{
	return _playing;
}

void Player::Tick()
{
	_pipeline->Tick();
}

Playlist& Player::GetPlaylist()
{
	return _playlist;
}

void Player::EndOfStream()
{
	if(_playlist_iterator.End())
	{
		_playing = false;
		_window->StreamEnded();
	}
	else
	{
		_pipeline->SetState(GST_STATE_READY);
		PlayMedia(_playlist_iterator.Next());
	}
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
	pos *= 1000000; // Convert into nanoseconds (from milliseconds)
	if(!_pipeline->Seek(pos))
	{
		debug::Printf("[Warning] Failed to set new position.\n");
	}
}