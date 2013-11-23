#include "../common.h"
#include "../player.h"

#include "playerwindow.h"
#include "ui_playerwindow.h"
#include <QFileDialog>
#include "../player.h"
#include <QTime>

PlayerWindow::PlayerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PlayerWindow),
    _player(new Player),
	_tickTimer(this),
	_refreshUITimer(this),
	_playing(false)
{
    ui->setupUi(this);

    ui->playButton->setDisabled(true);
    ui->rewindButton->setDisabled(true);
    ui->stopButton->setDisabled(true);
    ui->fastForwardButton->setDisabled(true);
    ui->slider->setDisabled(true);

    connect(ui->actionOpen_File, SIGNAL(triggered()), this, SLOT(open()));
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));

	// Make sure player outputs video to our video widget
	_player->SetVideoOutput(ui->widget->winId());

	// Tick every 25ms
	_tickTimer.setInterval(25);
	_tickTimer.start();

	connect(&_tickTimer, SIGNAL(timeout()), this, SLOT(on_timerTick()));

	// Tick every 1 sec
	_refreshUITimer.setInterval(1000);
	_refreshUITimer.start();

    connect(&_refreshUITimer, SIGNAL(timeout()), this, SLOT(on_timerRefreshUI()));
    connect(ui->slider, SIGNAL(sliderMoved(int)), this, SLOT(SliderMoved(int)));

}

PlayerWindow::~PlayerWindow()
{
	delete _player;
    delete ui;
}
void PlayerWindow::open()
{
    fileNames = QFileDialog::getOpenFileNames(this, tr("Open Files"), "/", "(*.webm *.wav *.avi *.mp3 *.mp4 *.)");

    if(fileNames.length() != 0)
	{
        _player->PlayMedia(fileNames[0]);
		QIcon pauseIcon(":images/pauseButton.png");
		ui->playButton->setIcon(pauseIcon);
		_playing = true;
        ui->playButton->setEnabled(true);
        ui->rewindButton->setEnabled(true);
        ui->stopButton->setEnabled(true);
        ui->fastForwardButton->setEnabled(true);
        ui->slider->setEnabled(true);

	}
}


void PlayerWindow::on_playButton_clicked()
{
    if(_playing)
    {
        QIcon playIcon(":images/playButton.png");
        _playing = false;
        ui->playButton->setIcon(playIcon);
        _player->Pause();

    }
    else
    {
        QIcon pauseIcon(":images/pauseButton.png");
        _playing = true;
        ui->playButton->setIcon(pauseIcon);
        _player->Play();
    }
}

void PlayerWindow::on_rewindButton_clicked()
{
    _player->ReWind();
}

void PlayerWindow::on_stopButton_clicked()
{
	_player->Stop();
}

void PlayerWindow::on_fastForwardButton_clicked()
{
    _player->FastForward();
}

void PlayerWindow::on_timerTick()
{
	_player->Tick();
}

void PlayerWindow::on_timerRefreshUI()
{
	if(_playing)
	{
		int64_t duration = _player->GetDuration();
		int64_t currentTime = _player->GetTimeElapsed();
		UpdateDurationLabels(duration, currentTime);

        ui->slider->setMaximum(duration / 1000000000);

        if (!ui->slider->isSliderDown())
        {
            ui->slider->setValue(currentTime / 1000000000);
		}
	}
}

void PlayerWindow::UpdateDurationLabels(int64_t duration, int64_t currTime)
{
	QString totalDurationString;
	QString currentTimeString;
	long durationSeconds = duration/1000000000;
	long currentTimeSeconds = currTime/1000000000;

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

void PlayerWindow::mouseDoubleClickEvent(QMouseEvent *e)
{
    QWidget::mouseDoubleClickEvent(e);

}

void SliderMoved(int)
{

}
