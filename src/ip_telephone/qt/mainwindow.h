#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "call_dialog.h"
#include "../client.h"
#include "incoming_call_dialog.h"
#include "settings_dialog.h"

namespace Ui {
class MainWindow;
}

class Call_Dialog;
class Incoming_Call_Dialog;
class Client;
class Settings_Dialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
	void ShowIncomingCallDialog();

private slots:

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    Client* _client;
    Call_Dialog* _call_dialog;
	Incoming_Call_Dialog* _incoming_call_dialog;
    Settings_Dialog* _settings_dialog;
};

#endif // MAINWINDOW_H
