#include "common.h"

#include "player.h"
#include "pipeline.h"
#include "bus.h"
#include "qt/playerwindow.h"

#include <QApplication>

Player::Player() : _pipeline(NULL), _window(NULL)
{
	_pipeline = new Pipeline("playbin2");
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
	_window = new PlayerWindow();
	_window->show();

	_pipeline->SetOutput(_window->GetOutputHandle());


	return a.exec();
}
