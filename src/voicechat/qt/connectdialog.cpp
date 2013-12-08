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
    QString userName = ui->lineEdit->text();
    QString serverIP = ui->lineEdit_2->text();
    QString serverPort = ui->lineEdit_3->text();

    if (!userName.isEmpty() && !serverIP.isEmpty() && !serverPort.isEmpty())
    {
        _client->ConnectClicked();
        this->close();
    }
    else
    {
        // Error, alla fält är inte fyllda
    }
}
