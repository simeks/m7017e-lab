#include "active_call_dialog.h"
#include "ui_active_call_dialog.h"

Active_Call_Dialog::Active_Call_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Active_Call_Dialog)
{
    ui->setupUi(this);
}

Active_Call_Dialog::~Active_Call_Dialog()
{
    delete ui;
}
