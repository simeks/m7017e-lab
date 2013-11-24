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
class VideoWidget;
class PlaylistWindow;
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
	
	/// @brief Toggles fullscreen mode
	void ToggleFullscreen();
	
	/// @brief Toggles the playlist
	void TogglePlaylist();

	/// @return True if the player is currently in fullscreen mode.
	bool IsFullscreen() const;

private slots:
    void open();
	void fullscreen();
	void playlist();

    void on_playButton_clicked();

    void on_rewindButton_toggled();

    void on_stopButton_clicked();

    void on_fastForwardButton_toggled();

	void on_timerTick();

    void on_timerRefreshUI();

private:
	/// @param duration Duration in milliseconds.
	/// @param currTime Current time in milliseconds.
    void UpdateDurationLabels(int duration, int currTime);


    Ui::PlayerWindow *ui;
	PlaylistWindow* _playlist_window;
	PlaybackSlider* _slider;
	VideoWidget* _video_widget;

	Player* _player;
    QTimer _tick_timer;
    QTimer _refresh_ui_timer;

	bool _fullscreen;
	bool _playlist_visible;

};

#endif // PLAYERWINDOW_H
