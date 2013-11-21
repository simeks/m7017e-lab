#ifndef __PLAYER_H__
#define __PLAYER_H__

class Pipeline;
class Bus;
class PlayerWindow;

class Player
{
public:
	Player();
	~Player();

	int Run(int argc, char *argv[]);

private:
	Pipeline* _pipeline;
	PlayerWindow* _window;

};

#endif // __PLAYER_H__
