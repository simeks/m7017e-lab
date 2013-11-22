#include "playerwindow.h"
#include "ui_playerwindow.h"
#include <QFileDialog>
#include "../player.h"

PlayerWindow::PlayerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PlayerWindow)

{
    ui->setupUi(this);
    playing = false;
	_player = new Player();
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
    fileNames = QFileDialog::getOpenFileNames(this, tr("Open Files"));
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

void PlayerWindow::on_fullScreenButton_clicked(bool checked)
{
    ui->widget->showFullScreen();
}
