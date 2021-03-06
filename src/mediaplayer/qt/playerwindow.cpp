#include "shared/common.h"
#include "../player.h"

#include "playerwindow.h"
#include "ui_playerwindow.h"
#include "playlistwindow.h"
#include "playbackslider.h"
#include "videowidget.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTime>
#include <QKeyEvent>

#include "../player.h"

PlayerWindow::PlayerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PlayerWindow),
    _slider(NULL),
    _muteButton(NULL),
    _playlist_window(NULL),
    _volumeSlider(NULL),
    _player(new Player(this)),
	_tick_timer(this),
	_refresh_ui_timer(this),
	_fullscreen(false),
	_playlist_visible(false)
{
    ui->setupUi(this);

    // hide the status bar at the bottom of the screen
    this->statusBar()->hide();

	// Create our custom video widget
    _video_widget = new VideoWidget(_player, this);

	// Insert the widget to the layout
	ui->verticalLayout->insertWidget(0, _video_widget);

	// Create our custom slider
	_slider = new PlaybackSlider(_player, this);
	
	// Insert our slider to the layout
	ui->horizontalLayout->insertWidget(1, _slider);

    // Create the mute button
    _muteButton = new QToolButton(this);

	_playlist_window = new PlaylistWindow(_player, this);
	// Hide playlist until it's toggled by the user.
    _playlist_window->hide();

    // Set the size for the mute button
    _muteButton->setFixedSize(47, 47);

    // Make the mute button checkable
    _muteButton->setCheckable(true);

    // Give the mute button an icon
    QIcon muteButton(":images/muteButton.png");
    _muteButton->setIcon(muteButton);
    QSize muteIconSize(41, 41);
    _muteButton->setIconSize(muteIconSize);

    // Create the volume slider
    _volumeSlider = new QSlider(this);

    // Set the properties for the volume slider
    _volumeSlider->setOrientation(Qt::Horizontal);
    _volumeSlider->setFixedWidth(100);
    _volumeSlider->setTickInterval(1);
    _volumeSlider->setMaximum(100);
    _volumeSlider->setMinimum(0);
    _volumeSlider->setSliderPosition(100);

    // Create a horizontal layout for the volume button and volume slider
    _QHboxVolume = new QHBoxLayout();
    _QHboxVolume->addWidget(_muteButton);
    _QHboxVolume->addWidget(_volumeSlider);
    _QHboxVolume->setAlignment(Qt::AlignRight);

    // Create a horizontal layout and add the buttons and the volume slider to it
    _QHboxControls = new QHBoxLayout();
    _QHboxControls->addWidget(ui->frame);
    _QHboxControls->addLayout(_QHboxVolume);
    _QHboxControls->setAlignment(Qt::AlignLeft);

    // Add the horizontal layout to the vertical layout inside the UI
    ui->verticalLayout->addLayout(_QHboxControls);

    // Disable all buttons and sliders when no file is opened
    ui->playButton->setDisabled(true);
    ui->rewindButton->setDisabled(true);
    ui->stopButton->setDisabled(true);
    ui->fastForwardButton->setDisabled(true);
    _muteButton->setDisabled(true);
    _volumeSlider->setDisabled(true);
	_slider->setDisabled(true);
	_slider->setValue(0);

    connect(ui->actionOpen_File, SIGNAL(triggered()), this, SLOT(open()));
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));
	connect(ui->actionFullscreen, SIGNAL(triggered()), this, SLOT(fullscreen()));
	connect(ui->actionPlaylist, SIGNAL(triggered()), this, SLOT(playlist()));
    connect(_muteButton, SIGNAL(toggled(bool)), this, SLOT(SetMuted(bool)));
    connect(_volumeSlider, SIGNAL(valueChanged(int)), this, SLOT(SetVolume(int)));
	
	// Make sure player outputs video to our video widget
	_player->SetVideoOutput(_video_widget->winId());

	// Tick every 25ms
	_tick_timer.setInterval(25);
	_tick_timer.start();

	connect(&_tick_timer, SIGNAL(timeout()), this, SLOT(on_timerTick()));

	// Tick every 1 sec
	_refresh_ui_timer.setInterval(1000);
	_refresh_ui_timer.start();

    connect(&_refresh_ui_timer, SIGNAL(timeout()), this, SLOT(on_timerRefreshUI()));
}

