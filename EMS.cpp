#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm> 
using namespace std;


vector<string> splitLine(const string& line, char delimiter = '|') {
    vector<string> tokens;
    stringstream ss(line);
    string token;
    
    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    
    return tokens;
}

string getField(const string& line, int fieldIndex, char delimiter = '|') {
    vector<string> fields = splitLine(line, delimiter);
    
    if (fieldIndex >= 0 && fieldIndex < static_cast<int>(fields.size())) {
        return fields[fieldIndex];
    }
    return "";
}


string encryptPassword(const string& input) {
    string result = input;
    
    for (int i = 0; i < result.length(); i++) {
        result[i] = result[i] + 1; 
    }
    
    return result;
}

// Decrypt by shifting back
string decryptPassword(const string& input) {
    string result = input;
    
    for (int i = 0; i < result.length(); i++) {
        result[i] = result[i] - 1; 
    }
    
    return result;
}

string encryptQuestion(const string& input) {
    string result = input;
    reverse(result.begin(), result.end());
    return result;
}

string decryptQuestion(const string& input) {
    string result = input;
    reverse(result.begin(), result.end());
    return result;
}

bool isIdExists(const string& file, const string& id) {
    ifstream fin(file);
    if (!fin.is_open()) {
        return false; 
    }
    
    string line;
    while (getline(fin, line)) {
        string fileId = getField(line, 0);
        if (fileId == id) {
            return true;
        }
    }
    return false;
}



// ------------------- User Base Class -----------------
class User {
private:
    string id;       
    string username; 
    string password;
    
protected:
    bool checkCredentials(const string& file, const string& id, const string& password) {
        ifstream fin(file);
        if (!fin.is_open()) return false;
        
        string line;
        while (getline(fin, line)) {
            vector<string> fields = splitLine(line);
            if (fields.size() >= 3 && fields[0] == id && fields[2] == encryptPassword(password)) {
                return true;
            }
        }
        return false;
    }
    
    bool addUser(const string& file, const string& id, const string& username, const string& password) {
        if (isIdExists(file, id)) {
            cout << "Error: User ID already exists.\n";
            return false;
        }
       
        
        ofstream fout(file, ios::app);
        if (!fout.is_open()) {
            cout << "Error: Could not open file for writing: " << file << endl;
            return false;
        }
        
        // Encrypt the password 
        string encryptedPass = encryptPassword(password);
        
        fout << id << "|" << username << "|" << encryptedPass << "\n";
        cout << "User added successfully. ID: " << id << ", Username: " << username << endl;
        return true;
    }
    
    void displayUsers(const string& file) {
        ifstream fin(file);
        if (!fin.is_open()) {
            cout << "Error: Could not open file for reading: " << file << endl;
            return;
        }
        
        cout << "\n--- User List ---\n";
        cout << "ID | Username | Password (Encrypted)\n";
        cout << "-----------------------------------\n";
        
        string line;
        while (getline(fin, line)) {
            cout << line << "\n";
        }
        cout << "-----------------------------------\n";
    }
    
    void deleteUser(const string& file, const string& idToDelete) {
        ifstream inFile(file);
        if (!inFile) {
            cout << "Unable to open file: " << file << endl;
            return;
        }

        ofstream tempFile("temp.txt");
        if (!tempFile) {
            cout << "Unable to create temp file" << endl;
            return;
        }

        string line;
        bool userFound = false;

        while (getline(inFile, line)) {
            string userId = getField(line, 0);
            if (userId != idToDelete) {
                tempFile << line << '\n';
            } else {
                userFound = true;
            }
        }

        inFile.close();
        tempFile.close();

        if (!userFound) {
            remove("temp.txt");
            cout << "User ID not found.\n";
            return;
        }

        remove(file.c_str());
        rename("temp.txt", file.c_str());
        cout << "User deleted successfully.\n";
    }
    
public:
    User(string i, string u, string p) : id(i), username(u), password(p) {}
    
    string getId() const { return id; }
    string getUsername() const { return username; }
    
    virtual void dashboard() = 0;
    
    virtual ~User() {}
    
