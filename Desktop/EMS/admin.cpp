#include "admin.h"
#include "ui_admin.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

Admin::Admin(QWidget *parent)
    : User(parent), ui(new Ui::Admin)
{
    ui->setupUi(this);
}

Admin::~Admin() {
    delete ui;
}



bool Admin::authenticate(const QString& username, const QString& password) {
    QFile file("C:/Users/HARIRAAM/Desktop/EMS/user/admin.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        QStringList parts = line.split(',');
        if (parts.size() != 2) continue;

        if (parts[0] == username && parts[1] == password) {
            return true;
        }
    }
    return false;
}



void Admin::on_mngstudent_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->manage_student);
    ui->studentdetailstable->setRowCount(0);
    loadStudentsFromFile();

}


void Admin::on_mngteacher_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->manage_teacher);
    ui->teacherdetailstable->setRowCount(0);
    loadTeachersFromFile();
}


void Admin::on_mngexam_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->manage_exam);
}


void Admin::on_add_student_clicked()
{
    QString id = ui->inp_id->text().trimmed();
    QString name = ui->inp_name->text().trimmed();
    QString password = ui->inp_password->text().trimmed();

    try {
        if (id.isEmpty() || name.isEmpty() || password.isEmpty()) {
            throw std::runtime_error("All fields must be filled.");
        }

        // Open file
        QFile file("C:/Users/HARIRAAM/Desktop/EMS/user/student.txt");
        if (!file.open(QIODevice::Append | QIODevice::Text)) {
            throw std::runtime_error("Failed to open file for writing.");
        }

        QTextStream out(&file);
        out << id << "," << name << "," << password << "\n";
        file.close();

        // Update table
        int row = ui->studentdetailstable->rowCount();
        ui->studentdetailstable->insertRow(row);
        ui->studentdetailstable->setItem(row, 0, new QTableWidgetItem(id));
        ui->studentdetailstable->setItem(row, 1, new QTableWidgetItem(name));
        ui->studentdetailstable->setItem(row, 2, new QTableWidgetItem(password));

        ui->inp_id->clear();
        ui->inp_name->clear();
        ui->inp_password->clear();
    }
    catch (const std::runtime_error& e) {
        QMessageBox::critical(this, "Error", e.what());
    }

}



void Admin::loadStudentsFromFile() {
    QFile file("C:/Users/HARIRAAM/Desktop/EMS/user/student.txt");

    try {
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            throw std::runtime_error("Failed to open file for reading.");
        }

        QTextStream in(&file);
        int lineNumber = 0;

        while (!in.atEnd()) {
            lineNumber++;
            QString line = in.readLine();
            QStringList parts = line.split(",");

            if (parts.size() != 3) {
                throw std::runtime_error(("Corrupted data on line " + std::to_string(lineNumber)).c_str());
            }

            int row = ui->studentdetailstable->rowCount();
            ui->studentdetailstable->insertRow(row);
            ui->studentdetailstable->setItem(row, 0, new QTableWidgetItem(parts[0]));
            ui->studentdetailstable->setItem(row, 1, new QTableWidgetItem(parts[1]));
            ui->studentdetailstable->setItem(row, 2, new QTableWidgetItem(parts[2]));
        }

        file.close();
    }
    catch (const std::runtime_error& e) {
        QMessageBox::critical(this, "File Read Error", e.what());
    }
}



void Admin::on_add_teacher_clicked()
{
    QString id_2= ui->inp_id_3->text().trimmed();
    QString name_2 = ui->inp_name_3->text().trimmed();
    QString password_2 = ui->inp_password_3->text().trimmed();

    try {
        if (id_2.isEmpty() || name_2.isEmpty() || password_2.isEmpty()) {
            throw std::runtime_error("All fields must be filled.");
        }

        // Open file
        QFile file("C:/Users/HARIRAAM/Desktop/EMS/user/teacher.txt");
        if (!file.open(QIODevice::Append | QIODevice::Text)) {
            throw std::runtime_error("Failed to open file for writing.");
        }

        QTextStream out(&file);
        out << id_2 << "," << name_2 << "," << password_2 << "\n";
        file.close();

        // Update table
        int row = ui->teacherdetailstable->rowCount();
        ui->teacherdetailstable->insertRow(row);
        ui->teacherdetailstable->setItem(row, 0, new QTableWidgetItem(id_2));
        ui->teacherdetailstable->setItem(row, 1, new QTableWidgetItem(name_2));
        ui->teacherdetailstable->setItem(row, 2, new QTableWidgetItem(password_2));

        ui->inp_id_3->clear();
        ui->inp_name_3->clear();
        ui->inp_password_3->clear();
    }
    catch (const std::runtime_error& e) {
        QMessageBox::critical(this, "Error", e.what());
    }


}
void Admin::loadTeachersFromFile() {
    QFile file("C:/Users/HARIRAAM/Desktop/EMS/user/teacher.txt");

    try {
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            throw std::runtime_error("Failed to open file for reading.");
        }

        QTextStream in(&file);
        int lineNumber = 0;

        while (!in.atEnd()) {
            lineNumber++;
            QString line = in.readLine();
            QStringList parts = line.split(",");

            if (parts.size() != 3) {
                throw std::runtime_error(("Corrupted data on line " + std::to_string(lineNumber)).c_str());
            }

            int row = ui->teacherdetailstable->rowCount();
            ui->teacherdetailstable->insertRow(row);
            ui->teacherdetailstable->setItem(row, 0, new QTableWidgetItem(parts[0]));
            ui->teacherdetailstable->setItem(row, 1, new QTableWidgetItem(parts[1]));
            ui->teacherdetailstable->setItem(row, 2, new QTableWidgetItem(parts[2]));
        }

        file.close();
    }
    catch (const std::runtime_error& e) {
        QMessageBox::critical(this, "File Read Error", e.what());
    }
}