PlayerWindow::~PlayerWindow()
{
	delete _playlist_window;
	delete _player;
    delete _slider;
	delete ui;
}

void PlayerWindow::TrackStarted(int, const std::string& track_name)
{
	// Clear video widget to make sure it isn't showing any left over frame
	_video_widget->update();

	if(track_name.empty())
	{
		setWindowTitle(QString("MediaPlayer"));
	}
	else
	{
		setWindowTitle(QString("MediaPlayer - ") + track_name.c_str());
	}

	QIcon pauseIcon(":images/pauseButton.png");
	ui->playButton->setIcon(pauseIcon);
    ui->playButton->setEnabled(true);
    ui->rewindButton->setEnabled(true);
    ui->stopButton->setEnabled(true);
    ui->fastForwardButton->setEnabled(true);
    _muteButton->setEnabled(true);
    _slider->setEnabled(true);
    _volumeSlider->setEnabled(true);
}

void PlayerWindow::PrintError(const std::string& msg)
{
	QMessageBox errorBox;
	errorBox.setText(QString("There was an error: ") + msg.c_str());
	errorBox.exec();
}
void PlayerWindow::StreamEnded()
{
	QIcon pauseIcon(":images/playButton.png");
	ui->playButton->setIcon(pauseIcon);
    ui->playButton->setEnabled(false);
    ui->rewindButton->setEnabled(false);
    ui->stopButton->setEnabled(false);
    ui->fastForwardButton->setEnabled(false);
    _muteButton->setEnabled(false);
    _slider->setEnabled(false);
    _volumeSlider->setEnabled(false);

	_slider->setDisabled(true);
	_slider->setValue(0);
	_slider->setMinimum(0);
    _slider->setMaximum(0);
	
	setWindowTitle(QString("MediaPlayer"));

	// Clear video widget to make sure it isn't showing any left over frame
	_video_widget->update();
}

void PlayerWindow::ToggleFullscreen()
{
	if(_fullscreen)
	{
		// First we set it to show normally
		_video_widget->showNormal();
		
		_video_widget->setParent(this);

		// Then we attach it to our layout again.
		ui->verticalLayout->insertWidget(0, _video_widget);

		_fullscreen = false;
	}
	else
	{
		// Before we set the widget to fullscreen we need to detach it from our window layout
		ui->verticalLayout->removeWidget(_video_widget);
		_video_widget->setParent(NULL);

		// Then we just set it to fullscreen
		_video_widget->showFullScreen();
		_video_widget->setFocus();

		_fullscreen = true;
	}
}void PlayerWindow::TogglePlaylist()
{
	if(_playlist_visible)
	{
		ui->horizontalLayout_3->removeWidget(_playlist_window);
		_playlist_window->hide();

		_playlist_visible = false;
	}
	else
	{
		ui->horizontalLayout_3->addWidget(_playlist_window);
		_playlist_window->show();

		_playlist_visible = true;
	}
}


bool PlayerWindow::IsFullscreen() const
{
	return _fullscreen;
}

