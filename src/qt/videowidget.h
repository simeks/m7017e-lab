#ifndef __VIDEOWIDGET_H__
#define __VIDEOWIDGET_H__

#include <QWidget>

class Player;
class PlayerWindow;

/// This is the widget that will be used for outputting video.
class VideoWidget : public QWidget
{
	Q_OBJECT;
public:
	VideoWidget(Player* player, PlayerWindow* parent);
	~VideoWidget();
	
protected:

	void keyPressEvent(QKeyEvent* key_event);
	void mouseDoubleClickEvent(QMouseEvent * event);

protected:
	Player* _player;
	PlayerWindow* _player_window;

};


#endif // __VIDEOWIDGET_H__

