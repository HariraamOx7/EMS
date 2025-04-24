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
struct Questionforo{
    QString id;
    QString text;
    QStringList options;
    QString crtoption;
};
QVector<Questionforo> question;



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

void Student::on_viewexambtn_clicked() {
    ui->studentstackedwidget->setCurrentWidget(ui->viewexam);

    QFile file("C:/Users/HARIRAAM/Desktop/EMS/exam/examdetails.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Unable to open exam details file.");
        return;
    }

    QTextStream in(&file);
    ui->examdetails->setRowCount(0);
    int row = 0;
    QString studentId = getCurrentStudentId();
    ui->examdetails->setColumnCount(6);
    ui->examdetails->setHorizontalHeaderLabels(QStringList() << "ID" << "Name" << "Type" << "Duration" << "Status" << "Action");

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        QStringList parts = line.split('|');
        qDebug() << "Loaded exam:" << parts;
        examid=parts[0];
        examtype=parts[2];
        if (parts.size() < 4) continue;

        ui->examdetails->insertRow(row);

        // Columns 0 to 3: Exam ID, Name, Type, Duration
        for (int i = 0; i < 4; ++i) {
            QTableWidgetItem *item = new QTableWidgetItem(parts[i]);
            item->setFlags(item->flags() ^ Qt::ItemIsEditable);
            ui->examdetails->setItem(row, i, item);
        }

        // Column 4: Status (Attempted / Not Attempted)
        QString status = "Not Attempted";
        if (parts.size() >= 5) {
            QStringList attemptedList = parts[4].split(',', Qt::SkipEmptyParts);
            if (attemptedList.contains(studentId)) {
                status = "Attempted";
            }
        }
        QTableWidgetItem *statusItem = new QTableWidgetItem(status);
        statusItem->setFlags(statusItem->flags() ^ Qt::ItemIsEditable);
        ui->examdetails->setItem(row, 4, statusItem);

        // Column 5: "Take Exam" button
        QPushButton* takeBtn = new QPushButton("Take");
        connect(takeBtn, &QPushButton::clicked, this, [=]() {
            checkandstartexam(parts[0]); // Exam ID
        });
        ui->examdetails->setCellWidget(row, 5, takeBtn);

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

            if(examtype=="objective") {
                ui->studentstackedwidget->setCurrentWidget(ui->examwindowforo);
                loadexamquestionforo();// Proceed to exam
            }
            else if(examtype=="subjective"){
                continue;
            }
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
void Student::loadexamquestionforo(){

}
