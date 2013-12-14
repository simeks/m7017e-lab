#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "shared/common.h"
#include "shared/configvalue.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _connect_dialog(NULL),
    _client(new Client(this))
{
    ui->setupUi(this);

    _connect_dialog = new ConnectDialog(_client, this);

    ui->lineEdit->setDisabled(true);
    ui->treeWidget->hideColumn(0);
	ui->treeWidget->setExpandsOnDoubleClick(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_lineEdit_returnPressed()
{
    QString text = ui->lineEdit->text();

    if (!text.isEmpty())
    {
        _client->SendChatMessage(_user_name ,text);
        ui->lineEdit->clear();
    }
}

void MainWindow::AppendMessage(QString text)
{
    ui->textEdit->append(text);
}

void MainWindow::on_muteVolume_toggled(bool toggled)
{
    _client->MuteVolume(toggled);
}

void MainWindow::on_muteMic_toggled(bool toggled)
{
    _client->MuteMic(toggled);
}

void MainWindow::on_actionConnect_triggered()
{
    _connect_dialog->open();
}

void MainWindow::Connected()
{
    ui->lineEdit->setEnabled(true);
    ui->treeWidget->showColumn(0);
}
void MainWindow::SetChannels(const ConfigValue& channels)
{
	if(!channels.IsArray())
	{
		return;
	}

	for(uint32_t i = 0; i < channels.Size(); ++i)
	{
		if(!channels[i]["id"].IsNumber() || !channels[i]["name"].IsString())
			continue;

		int parent_id = -1;
		if(channels[i]["parent_id"].IsNumber())
			parent_id = channels[i]["parent_id"].AsInt();
		
		QTreeWidgetItem* channel_item = new QTreeWidgetItem();
		channel_item->setText(0, QString("[Channel]") + channels[i]["name"].AsString());

		// We only add root channels here, the rest will be added in the next pass
		if(parent_id == -1)
		{
			ui->treeWidget->addTopLevelItem(channel_item);
		}

		int channel_id = channels[i]["id"].AsInt();
		_channels[channel_id] = channel_item; 
	}

	// Here we run a second pass linking any child channels,
	//	we do this here because we can now be assured that the parent channels are created.
	for(uint32_t i = 0; i < channels.Size(); ++i)
	{
		if(!channels[i]["id"].IsNumber() || !channels[i]["name"].IsString())
			continue;

		int parent_id = -1;
		if(channels[i]["parent_id"].IsNumber())
			parent_id = channels[i]["parent_id"].AsInt();

		if(parent_id == -1) // Root channels are already linked
			continue;

		int channel_id = channels[i]["id"].AsInt();
		QTreeWidgetItem* child = _channels[channel_id];

		std::map<int, QTreeWidgetItem*>::iterator it = _channels.find(parent_id);
		if(it != _channels.end())
		{
			QTreeWidgetItem* parent = _channels[parent_id];
			parent->addChild(child);
		}
		else
		{
			// No parent found so we have no use for this element, discard it.
			delete child;
			_channels.erase(channel_id);
		}
	}
	ui->treeWidget->expandAll();
}

void MainWindow::SetUsers(const ConfigValue& users)
{
	if(!users.IsArray())
	{
		return;
	}

	for(uint32_t i = 0; i < users.Size(); ++i)
	{
		if(!users[i]["id"].IsNumber() || !users[i]["name"].IsString() || !users[i]["channel"].IsNumber())
			continue;

		int channel_id = users[i]["channel"].AsInt();
		int user_id = users[i]["id"].AsInt();
		
		QTreeWidgetItem* user_item = new QTreeWidgetItem();
		user_item->setText(0, QString("[User]") + users[i]["name"].AsString());

		std::map<int, QTreeWidgetItem*>::iterator it = _channels.find(channel_id);
		if(it != _channels.end())
		{
			QTreeWidgetItem* channel_item = _channels[channel_id];
			channel_item->addChild(user_item);
		}
		_users[user_id] = user_item; 
	}
}

void MainWindow::ClearTree()
{
	ui->treeWidget->clear();
	_channels.clear();
	_users.clear();
}

void MainWindow::SetUserName(const QString &username)
{
    _user_name = username;
}

void MainWindow::Disconnected()
{
    ui->lineEdit->setEnabled(false);
	ui->treeWidget->hideColumn(0);
}

void MainWindow::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
	for(std::map<int, QTreeWidgetItem*>::iterator it = _channels.begin(); it != _channels.end(); ++it)
	{
		if(it->second == item)
		{
			_client->ChangeChannel(it->first);
		}
	}
}

void MainWindow::ChangeUserChannel(int user_id, int channel_id)
{
	std::map<int, QTreeWidgetItem*>::iterator user_it = _users.find(user_id);
	if(user_it == _users.end())
		return; // No user with the specified id

	std::map<int, QTreeWidgetItem*>::iterator channel_it = _channels.find(channel_id);
	if(channel_it == _channels.end())
		return; // No channel with the specified id	
	
	QTreeWidgetItem* old_channel = user_it->second->parent();
	if(old_channel)
		old_channel->removeChild(user_it->second);

	QTreeWidgetItem* new_channel = channel_it->second;
	if(new_channel)
		new_channel->addChild(user_it->second);
}

void MainWindow::AddUser(int user_id, const std::string& name, int channel_id)
{
	QTreeWidgetItem* user_item;

	// Check if a user with the given id already exists
	std::map<int, QTreeWidgetItem*>::iterator user_it = _users.find(user_id);
	if(user_it == _users.end())
	{
		user_item = new QTreeWidgetItem();

		// If not: Create a new item in the tree
		user_item->setText(0, QString("[User]") + QString::fromStdString(name));

		// Insert it to our container of users
		_users[user_id] =  user_item;

		return; 
	}
	else
	{
		user_item = user_it->second;

		QString qname = QString("[User]") + QString::fromStdString(name);

		// If it already exists, make sure to update its name in case it have changed
		user_item->setText(0, qname);

		// We also detach it from any previous channels
		QTreeWidgetItem* parent = user_item->parent();
		if(parent)
			parent->removeChild(user_item);
	}

	// Set the channel
	std::map<int, QTreeWidgetItem*>::iterator channel_it = _channels.find(channel_id);
	if(channel_it == _channels.end())
	{
		 // No channel with the specified id, put the user in root
		_channels[0]->addChild(user_item);
		return;
	}
	channel_it->second->addChild(user_item);
}
void MainWindow::RemoveUser(int user_id)
{
	std::map<int, QTreeWidgetItem*>::iterator user_it = _users.find(user_id);
	if(user_it == _users.end())
		return; // No user with the specified id

	QTreeWidgetItem* parent = user_it->second->parent();
	if(parent)
		parent->removeChild(user_it->second);

	delete user_it->second;
	_users.erase(user_it);
}

