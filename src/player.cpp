#include "common.h"

#include "player.h"
#include "pipeline.h"
#include "bus.h"
#include "qt/playerwindow.h"

using namespace std;

#include <QApplication>

Player::Player() : _pipeline(NULL), _window(NULL)
{
	_pipeline = new Pipeline();
}
Player::~Player()
{
	delete _window;
	_window = NULL;

	delete _pipeline;
	_pipeline = NULL;
}

int Player::Run(int argc, char *argv[])
{
    QApplication a(argc, argv);
	_window = new PlayerWindow(this);
	_window->show();

	//_pipeline->SetOutput(_window->GetOutputHandle());

	return a.exec();
}

void Player::Play()
{
	_pipeline->SetState(GST_STATE_PLAYING);
}

void Player::Pause()
{
	_pipeline->SetState(GST_STATE_PAUSED);
}

void Player::Stop()
{
	_pipeline->SetState(GST_STATE_NULL);
}

void Player::ReWind()
{

}

void Player::FastForward()
{

}

void Player::FullScreen()
{

}

void Player::PlayMedia(QString fileName)
{
	std::string file = fileName.toLocal8Bit().constData();
	_pipeline->SetUri(file);
	_pipeline->SetOutput(_window->GetOutputHandle());
	_pipeline->SetState(GST_STATE_PLAYING);
}