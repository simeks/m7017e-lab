#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../client.h"
#include "settings_dialog.h"

namespace Ui {
class MainWindow;
}

class Client;
class Settings_Dialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

	/// @brief Show the Incoming-call panel
	void ShowIncomingCallPanel(std::string incoming_uri);

	/// @brief Hide the Active-call panel
	void HideActiveCallPanel();

	/// @brief Hide the Incoming-call panel
	void HideIncomingCallPanel();

	/// @brief Hide the Calling panel
	void HideCallingPanel();

	/// @brief Show the Active-call panel
	void ShowActiveCallPanel();

	/// @brief Show the Call-button and sip-address textfield
	void ShowMainWindow();

private slots:

	/// @brief Callback for when the Call button is clicked
    void on_pushButton_clicked();

	/// @brief Callback for when the Stop button is clicked
    void on_pushButton_2_clicked();

	/// @brief Callback for when the Answer button is clicked
    void on_pushButton_3_clicked();

	/// @brief Callback for when the Decline button is clicked
    void on_pushButton_4_clicked();

	/// @brief Callback for when the Hang-Up button is clicked
    void on_pushButton_5_clicked();

private:
    Ui::MainWindow *ui;

    Client* _client;
    Settings_Dialog* _settings_dialog;
};

#endif // MAINWINDOW_H
