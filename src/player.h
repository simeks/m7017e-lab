#ifndef __PLAYER_H__
#define __PLAYER_H__

class Pipeline;
class Bus;
class Player
{
public:
	Player();
	~Player();



private:
	Pipeline* _pipeline;
};

#endif // __PLAYER_H__
