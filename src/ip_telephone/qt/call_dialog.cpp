#include "call_dialog.h"
#include "ui_call_dialog.h"

Call_Dialog::Call_Dialog(Client* client, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Call_Dialog),
	_client(client)
{
    ui->setupUi(this);
}

Call_Dialog::~Call_Dialog()
{
    delete ui;
}

// Callback for when the Stop button is clicked
void Call_Dialog::on_pushButton_clicked()
{
    this->close();
	_client->InterruptCall();
}
