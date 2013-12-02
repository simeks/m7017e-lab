#ifndef PLAYERWINDOW_H
#define PLAYERWINDOW_H

#include "shared/common.h"
#include <QMainWindow>
#include <QTimer>
#include <QToolButton>
#include <QHBoxLayout>
#include <QSlider>


namespace Ui {
class PlayerWindow;
}


class Player;
class PlaybackSlider;
class VideoWidget;
class PlaylistWindow;

/// This is the main window of our media player.
class PlayerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PlayerWindow(QWidget *parent = 0);
    ~PlayerWindow();

	/// Called when a new track is started.
	/// @param playlist_index An index in the players playlist.
	/// @param track_name Name of the track.
	void TrackStarted(int playlist_index, const std::string& track_name);

	/// Creates a popup and shows an error message.
	void PrintError(const std::string& msg);

	/// Indicates that the stream has stopped.
	void StreamEnded();
	
	/// @brief Toggles fullscreen mode
	void ToggleFullscreen();
	
	/// @brief Toggles the playlist
	void TogglePlaylist();

	/// @return True if the player is currently in fullscreen mode.
	bool IsFullscreen() const;

private slots:
	/// @brief Callback for when a new media file has been opened.
    void open();
	
	
	void fullscreen();
	
	/// Callback for when the "show" button in the playlist menu is clicked.
	void playlist();

	/// Callback for when the play button is clicked. Starts to play the media file.
    void on_playButton_clicked();

	/// Callback for when the rewind button is clicked. Starts to play the media file backwards.
    void on_rewindButton_toggled();

	/// Callback for when the stop button is clicked. The media file will be closed.
    void on_stopButton_clicked();

	/// Callback for when the fast-forward button is clicked. Starts to play the media file at a 2x rate.
    void on_fastForwardButton_toggled();


	void on_timerTick();

	/// @brief Callback for when _refresh_ui_timer ticks, which is every second.
    void on_timerRefreshUI();

	/// @brief Mutes the sound.
    void SetMuted(bool muted);

	/// @brief Changes the volume
	/// @param volume The value of volume in percent.
    void SetVolume(int volume);

private:
	/// @brief Updates the text labels.
	/// @param duration Duration in milliseconds.
	/// @param currTime Current time in milliseconds.
    void UpdateDurationLabels(int duration, int currTime);

    Ui::PlayerWindow *ui;
	PlaylistWindow* _playlist_window;
	PlaybackSlider* _slider;
	VideoWidget* _video_widget;
    QToolButton* _muteButton;
    QHBoxLayout* _QHboxControls;
    QHBoxLayout* _QHboxVolume;
    QSlider* _volumeSlider;

	Player* _player;
    QTimer _tick_timer;
    QTimer _refresh_ui_timer;

	bool _fullscreen;
	bool _playlist_visible;
    int _sliderPosition;

};

#endif // PLAYERWINDOW_H
