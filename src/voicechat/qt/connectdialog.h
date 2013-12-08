#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

#include <QDialog>
#include "../client.h"

namespace Ui {
class ConnectDialog;
}

class MainWindow;
class Client;

class ConnectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectDialog(Client* client, QWidget *parent = 0);
    ~ConnectDialog();

private slots:
    void on_pushButton_clicked();

private:
    Ui::ConnectDialog *ui;
    Client* _client;
};

#endif // CONNECTDIALOG_H
