#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
using namespace std;

const int ROWS = 5, COLS = 10;
int seats[ROWS][COLS] = {0};

bool convertSeatInput(string seatInput, int &row, int &col) {
    if (seatInput.length() < 2) return false;
    
    char rowChar = toupper(seatInput[0]);
    if (rowChar < 'A' || rowChar >= 'A' + ROWS) return false;
    row = rowChar - 'A';
    
    try {
        col = stoi(seatInput.substr(1)) - 1;
        return (col >= 0 && col < COLS);
    } catch (...) {
        return false;
    }
}

int getValidInput(int min, int max) {
    int input;
    while (!(cin >> input) || input < min || input > max) {
        cout << "Invalid input. Enter " << min << "-" << max << ": ";
        cin.clear();
        cin.ignore(1000, '\n');
    }
    return input;
}

int getAvailableSeats() {
    int count = 0;
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            if (seats[i][j] == 0) count++;
    return count;
}

void loadSeats() {
    ifstream file("seats.txt");
    if (file.is_open()) {
        for (int i = 0; i < ROWS; i++)
            for (int j = 0; j < COLS; j++)
                file >> seats[i][j];
        file.close();
        cout << "Seat data loaded.\n";
    } else {
        cout << "Starting fresh.\n";
    }
}

void viewSeats() {
    cout << "\nSeat Layout (0 = Available, 1 = Booked):\n   ";
    for (int j = 1; j <= COLS; j++) cout << j << " ";
    cout << "\n";
    
    for (int i = 0; i < ROWS; i++) {
        cout << char('A' + i) << ": ";
        for (int j = 0; j < COLS; j++) cout << seats[i][j] << " ";
        cout << "\n";
    }
}

void bookSeat() {
    string seatInput;
    int row, col;
    
    cout << "Enter seat (e.g., A1): ";
    cin >> seatInput;
    
    if (!convertSeatInput(seatInput, row, col)) {
        cout << "Invalid seat format. Please make a valid choice!\n";
        return;
    }
    
    if (seats[row][col] == 0) {
        seats[row][col] = 1;
        cout << "Seat " << seatInput << " booked successfully!\n";
    } else {
        cout << "Seat " << seatInput << " already booked!\n";
    }
}

void bookMultipleSeats() {
    int available = getAvailableSeats();
    if (available == 0) {
        cout << "No seats available at this moment, try next screening!\n";
        return;
    }
    
    cout << "Available seats: " << available << "\n";
    cout << "Enter number of seats to book (max 10): ";
    int numSeats = getValidInput(1, min(available, 10));
    
    for (int i = 0; i < numSeats; i++) {
        cout << "\nBooking seat " << (i+1) << " of " << numSeats << ":\n";
        viewSeats();
        bookSeat();
    }
}

void cancelSeat() {
    string seatInput;
    int row, col;
    
    cout << "Enter seat to cancel booking for: ";
    cin >> seatInput;
    
    if (!convertSeatInput(seatInput, row, col)) {
        cout << "Invalid seat format.\n";
        return;
    }
    
    if (seats[row][col] == 1) {
        seats[row][col] = 0;
        cout << "Seat " << seatInput << " cancelled sussecfully!\n";
    } else {
        cout << "Seat " << seatInput << " not booked!\n";
    }
}

void saveSeats() {
    ofstream file("seats.txt");
    if (!file.is_open()) {
        cout << "Error saving data!\n";
        return;
    }
    
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++)
            file << seats[i][j] << " ";
        file << "\n";
    }
    file.close();
    cout << "Data saved.\n";
}

void clearAllBookings() {
    cout << "Clear all bookings? (y/n): ";
    char confirm;
    cin >> confirm;
    if (confirm != 'y' && confirm != 'Y') {
        cout << "Cancelled.\n";
        return;
    }
    
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            seats[i][j] = 0;
    
    remove("seats.txt");
    cout << "All bookings cleared!\n";
}

int main() {
    cout << "Movie Seat Booking System\n";
    cout << "-------------------------\n";
    loadSeats();
    
    while (true) {
        cout << "\n1. View seats\n2. Book seat\n3. Book multiple seats\n";
        cout << "4. Cancel seat\n5. Clear all\n6. Exit\n-------------------------\n";
        cout << "Please chose a service: ";
        
        switch (getValidInput(1, 6)) {
            case 1: 
                    viewSeats(); 
                    break;
            case 2: 
                    bookSeat(); 
                    break;
            case 3: 
                    bookMultipleSeats(); 
                    break;
            case 4: 
                    cancelSeat(); 
                    break;
            case 5: 
                    clearAllBookings();
                    break;
            case 6: 
                saveSeats();
                cout << "Thank you for using our system!\n";
                return 0;
        }
    }
}