#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <Qstring>
#include <QTimer>

#include "pipeline.h"

class Pipeline;
class Bus;
class PlayerWindow;

class Player : public PipelineListener
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

	
	// PipelineListener

	void EndOfStream();
	void Error(const std::string& msg);


private:
	Pipeline* _pipeline;

};

#endif // __PLAYER_H__
