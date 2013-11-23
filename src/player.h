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
	void ReWind();
	void FastForward();
	void FullScreen();
	void PlayMedia(const std::string& file_path);

	int64_t GetDuration();
	int64_t GetTimeElapsed();

	/// Returns true if the player is currently playing, false if not.
	bool IsPlaying() const;
	
	/// Updates any sub-systems the player may have, should be called within a regular interval.
	void Tick();

	// PipelineListener

	void EndOfStream();
	void Error(const std::string& msg);

private:
	Pipeline* _pipeline;
	int64_t duration;
	int64_t timeElapsed;
    bool _playing;

	PlayerWindow* _window;
};

#endif // __PLAYER_H__
