#ifndef __PLAYER_H__
#define __PLAYER_H__
#include <stdint.h>

class Pipeline;
class Bus;
class PlayerWindow;

class Player
{
public:
	Player();
	~Player();

	int Run(int argc, char *argv[]);
	void Play();
	void Pause();
	void Stop();
	void ReWind();
	void FastForward();
	void FullScreen();

private:
	Pipeline* _pipeline;
	PlayerWindow* _window;
	int64_t duration;
};

#endif // __PLAYER_H__