    static bool validateLogin(const string& file, const string& username, const string& password) {
        ifstream fin(file);
        if (!fin.is_open()) return false;
        
        string line;
        while (getline(fin, line)) {
            vector<string> fields = splitLine(line);
            
            if (fields.size() >= 3 && fields[1] == username) {
                // Decrypt stored password and compare with input
                string decryptedStored = decryptPassword(fields[2]);
                if (decryptedStored == password) {
                    return true;
                }
                return false; // Username found but password doesn't match
            }
        }
        return false;
    }
    
    static string getIdByUsername(const string& file, const string& username) {
        ifstream fin(file);
        if (!fin.is_open()) return "";
        
        string line;
        while (getline(fin, line)) {
            vector<string> fields = splitLine(line);
            if (fields.size() >= 2 && fields[1] == username) {
                return fields[0]; // Return the ID field
            }
        }
        return "";
    }
    
    static string getUsernameById(const string& file, const string& id) {
        ifstream fin(file);
        if (!fin.is_open()) return "";
        
        string line;
        while (getline(fin, line)) {
            vector<string> fields = splitLine(line);
            if (fields.size() >= 2 && fields[0] == id) {
                return fields[1]; // Return the username field
            }
        }
        return "";
    }
};

// ------------------- Admin ------------------
class Admin : public User {
public:
    Admin(string i, string u, string p) : User(i, u, p) {}

    void dashboard() override {
        int choice;
        while (true) {
            cout << "\n--- Admin Menu ---\n";
            cout << "Current User's Login: HariraamOx7\n";
            cout << "Logged in as: " << getUsername() << " (ID: " << getId() << ")\n";
            cout << "1. Add Student\n2. List Students\n3. Delete student\n";
            cout << "4. Add Teacher\n5. List Teachers\n6. Delete teacher\n";
            cout << "7. Logout\nChoice: ";
            cin >> choice;

            string id, username, password;
            switch (choice) {
                case 1:
                    cout << "Enter student ID (must be unique): ";
                    cin >> id;
                    // Check if the ID exists in students file
                    if (isIdExists("students.txt", id)) {
                        cout << "Error: Student ID already exists.\n";
                    }  else {
                        cout<<"Enter the student username : ";
                        cin>>username;
                        cout << "Enter student password : ";
                        cin >> password;
                        addUser("students.txt", id, username, password);
                    }
                    break;
                case 2:
                    displayUsers("students.txt");
                    break;
                case 3:
                    cout << "Enter student ID to delete: ";
                    cin >> id;
                    deleteUser("students.txt", id);
                    break;
                case 4:
                    cout << "Enter teacher ID (must be unique): ";
                    cin >> id;
                    
                    // Check if the ID exists in teachers file
                    if (isIdExists("teachers.txt", id)) {
                        cout << "Error: Teacher ID already exists.\n";
                    } else {
                        cout << "Enter teacher username : ";
                        cin >> username;
                        cout << "Enter teacher password : ";
                        cin >> password;
                        addUser("teachers.txt", id, username, password);
                    }
                    break;
                case 5:
                    displayUsers("teachers.txt");
                    break;
                case 6:
                    cout << "Enter teacher ID to delete: ";
                    cin >> id;
                    deleteUser("teachers.txt", id);
                    break;
                case 7:
                    return;
                default:
                    cout << "Invalid option.\n";
            }
        }
    }
};

// ------------------- Exam Interface ------------------
class ExamInterface {
public:
    virtual void createSubjectiveExam(const string& examID, int numQuestions) = 0;
    virtual void createObjectiveExam(const string& examID, int numQuestions) = 0;
    virtual ~ExamInterface() {}
};

// ------------------- Teacher ------------------
class Teacher : public User, public ExamInterface {
private:
    const string exams_dir = "D://EMS//exam//";
    const string exam_details_file = "D://EMS//exam//examdetails.txt";
    
    void saveExamDetails(const string& examID, const string& subject, 
                        const string& examType, int numQuestions) {
        
        ofstream fout(exam_details_file, ios::app);
        if (!fout) {
            cout << "Failed to open examdetails.txt\n";
            return;
        }
        
        fout << examID << "|" << subject << "|" << examType << "|" 
             << numQuestions << "|" << getId() << "|" << getUsername() << "\n";
        fout.close();
    }

public:
    Teacher(string i, string u, string p) : User(i, u, p) {}
    
