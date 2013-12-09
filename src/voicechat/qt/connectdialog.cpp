#include "connectdialog.h"
#include "ui_connectdialog.h"

ConnectDialog::ConnectDialog(Client* client, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectDialog),
    _client(client)
{
    ui->setupUi(this);

}

ConnectDialog::~ConnectDialog()
{
    delete ui;
}

void ConnectDialog::on_pushButton_clicked()
{
    QString userName = ui->userNameEdit->text();
    QString serverIP = ui->serverIpEdit->text();
    QString serverPortText = ui->serverPortEdit->text();
    int serverPort = serverPortText.toInt();

    if (!userName.isEmpty() && !serverIP.isEmpty() && !serverPortText.isEmpty())
    {
        _client->ConnectClicked();
        _client->SetUserName(userName);
        _client->SetServerIp(serverIP);
        _client->SetServerPort(serverPort);

        this->close();
    }
    else
    {
        // Error, alla fält är inte fyllda
    }
}
