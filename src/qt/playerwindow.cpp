#include "../common.h"
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
	_playlist_window(NULL),
    _player(new Player(this)),
	_tick_timer(this),
	_refresh_ui_timer(this),
	_fullscreen(false),
	_playlist_visible(false)
{
    ui->setupUi(this);

	// Create our custom video widget
	_video_widget = new VideoWidget(_player, this);
	
	// Insert the widget to the layout
	ui->verticalLayout->insertWidget(0, _video_widget);

	// Create our custom slider
	_slider = new PlaybackSlider(_player, this);
	
	// Insert our slider to the layout
	ui->horizontalLayout->insertWidget(1, _slider);

	_playlist_window = new PlaylistWindow(this);
	// Hide playlist until it's toggled by the user.
	_playlist_window->hide();

    ui->playButton->setDisabled(true);
    ui->rewindButton->setDisabled(true);
    ui->stopButton->setDisabled(true);
    ui->fastForwardButton->setDisabled(true);
	_slider->setDisabled(true);
	_slider->setValue(0);

    connect(ui->actionOpen_File, SIGNAL(triggered()), this, SLOT(open()));
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));
	connect(ui->actionFullscreen, SIGNAL(triggered()), this, SLOT(fullscreen()));
	connect(ui->actionPlaylist, SIGNAL(triggered()), this, SLOT(playlist()));
	
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

	SetTrackName("");
}
void PlayerWindow::SetTrackName(const std::string& msg)
{
	if(msg.empty())
	{
		setWindowTitle(QString("MediaPlayer"));
	}
	else
	{
		setWindowTitle(QString("MediaPlayer - ") + msg.c_str());
	}
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
    fileNames = QFileDialog::getOpenFileNames(this, tr("Open Files"), "/", "(*.webm *.wav *.avi *.mp3 *.mp4 *.)");

    if(fileNames.length() != 0)
	{
		// Stop any old media playing before starting any new.
		if(_player->IsPlaying())
			_player->Stop();

		_player->PlayMedia(fileNames[0].toLocal8Bit().constData());
		QIcon pauseIcon(":images/pauseButton.png");
		ui->playButton->setIcon(pauseIcon);
        ui->playButton->setEnabled(true);
        ui->rewindButton->setEnabled(true);
        ui->stopButton->setEnabled(true);
        ui->fastForwardButton->setEnabled(true);
        _slider->setEnabled(true);

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
