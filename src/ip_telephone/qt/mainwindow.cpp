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

    _call_dialog = new Call_Dialog(_client, this);
	_incoming_call_dialog = new Incoming_Call_Dialog(_client, this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

// Callback for when the Call button is clicked
void MainWindow::on_pushButton_clicked()
{
	QString uri = ui->lineEdit->text();
	std::string _uri = uri.toStdString(); // convert from Qstring to std::string
	_client->MakeCall(_uri);
    _call_dialog->open();
}

void MainWindow::ShowIncomingCallDialog()
{
	_incoming_call_dialog->open();
}
