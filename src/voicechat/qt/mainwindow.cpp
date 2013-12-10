#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _connect_dialog(NULL),
    _client(new Client(this))
{
    ui->setupUi(this);

    _connect_dialog = new ConnectDialog(_client, this);

    ui->lineEdit->setDisabled(true);
    ui->treeWidget->hideColumn(0);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_lineEdit_returnPressed()
{

    // skicka meddelandet till servern som sen det vidare till alla klienter?


    QString text = ui->lineEdit->text();

    if (!text.isEmpty())
    {
        _client->sendChatMessage(text);
        //ui->textEdit->append(text);
        ui->lineEdit->clear();
    }
}

void MainWindow::on_muteVolume_toggled(bool toggled)
{

}

void MainWindow::on_muteMic_toggled(bool toggled)
{

}

void MainWindow::on_actionConnect_triggered()
{
    _connect_dialog->open();
}

void MainWindow::Connected()
{
    ui->lineEdit->setEnabled(true);
    ui->treeWidget->showColumn(0);
}
