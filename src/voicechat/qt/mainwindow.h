#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "connectdialog.h"
#include "../client.h"
#include <QTreeWidgetItem>
#include <QDebug>

namespace Ui {
class MainWindow;
}

class Client;
class ConnectDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    /// @brief Called when a user has connected to the server.
    void Connected();

    /// @brief Called when a user has disconnected from the server.
	void Disconnected();

    /// Called when a message is received and writes the message to the chat.
    void OnMessageRecieved(QString from, QString text);

    void SetUserName(const QString &username);

    void OnUserStateChanged(const QString &username, const QString &prevChannel, const QString &newChannel);

private slots:

    /// @brief Called when a user has written a chat message and then pressed the return key.
    void on_lineEdit_returnPressed();

    /// @brief Called when the mute-volume button is clicked on.
    void on_muteVolume_toggled(bool toggled);

    /// @brief Called when the mute-microphone button is clicked on.
    void on_muteMic_toggled(bool toggled);

    /// @brief Called when the connect button is clicked on, and opens the connect dialog.
    void on_actionConnect_triggered();

    /// Called when a user doubleclicks a channel, and moves the user to that channel.
    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

private:
    Ui::MainWindow *ui;
    ConnectDialog* _connect_dialog;
    Client* _client;
    QTreeWidgetItem* _current_channel;
    QTreeWidgetItem* name;
    QString _user_name;
};

#endif // MAINWINDOW_H
