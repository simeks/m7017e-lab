#ifndef ACTIVE_CALL_DIALOG_H
#define ACTIVE_CALL_DIALOG_H

#include <QDialog>
#include "../client.h"

namespace Ui {
class Active_Call_Dialog;
}

class Client;

class Active_Call_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Active_Call_Dialog(Client* client, QWidget *parent = 0);
    ~Active_Call_Dialog();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Active_Call_Dialog *ui;
	Client* _client;
};

#endif // ACTIVE_CALL_DIALOG_H
