#include "shared/common.h"
#include "../player.h"

#include "playbackslider.h"

#include <QMouseEvent>

PlaybackSlider::PlaybackSlider(Player* player, QWidget* parent) 
	: QSlider(parent),
	_player(player)
{
	// Set properties
	setObjectName(QStringLiteral("playbackSlider"));
	setMouseTracking(false);
	setOrientation(Qt::Horizontal);
	setPageStep(1);
	setTracking(false);
	setTickPosition(QSlider::NoTicks);

    connect((QSlider*)this, SIGNAL(sliderMoved(int)), this, SLOT(on_sliderMoved(int)));
}
PlaybackSlider::~PlaybackSlider()
{
}

void PlaybackSlider::mousePressEvent(QMouseEvent* event)
{
	// Check if the user clicked somewhere on the slider with the left button
	if(event->button() == Qt::LeftButton)
	{
		// In case they did we jump to that position.

		int new_value = minimum() + ((maximum() - minimum()) * event->x()) / width();
		setValue(new_value);

		// Let the player seek to the new position
		_player->Seek(new_value);

		event->accept();
	}
	QSlider::mousePressEvent(event);
}

void PlaybackSlider::on_sliderMoved(int position)
{
	_player->Seek(position);
}
