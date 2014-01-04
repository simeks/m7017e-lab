#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "call_dialog.h"
#include "../client.h"

namespace Ui {
class MainWindow;
}

class Call_Dialog;
class Client;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    Client* _client;
    Call_Dialog* _call_dialog;
};

#endif // MAINWINDOW_H
