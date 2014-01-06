#include "settings_dialog.h"
#include "ui_settings_dialog.h"

Settings_Dialog::Settings_Dialog(Client* client, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings_Dialog),
    _client(client)
{
    ui->setupUi(this);

    ui->lineEdit_2->setEchoMode(QLineEdit::Password);

    ui->lineEdit->setText("joohanforsling");
    ui->lineEdit_2->setText("hejhej");
    ui->lineEdit_3->setText("iptel.org");
}

Settings_Dialog::~Settings_Dialog()
{
    delete ui;
}

// Callback for when the Save button is clicked
void Settings_Dialog::on_pushButton_clicked()
{
    // Username
    QString username = ui->lineEdit->text();
    std::string _user_name = username.toStdString();

    // Password
    QString password = ui->lineEdit_2->text();
    std::string _password = password.toStdString();

    // Domain
    QString domain = ui->lineEdit_3->text();
    std::string _domain = domain.toStdString();


    _client->SetUser(_user_name, _password, _domain);

    this->close();
}
