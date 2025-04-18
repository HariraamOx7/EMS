#include "mainwindow.h"
#include "admin.h"
#include <QMessageBox>
#include "ui_mainwindow.h"
#include<QWidget>
#include <ui_admin.h>
#include <QStackedWidget>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    adminUI = new Admin(this);  // Create adminUI instance
    ui->stackedWidget->addWidget(adminUI);
    ui->stackedWidget->setCurrentWidget(ui->main);

}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_admin_btn_clicked()
{
    selectedRole = "Admin";
    ui->stackedWidget->setCurrentWidget(ui->login);

}


void MainWindow::on_teacher_btn_clicked()
{
    selectedRole = "Teacher";
    ui->stackedWidget->setCurrentWidget(ui->login);

}


void MainWindow::on_student_btn_clicked()
{
    selectedRole = "Student";
    ui->stackedWidget->setCurrentWidget(ui->login);

}


void MainWindow::on_login_btn_clicked()
{
    QString username = ui->username->text();
    QString password = ui->password->text();

    bool success = false;

    if (selectedRole == "Admin") {
        success = adminUI->authenticate(username, password);
        if (success) {
            qDebug() << "Admin login successful. Switching to admin dashboard.";
            ui->stackedWidget->setCurrentWidget(adminUI);
            adminUI->show();// Resize the Admin widget to ensure it's large enough to be seen

        }
    }

    if (!success) {
        QMessageBox::warning(this, "Login Failed", "Invalid credentials!");
    }

}

