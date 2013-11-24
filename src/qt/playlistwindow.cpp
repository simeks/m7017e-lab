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
		// We only want to show the file name in the playlist, not the whole path.
		std::string file_name = util::GetFileName(it.Next());

		ui->listWidget->addItem(file_name.c_str());
	}
}