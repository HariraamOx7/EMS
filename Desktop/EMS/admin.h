#ifndef ADMIN_H
#define ADMIN_H

#include <QWidget>
#include "User.h"
namespace Ui {
class Admin;
}

class Admin : public User
{
 Q_OBJECT

public:
    explicit Admin(QWidget *parent = nullptr); // Constructor with parent argument
    ~Admin();
    bool authenticate(const QString& username, const QString& password) ;



private slots:

    void on_mngstudent_clicked();

    void on_mngteacher_clicked();

    void on_mngexam_clicked();

    void on_add_student_clicked();

    void   loadStudentsFromFile();
    void on_add_teacher_clicked();

    void   loadTeachersFromFile();



    void on_studentbackbtn_clicked();

    void on_teacherbackbtn_clicked();

private:
    Ui::Admin *ui;
};

#endif // ADMIN_H
