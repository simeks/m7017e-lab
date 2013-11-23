#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <Qstring>
#include <QTimer>
#include "stdint.h"

class Pipeline;
class Bus;
class PlayerWindow;

class Player
{
public:
	Player();
	~Player();

	void SetVideoOutput(uintptr_t window_handle);

	void Play();
	void Pause();
	void Stop();
	void ReWind();
	void FastForward();
	void FullScreen();
	void PlayMedia(QString fileName);

	/// Updates any sub-systems the player may have, should be called within a regular interval.
	void Tick();

	bool QueryDuration();
	bool QueryPosition();
	int64_t GetDuration();
	int64_t GetTimeElapsed();

private:
	Pipeline* _pipeline;
	int64_t duration;
	int64_t timeElapsed;
};

#endif // __PLAYER_H__
