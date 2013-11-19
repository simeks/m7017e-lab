#ifndef PLAYERWINDOW_H
#define PLAYERWINDOW_H

#include <QMainWindow>

namespace Ui {
class PlayerWindow;
}

class PlayerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PlayerWindow(QWidget *parent = 0);
    ~PlayerWindow();

private slots:
    void open();

private:
    Ui::PlayerWindow *ui;
    QStringList fileName;
};

#endif // PLAYERWINDOW_H
