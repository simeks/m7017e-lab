#ifndef CALL_DIALOG_H
#define CALL_DIALOG_H

#include <QDialog>

namespace Ui {
class Call_Dialog;
}

class Call_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Call_Dialog(QWidget *parent = 0);
    ~Call_Dialog();

private slots:

    void on_pushButton_clicked();

private:
    Ui::Call_Dialog *ui;
};

#endif // CALL_DIALOG_H
