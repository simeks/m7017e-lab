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
	/// Callback for when the slider is moved. Changes the position in the media file to the new position given by where the slider was moved.
	/// @param position The position on the slider given in seconds.
    void on_sliderMoved(int position);

protected:
	/// Called when the user has clicked somewhere on the slider with the mouse. Changes the position in the media file to the new position given by where user clicked on the slider.
	void mousePressEvent(QMouseEvent* event);

	Player* _player;
};


#endif // __PLAYBACKSLIDER_H__

