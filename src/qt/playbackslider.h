#ifndef __PLAYBACKSLIDER_H__
#define __PLAYBACKSLIDER_H__

#include <QSlider>

class Player;

/// A custom slider implementation that handles seeking when clicked.
class PlaybackSlider : public QSlider
{
	Q_OBJECT;
public:
	PlaybackSlider(Player* player, QWidget* parent);
	~PlaybackSlider();
	
protected slots:
    void on_sliderMoved(int position);

protected:
	void mousePressEvent(QMouseEvent* event);

	Player* _player;
};


#endif // __PLAYBACKSLIDER_H__

