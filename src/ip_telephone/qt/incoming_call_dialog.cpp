#include "incoming_call_dialog.h"
#include "ui_incoming_call_dialog.h"

Incoming_Call_Dialog::Incoming_Call_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Incoming_Call_Dialog)
{
    ui->setupUi(this);
}

Incoming_Call_Dialog::~Incoming_Call_Dialog()
{
    delete ui;
}

// Callback for when the Decline button is clicked
void Incoming_Call_Dialog::on_pushButton_2_clicked()
{
    this->close();
}

// Callback for when the Answer button is clicked
void Incoming_Call_Dialog::on_pushButton_clicked()
{
    this->close();
}
