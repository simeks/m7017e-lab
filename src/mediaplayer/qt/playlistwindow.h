#ifndef PLAYLISTWINDOW_H
#define PLAYLISTWINDOW_H

#include <QWidget>
#include <QListWidget>
#include "shared/common.h"

namespace Ui {
class PlaylistWindow;
}

class QListWidgetItem;
class Playlist;
class Player;

/// A simple window showing a list of all entries in the active Playlist.
class PlaylistWindow : public QWidget
{
    Q_OBJECT;
	
	/// A custom list item that stores the index of the item in the active playlist.
	class PlaylistItem : public QListWidgetItem
	{
	public:
		PlaylistItem(int playlist_index) : _playlist_index(playlist_index) { }
		~PlaylistItem() { }

		int PlaylistIndex() const { return _playlist_index; }

	private:
		int _playlist_index;
	};

public:
    explicit PlaylistWindow(Player* player, QWidget *parent = 0);
    ~PlaylistWindow();

	/// Updates the playlist, this will clear all previous entries and refill it with whatever is in the current playlist.
	void UpdatePlaylist(const Playlist& playlist);

protected slots:
	void ItemDoubleClicked(QListWidgetItem* item);

private:
    Ui::PlaylistWindow *ui;

	Player* _player;

};

#endif // PLAYLISTWINDOW_H
