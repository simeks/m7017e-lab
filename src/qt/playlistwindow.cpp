#include "../common.h"

#include "playlistwindow.h"
#include "ui_playlistwindow.h"

#include "../playlist.h"

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

void PlaylistWindow::UpdatePlaylist(const Playlist& playlist)
{
	// Clear current entries
	ui->listWidget->clear();

	Playlist::Iterator it = playlist.CreateIterator();
	while(!it.End())
	{
		ui->listWidget->addItem(it.Next().c_str());
	}
}