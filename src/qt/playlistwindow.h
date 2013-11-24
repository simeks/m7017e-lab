#ifndef PLAYLISTWINDOW_H
#define PLAYLISTWINDOW_H

#include <QWidget>
#include <QListWidget>

namespace Ui {
class PlaylistWindow;
}

class QListWidgetItem;
class Playlist;
class Player;
class PlaylistWindow : public QWidget
{
    Q_OBJECT
	
	class PlaylistItem : public QListWidgetItem
	{
	public:
		PlaylistItem(int playlist_index) : _playlist_index(playlist_index) {}
		~PlaylistItem() {}

		int PlaylistIndex() const { return _playlist_index; }

	private:
		int _playlist_index;
	};

public:
    explicit PlaylistWindow(Player* player, QWidget *parent = 0);
    ~PlaylistWindow();

	void TrackStarted(int playlist_index);

	void UpdatePlaylist(const Playlist& playlist);

protected slots:
	void ItemDoubleClicked(QListWidgetItem* item);

private:
	/// Frees all items that have been allocated and stored in _items;
	void FreeItems();

    Ui::PlaylistWindow *ui;

	Player* _player;
	std::vector<PlaylistItem*> _items;

};

#endif // PLAYLISTWINDOW_H
