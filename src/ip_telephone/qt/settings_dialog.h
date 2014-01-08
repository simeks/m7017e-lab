#ifndef SETTINGS_DIALOG_H
#define SETTINGS_DIALOG_H

#include <QDialog>
#include "../client.h"

namespace Ui {
class Settings_Dialog;
}

class Client;

class Settings_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Settings_Dialog(Client* client, QWidget *parent = 0);
    ~Settings_Dialog();

private slots:
	/// Callback for when the Save button is clicked
    void on_pushButton_clicked();

private:
    Ui::Settings_Dialog *ui;
    Client* _client;
};

#endif // SETTINGS_DIALOG_H
