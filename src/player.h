#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <Qstring>
#include <QTimer>

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

private:
	Pipeline* _pipeline;

};

#endif // __PLAYER_H__
