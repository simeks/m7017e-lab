#ifndef __VIDEOWIDGET_H__
#define __VIDEOWIDGET_H__

#include <QWidget>

class Player;
class PlayerWindow;
class VideoWidget : public QWidget
{
	Q_OBJECT;
public:
	VideoWidget(Player* player, PlayerWindow* parent);
	~VideoWidget();
	
protected:
	void keyPressEvent(QKeyEvent* key_event);

protected:
	Player* _player;
	PlayerWindow* _player_window;

};


#endif // __VIDEOWIDGET_H__

