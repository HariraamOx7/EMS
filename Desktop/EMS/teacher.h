#ifndef TEACHER_H
#define TEACHER_H
#include "admin.h"
#include <QWidget>
#include "User.h"
namespace Ui {
class Teacher;
}

class Teacher : public User
{
    Q_OBJECT

public:

    explicit Teacher(Admin *adminwindow,QWidget* parent = nullptr);
     bool authenticate(const QString& username, const QString& password) override;


private slots:
    void on_managestudent_clicked();

    void on_createexam_clicked();

    void on_createexam_2_clicked();



    void on_addquestionbtnforo_clicked();

    void on_addquestionbtnfors_clicked();

    void on_savebtn_clicked();

    void on_examdetailsbackbtn_clicked();

    void on_savebtn_2_clicked();

private:
    Ui::Teacher *ui;
    Admin *admin;
    QString examid;
};

#endif // TEACHER_H
