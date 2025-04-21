#include "student.h"
#include "ui_student.h"
#include <QMessageBox>
#include <QFile>
Student::Student(Admin *adminwindow, QWidget *parent)
    : User(parent),admin(adminwindow)
    , ui(new Ui::Student)
{
    ui->setupUi(this);

}



bool Student::authenticate(const QString& username, const QString& password) {
    QFile file("C:/Users/HARIRAAM/Desktop/EMS/user/student.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        QStringList parts = line.split(',');
        if (parts.size() != 3) continue;

        if (parts[1] == username && parts[2] == password) {
            setCurrentStudentId(parts[0]);
            return true;
        }
    }
    return false;
}


void Student::on_viewexambtn_clicked(){

    ui->studentstackedwidget->setCurrentWidget(ui->viewexam);
    QFile file("C:/Users/HARIRAAM/Desktop/EMS/exam/examdetails.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Unable to open exam details file.");
        return;
    }

    QTextStream in(&file);
    ui->examdetails->setRowCount(0);
    int row = 0;

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        QStringList parts = line.split('|');
        if (parts.size() != 4) continue;

        ui->examdetails->insertRow(row);

        for (int i = 0; i < 4; ++i) {
            QTableWidgetItem *item = new QTableWidgetItem(parts[i]);
            item->setFlags(item->flags() ^ Qt::ItemIsEditable);
            ui->examdetails->setItem(row, i, item);
        }

        // Add "Take Exam" button
        QPushButton* takeBtn = new QPushButton("Take");
        connect(takeBtn, &QPushButton::clicked, this, [=]() {
            checkandstartexam(parts[0]); // parts[0] = exam ID
        });
        ui->examdetails->setCellWidget(row, 4, takeBtn);

        row++;
    }

    file.close();

}
void Student::checkandstartexam(const QString &examid){
    QString studentId = this->getCurrentStudentId(); // Store logged-in student ID here

    QFile file("C:/Users/HARIRAAM/Desktop/EMS/exam/examdetails.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "Cannot open examdetails.txt");
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        QStringList parts = line.split('|');
        if (parts.size() < 5 || parts[0] != examid) continue;

        if (parts[4].split(',', Qt::SkipEmptyParts).contains(studentId)) {
            QMessageBox::information(this, "Already Attempted", "You have already attempted this exam.");
        } else {
            continue;// Proceed to exam
        }
        return;
    }

    QMessageBox::warning(this, "Not Found", "Exam ID not found.");

}

void Student::setCurrentStudentId(const QString &id) {
    currentStudentId = id;
}

QString Student::getCurrentStudentId() {
    return currentStudentId;
}
