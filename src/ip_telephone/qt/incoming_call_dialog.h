#ifndef INCOMING_CALL_DIALOG_H
#define INCOMING_CALL_DIALOG_H

#include <QDialog>
#include "../client.h"
#include "active_call_dialog.h"

namespace Ui {
class Incoming_Call_Dialog;
}

class Client;
class Active_Call_Dialog;

class Incoming_Call_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Incoming_Call_Dialog(Client* client, QWidget *parent = 0);
    ~Incoming_Call_Dialog();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::Incoming_Call_Dialog *ui;
    Client* _client;
	Active_Call_Dialog* _active_call_dialog;
};

#endif // INCOMING_CALL_DIALOG_H
