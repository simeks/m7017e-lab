#include "playerwindow.h"
#include "ui_playerwindow.h"
#include <QFileDialog>
#include <QPalette>

PlayerWindow::PlayerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PlayerWindow)


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
        }
    else
        {
            QIcon pauseIcon(":images/pauseButton.png");
            playing = true;
            ui->playButton->setIcon(pauseIcon);
        }
}

void PlayerWindow::on_rewindButton_clicked()
{

}

void PlayerWindow::on_stopButton_clicked()
{

}

void PlayerWindow::on_fastForwardButton_clicked()
{

}

void PlayerWindow::on_fullScreenButton_clicked()
{

}