void PlayerWindow::open()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open Files"), "/", "(*.webm *.wav *.avi *.mp3 *.mp4 *.mkv *.)");

    if(fileNames.length() != 0)
	{
		/// Make sure to stop any previous playback.
		_player->Stop();

		// Clear the playlist first
		_player->GetPlaylist().Clear();

		// Push all opened files to the playlist
		QStringList::Iterator it, end;
		it = fileNames.begin(); end = fileNames.end();
		for( ; it != end; ++it)
		{
			_player->GetPlaylist().AddEntry(it->toLocal8Bit().constData());
		}

		// Start playing the first file in the playlist
		_player->PlayNext();
		
		// Update our playlist window
		_playlist_window->UpdatePlaylist(_player->GetPlaylist());
		
		QIcon pauseIcon(":images/pauseButton.png");
		ui->playButton->setIcon(pauseIcon);
        ui->playButton->setEnabled(true);
        ui->rewindButton->setEnabled(true);
        ui->stopButton->setEnabled(true);
        ui->fastForwardButton->setEnabled(true);
        _muteButton->setEnabled(true);
        _slider->setEnabled(true);
        _volumeSlider->setEnabled(true);

        _volumeSlider->setSliderPosition(100);

	}
}
void PlayerWindow::fullscreen()
{
	ToggleFullscreen();
}
void PlayerWindow::playlist()
{
	TogglePlaylist();
}

void PlayerWindow::on_playButton_clicked()
{
	if(_player->IsPlaying())
    {
        QIcon playIcon(":images/playButton.png");
        ui->playButton->setIcon(playIcon);
        _player->Pause();

    }
    else
    {
        QIcon pauseIcon(":images/pauseButton.png");
        ui->playButton->setIcon(pauseIcon);
        _player->Play();
    }
}

void PlayerWindow::on_rewindButton_toggled()
{
    _player->ToggleRewind();
}

void PlayerWindow::on_stopButton_clicked()
{
	_player->Stop();
}

void PlayerWindow::on_fastForwardButton_toggled()
{
    _player->ToggleFastForward();
}

void PlayerWindow::on_timerTick()
{
	_player->Tick();
}

void PlayerWindow::on_timerRefreshUI()
{
	if(_player->IsPlaying())
	{
		int duration = _player->GetDuration();
		int currentTime = _player->GetTimeElapsed();
		UpdateDurationLabels(duration, currentTime);

        _slider->setMaximum(duration);
        _slider->setMinimum(0);
        _slider->setValue((currentTime));
	}
}

void PlayerWindow::UpdateDurationLabels(int duration, int currTime)
{
	QString totalDurationString;
	QString currentTimeString;

	long durationSeconds = duration / 1000; // Convert duration from milliseconds to seconds
	long currentTimeSeconds = currTime / 1000; // Convert position from milliseconds to seconds

	// Convert time to a more readable format before printing.

	QTime totalDuration((durationSeconds/3600)%60, (durationSeconds/60)%60, durationSeconds%60, (durationSeconds*1000)%1000);
	QTime currentTime((currentTimeSeconds/3600)%60, (currentTimeSeconds/60)%60, currentTimeSeconds%60, (currentTimeSeconds*1000)%1000);
	QString format = "mm:ss";
	if (durationSeconds > 3600)
            format = "hh:mm:ss";

    totalDurationString = totalDuration.toString(format);
	ui->totalDuration->setText(totalDurationString);

	currentTimeString = currentTime.toString(format);
	ui->timeElapsed->setText(currentTimeString);
}

void PlayerWindow::SetMuted(bool muted)
{
    _player->SetMuted(muted);

    if(muted)
    {
        // Save the volume slider position, and then set the slider position to 0 when the mute button is toggled
        _sliderPosition = _volumeSlider->sliderPosition();
        _volumeSlider->setSliderPosition(0);
    }
    else
    {
        // Set the volume slider position to the position it had before the mute button was toggled
        _volumeSlider->setSliderPosition(_sliderPosition);
    }


}

void PlayerWindow::SetVolume(int volume)
{
    _player->SetVolume(volume);

    if(_volumeSlider->sliderPosition() == 0)
    {
        if(!_muteButton->isDown())
        {
            _muteButton->setDown(true);
        }
    }
    else
    {
        if(_muteButton->isDown())
        {
            _muteButton->setDown(false);
        }
    }
}
