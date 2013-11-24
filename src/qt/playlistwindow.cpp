#include "../common.h"

#include "playlistwindow.h"
#include "ui_playlistwindow.h"

#include "../playlist.h"
#include "../player.h"

PlaylistWindow::PlaylistWindow(Player* player, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlaylistWindow),
	_player(player)
{
    ui->setupUi(this);
	connect(ui->listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(ItemDoubleClicked(QListWidgetItem*)));
}

PlaylistWindow::~PlaylistWindow()
{
	FreeItems();
    delete ui;
}

void PlaylistWindow::TrackStarted(int playlist_index)
{
	ui->listWidget->item(playlist_index)->setSelected(true);
}

void PlaylistWindow::UpdatePlaylist(const Playlist& playlist)
{
	// Clear current entries
	ui->listWidget->clear();
	FreeItems();

	Playlist::Iterator it = playlist.CreateIterator();
	while(!it.End())
	{
		// We only want to show the file name in the playlist, not the whole path.
		std::string file_name = util::GetFileName(it.Next());

		PlaylistItem* item = new PlaylistItem(it.CurrentIndex());
		_items.push_back(item);

		item->setText(file_name.c_str());

		ui->listWidget->addItem(item);
	}
}

void PlaylistWindow::ItemDoubleClicked(QListWidgetItem* item)
{
	PlaylistItem* playlist_item = (PlaylistItem*)item;
	_player->PlayTrack(playlist_item->PlaylistIndex());
}

void PlaylistWindow::FreeItems()
{
	std::vector<PlaylistItem*>::iterator it, end;
	it = _items.begin();
	end = _items.end();
	for( ; it != end; ++it)
	{
		delete *it;
	}
	_items.clear();
}