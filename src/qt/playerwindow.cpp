#include "playerwindow.h"
#include "ui_playerwindow.h"
#include <QFileDialog>
#include "../player.h"
#include <QTime>

PlayerWindow::PlayerWindow(Player* player, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PlayerWindow),
    _player(player)

{
    ui->setupUi(this);
    playing = false;
    connect(ui->actionOpen_File, SIGNAL(triggered()), this, SLOT(open()));
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));
}

PlayerWindow::~PlayerWindow()
{
    delete ui;
}
WId PlayerWindow::GetOutputHandle() const
{
	return ui->widget->winId();
}
void PlayerWindow::open()
{
    fileNames = QFileDialog::getOpenFileNames(this, tr("Open Files"), "/", "Videos (*.webm *.wav *.avi)");

    if(fileNames.length() != 0)
        _player->PlayMedia(fileNames[0]);

	QIcon pauseIcon(":images/pauseButton.png");
	ui->playButton->setIcon(pauseIcon);
	playing = true;

}

void PlayerWindow::on_playButton_clicked()
{
    if(playing)
        {
            QIcon playIcon(":images/playButton.png");
            playing = false;
            ui->playButton->setIcon(playIcon);
			_player->Pause();
			
        }
    else
        {
            QIcon pauseIcon(":images/pauseButton.png");
            playing = true;
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
