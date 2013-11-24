#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <Qstring>
#include <QTimer>
#include "stdint.h"

#include "pipeline.h"

class Pipeline;
class Bus;
class PlayerWindow;

class Player : public PipelineListener
{
public:
	Player(PlayerWindow* window);
	~Player();

	void SetVideoOutput(uintptr_t window_handle);

	void Play();
	void Pause();
	void Stop();

	void ToggleRewind();
	void ToggleFastForward();

	void FullScreen();

	void PlayMedia(const std::string& file_path);
	
	/// Seeks the the specified position
	/// @param position in milliseconds.
	void Seek(int position);

	/// Returns the duration in milliseconds.
	int GetDuration();
	/// Returns the time elapsed in milliseconds.
	int GetTimeElapsed();

	/// Returns true if the player is currently playing, false if not.
	bool IsPlaying() const;
	
	/// Updates any sub-systems the player may have, should be called within a regular interval.
	void Tick();

	// PipelineListener

	void EndOfStream();
	void Error(const std::string& msg);

private:
	enum PlaybackRate
	{
		PLAYBACK_NORMAL,
		PLAYBACK_FASTFORWARD,
		PLAYBACK_REWIND
	};

	Pipeline* _pipeline;
	
	int64_t duration;
	int64_t timeElapsed;

	bool _playing;
	
	PlaybackRate _rate;


	PlayerWindow* _window;
};

#endif // __PLAYER_H__
