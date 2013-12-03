#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_lineEdit_returnPressed()
{
    QString text = ui->lineEdit->text();

    if (!text.isEmpty())
    {
        ui->textEdit->append(text);
        ui->lineEdit->clear();
    }
}

void MainWindow::on_muteVolume_toggled(bool toggled)
{

}

void MainWindow::on_muteMic_toggled(bool toggled)
{

}