    void dashboard() override {
        int choice;
        while (true) {
            cout << "\n--- Teacher Menu ---\n";
    
            cout << "Logged in as: " << getUsername() << " (ID: " << getId() << ")\n";
            cout << "1. Create Exam\n2. List Exam\n3. Logout\nChoice: ";
            cin >> choice;

            if (choice == 1) {
                createExam();
            } else if (choice == 2) {
                listExams();
            } else if (choice == 3) {
                cout << "Logging out...\n";
                break;
            } else {
                cout << "Invalid option.\n";
            }
        }
    }
    
    void createExam() {
        cin.ignore(); // Fix leftover newline before getline

        string examID, subject, examType;
        int numQuestions;

        cout << "Enter Exam ID (must be unique): ";
        getline(cin, examID);
        
        // Check if exam ID already exists
        if (isIdExists(exam_details_file, examID)) {
            cout << "Error: Exam ID already exists.\n";
            return;
        }
        
        cout << "Enter Subject Name: ";
        getline(cin, subject);

        cout << "Enter Exam Type (objective/subjective): ";
        getline(cin, examType);

        cout << "Enter Number of Questions: ";
        cin >> numQuestions;
        cin.ignore(); // Again, ignore newline before reading questions

        saveExamDetails(examID, subject, examType, numQuestions);

        if (examType == "objective") {
            createObjectiveExam(examID, numQuestions);
        } else if (examType == "subjective") {
            createSubjectiveExam(examID, numQuestions);
        } else {
            cout << "Invalid exam type.\n";
        }
    }

    void createSubjectiveExam(const string& examID, int numQuestions) override {
        string examfile = exams_dir + examID + ".txt";
        ofstream fout(examfile, ios::out);
        if (!fout) {
            cout << "Error opening exam file: " << examfile << "\n";
            return;
        }

        // Add metadata
        fout << "Exam ID: " << examID << "\n";
        fout << "Created by: " << getUsername() << " (ID: " << getId() << ")\n";
        fout << "Type: Subjective\n";
        fout << "Number of Questions: " << numQuestions << "\n";
        fout << "-------------------------\n\n";

        for (int i = 1; i <= numQuestions; ++i) {
            string question;
            cout << "Question " << i << ": ";
            getline(cin, question);
            
            // Encrypt the question
            string encryptedQuestion = encryptQuestion(question);
            fout << "Q" << i << ": " << encryptedQuestion << "\n";
        }

        fout << "-------------------------\n";
        fout.close();
        cout << "Subjective exam created successfully!\n";
    }

    void createObjectiveExam(const string& examID, int numQuestions) override {
        string examfile = exams_dir + examID + ".txt";
        ofstream fout(examfile, ios::out);
        if (!fout) {
            cout << "Error opening exam file: " << examfile << "\n";
            return;
        }

        // Add metadata
        fout << "Exam ID: " << examID << "\n";
        fout << "Created by: " << getUsername() << " (ID: " << getId() << ")\n";
        fout << "Type: Objective\n";
        fout << "Number of Questions: " << numQuestions << "\n";
        fout << "-------------------------\n\n";

        for (int i = 1; i <= numQuestions; ++i) {
            string question, optA, optB, optC, optD, correct;

            cout << "Question " << i << ": ";
            getline(cin, question);
            cout << "Option A: ";
            getline(cin, optA);
            cout << "Option B: ";
            getline(cin, optB);
            cout << "Option C: ";
            getline(cin, optC);
            cout << "Option D: ";
            getline(cin, optD);
            cout << "Correct Option (A/B/C/D): ";
            getline(cin, correct);

            // Encrypt the question and options
            string encryptedQuestion = encryptQuestion(question);
            string encryptedOptA = encryptQuestion(optA);
            string encryptedOptB = encryptQuestion(optB);
            string encryptedOptC = encryptQuestion(optC);
            string encryptedOptD = encryptQuestion(optD);

            fout << "Q" << i << ": " << encryptedQuestion << "\n";
            fout << "A) " << encryptedOptA << "\n";
            fout << "B) " << encryptedOptB << "\n";
            fout << "C) " << encryptedOptC << "\n";
            fout << "D) " << encryptedOptD << "\n";
            fout << "Answer: " << correct << "\n\n";
        }

        fout << "-------------------------\n";
        fout.close();
        cout << "Objective exam created successfully!\n";
    }
    
