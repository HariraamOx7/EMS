#include "teacher.h"
#include "admin.h"
#include "ui_teacher.h"
#include <ui_admin.h>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
Teacher::Teacher(Admin *adminwindow, QWidget *parent)
    : User(parent),admin(adminwindow)
    , ui(new Ui::Teacher)
{
    ui->setupUi(this);

}


bool Teacher::authenticate(const QString& username, const QString& password) {
    QFile file("C:/Users/HARIRAAM/Desktop/EMS/user/teacher.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        QStringList parts = line.split(',');
        if (parts.size() != 3) continue;

        if (parts[1] == username && parts[2] == password) {
            return true;
        }
    }
    return false;
}



void Teacher::on_managestudent_clicked()
{
    admin->getStack()->setCurrentWidget(admin->getManageStudentPage());
    admin->show();
}


void Teacher::on_createexam_clicked()
{
    ui->teacherWidget->setCurrentWidget(ui->examdetails);

}


void Teacher::on_createexam_2_clicked()
{
    QString id = ui->examid->text();
    QString name = ui->examname->text();
    int duration=ui->duration->value();
    QString type=ui->examtype->currentText();
    QFile file("C:/Users/HARIRAAM/Desktop/EMS/exam/examdetails.txt");
    examid=id;
    if (name.isEmpty() || id.isEmpty()) {
        QMessageBox::warning(this, "Missing Info", "Please fill all fields.");
        return;
    }
    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "Cannot open exams.txt");
        return;
    }
    QTextStream out(&file);

    out << id << "|" << name << "|" << type << "|" << duration << "\n";

    file.close();
    QString question = "C:/Users/HARIRAAM/Desktop/EMS/exam/" + id + ".txt";
    QFile questionFile((question));
    if (!questionFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "Cannot create exam file");
        return;
    }
    QTextStream questionOut(&questionFile);
    questionOut << "ExamID:" << id << "\n";
    questionFile.close();

    QMessageBox::information(this, "Success", "Exam created and saved!");
    if (type.trimmed().toLower() == "objective") {
        ui->teacherWidget->setCurrentWidget(ui->setquestionforobjective);
    } else if (type.trimmed().toLower() == "subjective") {
        ui->teacherWidget->setCurrentWidget(ui->setquestionforsubjective);
    }
}



void Teacher::on_addquestionbtnforo_clicked()
{
    QString question = ui->question->toPlainText();
    QString opt1 = ui->option1->text().trimmed();
    QString opt2 = ui->option2->text().trimmed();
    QString opt3 = ui->option3->text().trimmed();
    QString opt4 = ui->option4->text().trimmed();
   QString selectedOption = ui->correctanswerbtn->currentText().trimmed();  // Assuming it's a QSpinBox

    if (question.isEmpty() || opt1.isEmpty() || opt2.isEmpty() || opt3.isEmpty() || opt4.isEmpty()) {
        QMessageBox::warning(this, "Incomplete", "Please fill all fields.");
        return;
    }

    QString filename = "C:/Users/HARIRAAM/Desktop/EMS/exam/" + examid + ".txt";  // Make sure currentExamID is set when exam is created

    QFile file(filename);
    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "Unable to open exam file.");
        return;
    }

    QTextStream out(&file);
    out << "Q::" << question << "::" << opt1 << "::" << opt2 << "::" << opt3 << "::" << opt4 << "::" << selectedOption << "\n";
    file.close();

    // Clear the fields
    ui->question->clear();
    ui->option1->clear();
    ui->option2->clear();
    ui->option3->clear();
    ui->option4->clear();
    ui->correctanswerbtn->setCurrentIndex(1);

    QMessageBox::information(this, "Added", "Question added successfully!");


}


void Teacher::on_addquestionbtnfors_clicked()
{
    QString question = ui->questionfors->toPlainText();
    if(question.isEmpty()){
        QMessageBox::warning(this,"Incomplete", "Please fill all fields.");
        return;
    }
      QString filename = "C:/Users/HARIRAAM/Desktop/EMS/exam/" + examid + ".txt";
    QFile file(filename);
    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "Unable to open exam file.");
        return;
    }
     QTextStream out(&file);
    out<<"Q::"<<question;
     file.close();
      ui->questionfors->clear();
    QMessageBox::information(this, "Added", "Question added successfully!");

}


void Teacher::on_savebtn_clicked()
{
    QMessageBox::information(this,"Success","Exam has been saved successfully");
     ui->teacherWidget->setCurrentWidget(ui->examdetails);

}


void Teacher::on_examdetailsbackbtn_clicked()
{
     ui->teacherWidget->setCurrentWidget(ui->teacherdashboard);
}


void Teacher::on_savebtn_2_clicked()
{
    QMessageBox::information(this,"Success","Exam has been saved successfully");
    ui->teacherWidget->setCurrentWidget(ui->examdetails);

}

