#ifndef PLAYERWINDOW_H
#define PLAYERWINDOW_H

#include "../common.h"
#include <QMainWindow>
#include <QTimer>
#include "stdint.h"

namespace Ui {
class PlayerWindow;
}

class Player;
class PlayerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PlayerWindow(QWidget *parent = 0);
    ~PlayerWindow();



private slots:
    void open();

    void on_playButton_clicked();

    void on_rewindButton_clicked();

    void on_stopButton_clicked();

    void on_fastForwardButton_clicked();

	void on_timerTick();

	void on_timerRefreshUI();

private:
    void UpdateDurationLabels(int64_t duration, int64_t currTime);

    void mouseDoubleClickEvent(QMouseEvent *e);


    Ui::PlayerWindow *ui;
	Player* _player;
    QTimer _tickTimer;
    QTimer _refreshUITimer;

    QStringList fileNames;
};

#endif // PLAYERWINDOW_H
