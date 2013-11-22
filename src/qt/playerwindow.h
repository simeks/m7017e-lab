#ifndef PLAYERWINDOW_H
#define PLAYERWINDOW_H

#include <QMainWindow>
#include "../player.h"

namespace Ui {
class PlayerWindow;
}

class PlayerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PlayerWindow(QWidget *parent = 0);
    ~PlayerWindow();

	/// Returns the window handle for the widget used for outputting video.
	WId GetOutputHandle() const;


private slots:
    void open();

    void on_playButton_clicked();

    void on_rewindButton_clicked();

    void on_stopButton_clicked();

    void on_fastForwardButton_clicked();

    void on_fullScreenButton_clicked(bool checked);

private:
    Ui::PlayerWindow *ui;
	Player* _player;
    QStringList fileNames;
    bool playing;
};

#endif // PLAYERWINDOW_H
