#ifndef __VIDEOWIDGET_H__
#define __VIDEOWIDGET_H__

#include <QWidget>

class Player;
class VideoWidget : public QWidget
{
	Q_OBJECT;
public:
	VideoWidget(Player* player, QWidget* parent);
	~VideoWidget();
	
protected:
	Player* _player;
};


#endif // __VIDEOWIDGET_H__

