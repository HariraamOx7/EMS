#include "mainwindow.h"
#include "admin.h"
#include <QMessageBox>
#include "student.h"
#include "teacher.h"
#include "ui_mainwindow.h"
#include<QWidget>
#include <ui_admin.h>
#include <QStackedWidget>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("EMS");

    adminUI = new Admin(this);
    ui->stackedWidget->addWidget(adminUI);
    ui->stackedWidget->setCurrentWidget(ui->main);
    Admin *adminWindow = new Admin();
    teacherUI = new Teacher(adminWindow,this);
    ui->stackedWidget->addWidget(teacherUI);
    ui->stackedWidget->setCurrentWidget(ui->main);
    studentUI = new Student(adminWindow,this);
    ui->stackedWidget->addWidget(studentUI);
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
            adminUI->show();

        }
    }
    else if (selectedRole == "Teacher") {
         success = teacherUI->authenticate(username, password);
        if (success) {

            qDebug() << "Teacher login successful. Switching to teacher dashboard.";
             ui->stackedWidget->setCurrentWidget(teacherUI);
             teacherUI->show();

        }
    }    else if (selectedRole == "Student") {
         success = studentUI->authenticate(username, password);
        if (success) {
            qDebug() << "Student login successful. Switching to student dashboard.";
            ui->stackedWidget->setCurrentWidget(studentUI);
            studentUI->show();

        }
    }




    if (!success) {
        QMessageBox::warning(this, "Login Failed", "Invalid credentials!");
    }

}

