#include "shared/common.h"
#include "../player.h"

#include "videowidget.h"
#include "playerwindow.h"

#include <QKeyEvent>

VideoWidget::VideoWidget(Player* player, PlayerWindow* parent)
	: _player(player),
	_player_window(parent)
{
	// Set properties
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	setLayoutDirection(Qt::LeftToRight);
	setAutoFillBackground(true);

	QPalette palette;
	palette.setBrush(QPalette::All, QPalette::Base, QBrush(QColor(0,0,0,255)));
	palette.setBrush(QPalette::All, QPalette::Window, QBrush(QColor(0,0,0,255)));
	setPalette(palette);
	
}
VideoWidget::~VideoWidget()
{
}

void VideoWidget::keyPressEvent(QKeyEvent* key_event)
{
	if(key_event->key() == Qt::Key_F5) // F5 toggles fullscreen
	{
		_player_window->ToggleFullscreen();
	}
	else if(key_event->key() == Qt::Key_Escape && _player_window->IsFullscreen()) // Escapes exits fullscreen mode if active
	{
		_player_window->ToggleFullscreen();
	}
	else
	{
		QWidget::keyPressEvent(key_event);
	}
}

void VideoWidget::mouseDoubleClickEvent(QMouseEvent * event)
{
	// Toggle fullscreen if a user double-clicks the video widget.
	_player_window->ToggleFullscreen();

	QWidget::mouseDoubleClickEvent(event);
}
