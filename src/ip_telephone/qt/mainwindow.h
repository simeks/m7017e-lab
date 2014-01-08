#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../client.h"
#include "settings_dialog.h"

namespace Ui {
class MainWindow;
}

class Client;
class Settings_Dialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
	void ShowIncomingCallPanel();
	void HideActiveCallPanel();
	void HideIncomingCallPanel();
	void HideCallingPanel();
	void ShowActiveCallPanel();
	void ShowMainWindow();

private slots:

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::MainWindow *ui;

    Client* _client;
    Settings_Dialog* _settings_dialog;
};

#endif // MAINWINDOW_H
