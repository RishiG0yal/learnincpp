#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <sstream>
using namespace std;

const string password = "admin123";
const int Max = 150;
const int max_date = 31;
int studentcount = 0;

struct attendance {
    string date;
    bool ispresent;
};

struct Student {
    string name;
    attendance records[max_date];
    int datecount = 0;
};
Student students[Max];

void addstudent(){
    if (studentcount >= Max){ 
        cout << "Maximum number of student reached, can't add more" << endl;
        return;
    }
    cout << "Enter the name of the student: ";
    getline(cin, students[studentcount].name);
    students[studentcount].datecount = 0;
    studentcount++;
    cout << "Student added successfully\n";
}
void loadStudents(){
    ifstream file("students.dat");
    if (!file.is_open()){
        cout << "No existing student data found, starting fresh.\n";
        return;
    }
    file >> studentcount;
    file.ignore();
    for (int i = 0; i < studentcount; i++){
        getline(file, students[i].name);
        file >> students[i].datecount;
        file.ignore();
        for (int j = 0; j < students[i].datecount; j++){
            getline(file, students[i].records[j].date);
            file >> students[i].records[j].ispresent;
            file.ignore();
        }
    }
    file.close();
    cout << "Student data loaded successfully.\n";
}

void saveStudents(){
    ofstream file("students.dat");
    if (!file.is_open()){
        cout << "Error saving student data!\n";
        return;
    }
    file << studentcount << endl;
    for (int i = 0; i < studentcount; i++){
        file << students[i].name << endl;
        file << students[i].datecount << endl;
        for (int j = 0; j < students[i].datecount; j++){
            file << students[i].records[j].date << endl;
            file << students[i].records[j].ispresent << endl;
        }
    }
    file.close();
    cout << "Student data saved successfully.\n";
}

void mark_attendance(){
    if (studentcount == 0){
        cout << "No student found\n";
        return;
    }
    string date;
    cout << "Enter the date(dd-mm-yyyy): ";
    getline(cin, date);
    
    for (int i = 0; i < studentcount; i++){
        cout << "Is " << students[i].name << " present? (y/n): ";
        char present;
        cin >> present;
        cin.ignore();
        
        students[i].records[students[i].datecount].ispresent = (present == 'y' || present == 'Y');
        students[i].records[students[i].datecount].date = date;
        students[i].datecount++;
    }
    cout << "Attendance marked successfully\n";
}

void viewOwnAttendance(){
    if (studentcount == 0){
        cout << "No student found\n";
        return;
    }
    for (int i = 0; i < studentcount; i++){
        cout << "Attendance for " << students[i].name << ":\n";
        for (int j = 0; j < students[i].datecount; j++){
            cout << students[i].records[j].date << ": " 
                 << (students[i].records[j].ispresent ? "Present" : "Absent") << endl;
        }
        cout << endl;
    }
}

int searchStudent(int index, const string& query) {
    if (index == studentcount) return -1;
    if (students[index].name == query) return index;
    return searchStudent(index + 1, query);
}

void displayRecords() {
    if (studentcount == 0) {
        cout << "No records to display.\n";
        return;
    }

    for (int i = 0; i < studentcount; i++) {
        cout << "\nStudent: " << students[i].name << "\n";
        if (students[i].datecount == 0) {
            cout << "  No attendance records.\n";
            continue;
        }

        for (int j = 0; j < students[i].datecount; j++) {
            cout << "  Date: " << students[i].records[j].date
                 << ", Status: "
                 << (students[i].records[j].ispresent ? "Present" : "Absent") << "\n";
        }
    }
}


void teacherMenu() {
    while (true) {
        cout << "\n--- Teacher Menu ---\n";
        cout << "1. Add Student\n";
        cout << "2. Mark Attendance\n";
        cout << "3. View All Attendance Records\n";
        cout << "4. Logout\n";
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: addstudent(); break;
            case 2: mark_attendance(); break;
            case 3: displayRecords(); break;
            case 4: return;
            default: cout << "Invalid choice\n";
        }
    }
}

void studentMenu() {
    while (true) {
        cout << "\n--- Student Menu ---\n";
        cout << "1. View My Attendance\n";
        cout << "2. Logout\n";
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: viewOwnAttendance(); break;
            case 2: return;
            default: cout << "Invalid choice\n";
        }
    }
}

int main() {
    loadStudents();
    while (true) {
        cout << "\n--- Attendance Management System ---\n";
        cout << "1. Teacher Login\n";
        cout << "2. Student Login\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            string pwd;
            cout << "Enter password: ";
            getline(cin, pwd);
            if (pwd == password) {
                teacherMenu();
            } else {
                cout << "Incorrect password.\n";
            }
        } else if (choice == 2) {
            string name;
            cout << "Enter your name: ";
            getline(cin, name);
            int index = searchStudent(0, name);
            if (index != -1) {
                studentMenu();
            } else {
                cout << "Student not found.\n";
            }
        } else if (choice == 3) {
            saveStudents();
            cout << "Exiting the system. Goodbye!\n";
            break;
        } else {
            cout << "Invalid choice, please try again.\n";
        }
    }
    return 0;
}