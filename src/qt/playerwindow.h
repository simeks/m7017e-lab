#ifndef PLAYERWINDOW_H
#define PLAYERWINDOW_H

#include "../common.h"
#include <QMainWindow>
#include <QTimer>


namespace Ui {
class PlayerWindow;
}



class Player;
class PlaybackSlider;
class PlayerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PlayerWindow(QWidget *parent = 0);
    ~PlayerWindow();

	/// Creates a popup and shows an error message.
	void PrintError(const std::string& msg);

	/// Indicates that the stream has stopped.
	void StreamEnded();

	/// Sets the current track name.
	void SetTrackName(const std::string& msg);

private slots:
    void open();

    void on_playButton_clicked();

    void on_rewindButton_toggled();

    void on_stopButton_clicked();

    void on_fastForwardButton_toggled();

	void on_timerTick();

    void on_timerRefreshUI();

private:
    void UpdateDurationLabels(int64_t duration, int64_t currTime);


    Ui::PlayerWindow *ui;
	PlaybackSlider* _slider;

	Player* _player;
    QTimer _tickTimer;
    QTimer _refreshUITimer;

    QStringList fileNames;
};

#endif // PLAYERWINDOW_H
