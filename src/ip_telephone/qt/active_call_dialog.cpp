#include "active_call_dialog.h"
#include "ui_active_call_dialog.h"

Active_Call_Dialog::Active_Call_Dialog(Client* client, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Active_Call_Dialog),
	_client(client)
{
    ui->setupUi(this);
}

Active_Call_Dialog::~Active_Call_Dialog()
{
    delete ui;
}

// Callback for when the Hang Up button is clicked.
void Active_Call_Dialog::on_pushButton_clicked()
{
    _client->HangUpActiveCall();
    this->close();
}
