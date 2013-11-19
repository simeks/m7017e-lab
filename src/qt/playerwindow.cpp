#include "playerwindow.h"
#include "ui_playerwindow.h"
#include <QFileDialog>

PlayerWindow::PlayerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PlayerWindow)


{
    ui->setupUi(this);
    connect(ui->actionOpen_File, SIGNAL(triggered()), this, SLOT(open()));
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));
}

PlayerWindow::~PlayerWindow()
{
    delete ui;
}

void PlayerWindow::open()
{
    fileNames = QFileDialog::getOpenFileNames(this, tr("Open Files"));
}
