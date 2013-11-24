#include "../common.h"
#include "../player.h"

#include "videowidget.h"

VideoWidget::VideoWidget(Player* player, QWidget* parent)
	: _player(player)
{
	// Set properties
	setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
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

