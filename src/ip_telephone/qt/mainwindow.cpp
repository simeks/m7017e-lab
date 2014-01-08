#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _client(new Client(this)),
    _settings_dialog(NULL)
{
    ui->setupUi(this);

    // Hide the Calling panel
    ui->widget->hide();

    // Hide the Incoming-call panel
    ui->widget_2->hide();

    // Hide the Active-call panel
    ui->widget_3->hide();

    _settings_dialog = new Settings_Dialog(_client, this);

    ui->lineEdit->setText("sip:m7017elab3@iptel.org");

    // Open the settings dialog when the application starts
    _settings_dialog->open();

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
    ui->widget->show();

	// Hide Call button and sip-address textfield
    ui->widget_4->hide();
    ui->widget_5->hide();
}

void MainWindow::ShowIncomingCallPanel()
{
    ui->widget_2->show();
}

// Callback for when the Stop button is clicked
void MainWindow::on_pushButton_2_clicked()
{
    _client->InterruptCall();
}


// Callback for when the Answer button is clicked
void MainWindow::on_pushButton_3_clicked()
{
    _client->AnswerIncomingCall();
}

// Callback for when the Decline button is clicked
void MainWindow::on_pushButton_4_clicked()
{
    _client->DeclineCall();
}

// Callback for when the Hang-Up button is clicked
void MainWindow::on_pushButton_5_clicked()
{
    _client->HangUpActiveCall();
}

void MainWindow::HideIncomingCallPanel()
{
    ui->widget_2->hide();
}

void MainWindow::HideActiveCallPanel()
{
	ui->widget_3->hide();
}

void MainWindow::HideCallingPanel()
{
	ui->widget->hide();
}

void MainWindow::ShowActiveCallPanel()
{
	ui->widget_3->show();
}

void MainWindow::ShowMainWindow()
{
	ui->widget_4->show();
    ui->widget_5->show();
}