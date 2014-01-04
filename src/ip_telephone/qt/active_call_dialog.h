#ifndef ACTIVE_CALL_DIALOG_H
#define ACTIVE_CALL_DIALOG_H

#include <QDialog>

namespace Ui {
class Active_Call_Dialog;
}

class Active_Call_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Active_Call_Dialog(QWidget *parent = 0);
    ~Active_Call_Dialog();

private:
    Ui::Active_Call_Dialog *ui;
};

#endif // ACTIVE_CALL_DIALOG_H
