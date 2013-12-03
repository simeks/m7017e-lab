#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_lineEdit_returnPressed();

    void on_muteVolume_toggled(bool toggled);

    void on_muteMic_toggled(bool toggled);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
