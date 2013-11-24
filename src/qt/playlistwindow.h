#ifndef PLAYLISTWINDOW_H
#define PLAYLISTWINDOW_H

#include <QWidget>

namespace Ui {
class PlaylistWindow;
}

class Playlist;
class PlaylistWindow : public QWidget
{
    Q_OBJECT

public:
    explicit PlaylistWindow(QWidget *parent = 0);
    ~PlaylistWindow();

	void UpdatePlaylist(const Playlist& playlist);

private:
    Ui::PlaylistWindow *ui;
};

#endif // PLAYLISTWINDOW_H
