#include "../common.h"
#include "../player.h"

#include "playbackslider.h"


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
	QSlider::mousePressEvent(event);
}

void PlaybackSlider::on_sliderMoved(int position)
{
	_player->Seek(position);
}
