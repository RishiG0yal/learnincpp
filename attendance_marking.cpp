#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <sstream>
using namespace std;

const string password = "admin123";
const int Max = 200;
const int max_date = 31;
int studentcount = 0;

struct attendance {
    string date;
    bool ispresent;
};

struct student {
    string name;
    attendance records[max_date];
    int datecount = 0;
};
student students[Max];

void add_student(){
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

void save_students(){
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

void load_students(){
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


int search_student(int index, const string& query) {
    if (index == studentcount) return -1;
    if (students[index].name == query) return index;
    return search_student(index + 1, query);
}

void view_own_attendance(){
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

void display_records() {
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

void delete_all_students(){
    studentcount = 0;
    remove("students.dat");
    cout << "All student records deleted successfully.\n";
}


void teacher_menu() {
    while (true) {
        cout << "\n--- Teacher Menu ---\n";
        cout << "1. Add Student\n";
        cout << "2. Mark Attendance\n";
        cout << "3. View All Attendance Records\n";
        cout << "4. Delete All Students\n";
        cout << "5. Logout\n";
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: 
                add_student();
                break;
            case 2: 
                mark_attendance(); 
                break;
            case 3: 
                display_records(); 
                break;
            case 4: 
                delete_all_students(); 
                break;
            case 5: 
                save_students();
                return;
            default: 
                cout << "Invalid choice\n";
        }
    }
}

void student_menu() {
    while (true) {
        cout << "\n--- Student Menu ---\n";
        cout << "1. View My Attendance\n";
        cout << "2. Logout\n";
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: view_own_attendance(); break;
            case 2: return;
            default: cout << "Invalid choice\n";
        }
    }
}

int main() {
    load_students();
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
                teacher_menu();
            } else {
                cout << "Incorrect password.\n";
            }
        } else if (choice == 2) {
            string name;
            cout << "Enter your name: ";
            getline(cin, name);
            int index = search_student(0, name);
            if (index != -1) {
                student_menu();
            } else {
                cout << "Student not found.\n";
            }
        } else if (choice == 3) {
            cout << "Exiting the system. Goodbye!\n";
            break;
        } else {
            cout << "Invalid choice, please try again.\n";
        }
    }
    return 0;
}