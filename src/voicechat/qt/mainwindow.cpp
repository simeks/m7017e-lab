#include "mainwindow.h"
#include "ui_mainwindow.h"


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

void MainWindow::OnMessageRecieved(QString from, QString text)
{
    ui->textEdit->append(from + ": " + text);
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

    _current_channel = ui->treeWidget->topLevelItem(0);
    _current_channel->setExpanded(true);

    name = new QTreeWidgetItem(0);
    name->setText(0, _user_name);
    name->setDisabled(true);
    name->setTextColor(0, Qt::color1);

    _current_channel->addChild(name);

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
    if(!item->isDisabled())
        _client->ChangeUserState(_user_name, _current_channel->text(0), item->text(0));
}

void MainWindow::OnUserStateChanged(const QString &username, const QString &prevChannel, const QString &newChannel)
{
    if(prevChannel != newChannel)
    {
        QTreeWidgetItem* _prev_channel;
        QTreeWidgetItem* _new_channel;

        for(int i = 0; i < ui->treeWidget->topLevelItemCount(); i++)
        {
            if(ui->treeWidget->topLevelItem(i)->text(0) == prevChannel)
                _prev_channel = ui->treeWidget->topLevelItem(i);

            if(ui->treeWidget->topLevelItem(i)->text(0) == newChannel)
                _new_channel = ui->treeWidget->topLevelItem(i);

            for(int i = 0; i < _prev_channel->childCount(); i++)
            {
                if(_prev_channel->child(i)->text(0) == username)
                {
                    _new_channel->addChild(_prev_channel->child(i));
                    _prev_channel->removeChild(_prev_channel->child(i));

                }
            }
        }
    }
}