    void listExams() {
        ifstream fin(exam_details_file);
        if (!fin) {
            cout << "Could not open examdetails.txt\n";
            return;
        }
        
        string line;
        cout << "\n--- All Exams ---\n";
        cout << "ID | Subject | Type | Questions | Teacher ID | Teacher Name\n";
        cout << "--------------------------------------------------------\n";
        
        while (getline(fin, line)) {
            cout << line << "\n";
        }
        cout << "--------------------------------------------------------\n";
        fin.close();
    }
};

// ------------------- Student ------------------
class Student : public User {
private:
    const string exams_dir = "D://EMS//exam//";
    const string exam_details_file = "D://EMS//exam//examdetails.txt";
    const string submissions_dir = "D://EMS//submissions//";

    string getExamType(const string& examID) {
        ifstream fin(exam_details_file);
        if (!fin) {
            cout << "Could not open examdetails.txt\n";
            return "";
        }

        string line;
        while (getline(fin, line)) {
            vector<string> fields = splitLine(line);
            if (!fields.empty() && fields[0] == examID && fields.size() > 2) {
                fin.close();
                return fields[2]; // Type is the 3rd field (index 2)
            }
        }
        fin.close();
        return "";
    }

public:
    Student(string i, string u, string p) : User(i, u, p) {}

    void dashboard() override {
        int choice;
        while (true) {
            cout << "\n--- Student Menu ---\n";
            cout << "Logged in as: " << getUsername() << " (ID: " << getId() << ")\n";
            cout << "1. List Available Exams\n2. Take Exam\n3. Logout\nChoice: ";
            cin >> choice;
            cin.ignore();

            if (choice == 1) {
                listAvailableExams();
            } else if (choice == 2) {
                takeExam();
            } else if (choice == 3) {
                cout << "Logging out...\n";
                break;
            } else {
                cout << "Invalid option.\n";
            }
        }
    }

    void listAvailableExams() {
        ifstream fin(exam_details_file);
        if (!fin) {
            cout << "Could not open examdetails.txt\n";
            return;
        }

        string line;
        cout << "\n--- Available Exams ---\n";
        cout << "ID | Subject | Type | Questions | Teacher ID | Teacher Name\n";
        cout << "--------------------------------------------------------\n";
        
        while (getline(fin, line)) {
            cout << line << "\n";
        }
        cout << "--------------------------------------------------------\n";
        fin.close();
    }

    void takeExam() {
        string examID;
        cout << "Enter Exam ID to take: ";
        getline(cin, examID);

        // Check if the exam exists
        if (!isIdExists(exam_details_file, examID)) {
            cout << "Error: Exam ID not found.\n";
            return;
        }

        string examType = getExamType(examID);
        if (examType.empty()) {
            cout << "Error retrieving exam type.\n";
            return;
        }

        // Check if student has already taken this exam
        string submissionFile = submissions_dir + getId() + "_" + examID + ".txt";
        ifstream checkSubmission(submissionFile);
        if (checkSubmission.good()) {
            checkSubmission.close();
            char retake;
            cout << "You have already taken this exam. Do you want to retake it? (y/n): ";
            cin >> retake;
            cin.ignore();
            if (retake != 'y' && retake != 'Y') {
                return;
            }
        }

        if (examType == "objective") {
            takeObjectiveExam(examID);
        } else if (examType == "subjective") {
            takeSubjectiveExam(examID);
        } else {
            cout << "Invalid exam type format in details.\n";
        }
    }

