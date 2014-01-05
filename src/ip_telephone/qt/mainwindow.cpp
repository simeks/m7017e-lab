#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _client(new Client(this)),
    _call_dialog(NULL),
	_incoming_call_dialog(NULL)
{
    ui->setupUi(this);

    _call_dialog = new Call_Dialog(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    _call_dialog->open();
}

void MainWindow::ShowIncomingCallDialog()
{
	_incoming_call_dialog->open();
}