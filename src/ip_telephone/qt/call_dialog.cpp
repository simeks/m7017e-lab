#include "call_dialog.h"
#include "ui_call_dialog.h"

Call_Dialog::Call_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Call_Dialog)
{
    ui->setupUi(this);
}

Call_Dialog::~Call_Dialog()
{
    delete ui;
}

void Call_Dialog::on_pushButton_clicked()
{
    this->close();
}
