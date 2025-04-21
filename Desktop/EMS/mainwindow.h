#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include<ui_admin.h>
#include <QMainWindow>
#include "admin.h"
#include "student.h"
#include "teacher.h"
#include <QStackedWidget>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString selectedRole;

private slots:
    void on_admin_btn_clicked();

    void on_teacher_btn_clicked();

    void on_student_btn_clicked();

    void on_login_btn_clicked();

private:
    Ui::MainWindow *ui;
    QStackedWidget *stackedWidget;
    Admin *adminUI;
    Teacher *teacherUI;
    Student *studentUI;
};
#endif // MAINWINDOW_H
