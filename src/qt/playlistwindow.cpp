#include "playlistwindow.h"
#include "ui_playlistwindow.h"

PlaylistWindow::PlaylistWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlaylistWindow)
{
    ui->setupUi(this);

}

PlaylistWindow::~PlaylistWindow()
{
    delete ui;
}