    void takeObjectiveExam(const string& examID) {
        string examFile = exams_dir + examID + ".txt";
        ifstream fin(examFile);
        if (!fin) {
            cout << "Exam file not found.\n";
            return;
        }

        string submissionFile = submissions_dir + getId() + "_" + examID + ".txt";
        ofstream fout(submissionFile);
        if (!fout) {
            cout << "Unable to write submission.\n";
            return;
        }

        // Add submission metadata
        fout << "Student ID: " << getId() << "\n";
        fout << "Student Name: " << getUsername() << "\n";
        fout << "Exam ID: " << examID << "\n";
        fout << "-------------------------\n\n";

        string line;
        cout << "\n--- Begin Objective Exam ---\n";
        
        // Skip metadata lines
        for (int i = 0; i < 6; i++) {
            getline(fin, line);
        }
        
        while (getline(fin, line)) {
            if (line.find("Q") == 0) {
                // Extract and decrypt question
                string qNum = line.substr(0, line.find(": ") + 2); // "Q1: "
                string encryptedQuestion = line.substr(line.find(": ") + 2);
                string question = decryptQuestion(encryptedQuestion);
                
                cout << qNum << question << "\n";
                fout << line << "\n"; // Store encrypted version in submission

                string opt, optText;
                for (int i = 0; i < 4; i++) {
                    getline(fin, opt);
                    
                    // Extract and decrypt option
                    string optPrefix = opt.substr(0, 3); // "A) ", "B) ", etc.
                    string encryptedOpt = opt.substr(3);
                    optText = decryptQuestion(encryptedOpt);
                    
                    cout << optPrefix << optText << "\n";
                    fout << opt << "\n"; // Store encrypted version in submission
                }

                string correct, answer;
                getline(fin, correct);
                fout << correct << "\n";

                cout << "Your Answer (A/B/C/D): ";
                getline(cin, answer);
                fout << "Student Answer: " << answer << "\n\n";
            }
        }

        fin.close();
        fout.close();
        cout << "Objective exam submitted successfully!\n";
    }

    void takeSubjectiveExam(const string& examID) {
        string examFile = exams_dir + examID + ".txt";
        ifstream fin(examFile);
        if (!fin) {
            cout << "Exam file not found.\n";
            return;
        }

        string submissionFile = submissions_dir + getId() + "_" + examID + ".txt";
        ofstream fout(submissionFile);
        if (!fout) {
            cout << "Unable to write submission.\n";
            return;
        }

        // Add submission metadata
        fout << "Student ID: " << getId() << "\n";
        fout << "Student Name: " << getUsername() << "\n";
        fout << "Exam ID: " << examID << "\n";
        fout << "-------------------------\n\n";

        string line;
        cout << "\n--- Begin Subjective Exam ---\n";
        
        // Skip metadata lines
        for (int i = 0; i < 6; i++) {
            getline(fin, line);
        }

        while (getline(fin, line)) {
            if (line.find("Q") == 0) {
                // Extract and decrypt question
                string qNum = line.substr(0, line.find(": ") + 2); // "Q1: "
                string encryptedQuestion = line.substr(line.find(": ") + 2);
                string question = decryptQuestion(encryptedQuestion);
                
                cout << qNum << question << "\n";
                fout << line << "\n"; // Store encrypted version in submission

                string answer;
                cout << "Your Answer: ";
                getline(cin, answer);
                fout << "Student Answer: " << answer << "\n\n";
            }
        }

        fin.close();
        fout.close();
        cout << "Subjective exam submitted successfully!\n";
    }
};

// ------------------- Login System ------------------
class LoginSystem {
public:
    static void start() {
        string username, password;
        int role;
        cout << "\nSelect Role:\n1. Admin\n2. Teacher\n3. Student\nChoice: ";
        cin >> role;
        cout << "Username: "; cin >> username;
        cout << "Password: "; cin >> password;

        if (role == 1) {
            if (username == "admin" && password == "admin123") {
                Admin admin("admin", "Administrator", password);
                admin.dashboard();
            } else {
                cout << "Login failed: Invalid username or password." << endl;
            }
        } else if (role == 2) {
            if (User::validateLogin("teachers.txt", username, password)) {
                string id = User::getIdByUsername("teachers.txt", username);
                Teacher teacher(id, username, password);
                teacher.dashboard();
            } else {
                cout << "Login failed: Invalid username or password." << endl;
            }
        } else if (role == 3) {
            if (User::validateLogin("students.txt", username, password)) {
                string id = User::getIdByUsername("students.txt", username);
                Student student(id, username, password);
                student.dashboard();
            } else {
                cout << "Login failed: Invalid username or password." << endl;
            }
        } else {
            cout << "Invalid role selected.\n";
        }
    }
};

// ------------------- Main ------------------
int main() {
    while (true) {
        cout << "\n===== Online Exam System =====\n";
        LoginSystem::start();
        char again;
        cout << "Do you want to continue? (y/n): ";
        cin >> again;
        if (again != 'y' && again != 'Y') break;
    }
    return 0;
}
