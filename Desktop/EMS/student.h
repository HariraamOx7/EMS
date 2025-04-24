#ifndef STUDENT_H
#define STUDENT_H
#include "admin.h"
#include <QWidget>
#include "User.h"
#include <QWidget>

namespace Ui {
class Student;
}

class Student : public User
{
    Q_OBJECT

public:
    explicit Student(Admin *adminwindow,QWidget* parent = nullptr);

      void  checkandstartexam(const QString &examid);
    void setCurrentStudentId(const QString &id); // To set the ID after login
    QString getCurrentStudentId();
    bool authenticate(const QString& username, const QString& password) override;
    void loadexamquestionforo();
    void loadexamquestionfors();
    QString examid;
    QString examtype;


private slots:
    void on_viewexambtn_clicked();


private:
    Ui::Student *ui;
    Admin *admin;
     QString currentStudentId;
};

#endif // STUDENT_H