void Admin::on_studentbackbtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void Admin::on_teacherbackbtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void Admin::on_deletestdbtn_clicked()
{
    int row = ui->studentdetailstable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "No Selection", "Please select a student to delete.");
        return;
    }

    QString id = ui->studentdetailstable->item(row, 0)->text();

    // Remove from table
    ui->studentdetailstable->removeRow(row);

    // Remove from file
    deleteStudentFromFile(id);

}
void Admin::deleteStudentFromFile(QString &id){
    QString originalFilePath = "C:/Users/HARIRAAM/Desktop/EMS/user/student.txt";
    QString tempFilePath = "C:/Users/HARIRAAM/Desktop/EMS/user/temp.txt";

    QFile inputFile(originalFilePath);
    QFile tempFile(tempFilePath);

    if (!inputFile.open(QIODevice::ReadOnly | QIODevice::Text) ||
        !tempFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "File Error", "Could not open file for deletion.");
        return;
    }

    QTextStream in(&inputFile);
    QTextStream out(&tempFile);

    while (!in.atEnd()) {
        QString line = in.readLine();
        if (!line.startsWith(id + ",")) {
            out << line << "\n";
        }
    }

    inputFile.close();
    tempFile.close();

    // Delete original file first
    if (!QFile::remove(originalFilePath)) {
        QMessageBox::critical(this, "Error", "Failed to delete original student file.");
        return;
    }

    // Rename temp to original
    if (!QFile::rename(tempFilePath, originalFilePath)) {
        QMessageBox::critical(this, "Error", "Failed to rename temp file to student.txt.");
        return;
    }
}




void Admin::on_teacherdeletebtn_clicked()
{
    int row = ui->teacherdetailstable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "No Selection", "Please select a teacher to delete.");
        return;
    }

    // Get student ID
    QString id = ui->teacherdetailstable->item(row, 0)->text();

    // Remove from table
    ui->teacherdetailstable->removeRow(row);

    // Remove from file
    deleteTeacherFromFile(id);
}

void Admin::deleteTeacherFromFile(QString &id){
    QString originalFilePath = "C:/Users/HARIRAAM/Desktop/EMS/user/teacher.txt";
    QString tempFilePath = "C:/Users/HARIRAAM/Desktop/EMS/user/temp.txt";

    QFile inputFile(originalFilePath);
    QFile tempFile(tempFilePath);

    if (!inputFile.open(QIODevice::ReadOnly | QIODevice::Text) ||
        !tempFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "File Error", "Could not open file for deletion.");
        return;
    }

    QTextStream in(&inputFile);
    QTextStream out(&tempFile);

    while (!in.atEnd()) {
        QString line = in.readLine();
        if (!line.startsWith(id + ",")) {
            out << line << "\n";
        }
    }

    inputFile.close();
    tempFile.close();

    // Delete original file first
    if (!QFile::remove(originalFilePath)) {
        QMessageBox::critical(this, "Error", "Failed to delete original teacher file.");
        return;
    }

    // Rename temp to original
    if (!QFile::rename(tempFilePath, originalFilePath)) {
        QMessageBox::critical(this, "Error", "Failed to rename temp file to teacher txt.");
        return;
    }
}
QWidget* Admin::getManageStudentPage() {
    return ui->manage_student;
}

QStackedWidget* Admin::getStack() {
    return ui->stackedWidget; // the main stacked widget
}

