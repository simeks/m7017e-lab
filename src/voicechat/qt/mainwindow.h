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
class ConfigValue;

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

	/// @brief Fills the window with the channel layout.
	void SetChannels(const ConfigValue& channels);
	
	/// @brief Fills the window with the server users.
	void SetUsers(const ConfigValue& users);

	/// @brief Clears the complete tree view
	void ClearTree();

    /// Appends a message to the chat window
    void AppendMessage(QString text);

    void SetUserName(const QString &username);

	/// @brief Changes the channel for a specified user.
	/// @param user_id The unique id for the user that have changed its channel.
	/// @param channel_id The id of the new channel.
    void ChangeUserChannel(int user_id, int channel_id);
	
	/// @brief Adds the specified user from the tree
	void AddUser(int user_id, const std::string& name, int channel_id);

	/// @brief Removes the specified user from the tree
	void RemoveUser(int user_id);

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
    QTreeWidgetItem* name;
    QString _user_name;

	std::map<int, QTreeWidgetItem*> _channels; // channel_id => WidgetItem 
	std::map<int, QTreeWidgetItem*> _users; // user_id => WidgetItem 
};

#endif // MAINWINDOW_H
