#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "connectdialog.h"
#include "../client.h"

namespace Ui {
class MainWindow;
}

class Client;
class ConnectDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void Connected();

private slots:
    void on_lineEdit_returnPressed();

    void on_muteVolume_toggled(bool toggled);

    void on_muteMic_toggled(bool toggled);

    void on_actionConnect_triggered();

private:
    Ui::MainWindow *ui;
    ConnectDialog* _connect_dialog;
    Client* _client;
};

#endif // MAINWINDOW_H
