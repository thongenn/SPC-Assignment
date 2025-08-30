#include <iostream>
#include <fstream>
#include <string>
#include <regex>
using namespace std;


struct User {
    string username;
    string password;
    string phone;
};

void mainMenu();
void registerUser();
void loginUser();
void userMenu(const string& username);
void adminMenu();
bool validatePhone(const string& phone);
bool validatePassword(const string& password);

void userMenu(const string& username) {
    int choice;
    do {
        cout << "\n===== User Page (" << username << ") =====\n";
        cout << "1. Register Wedding Event\n";
        cout << "2. Customize Food & Menu Packages\n";
        cout << "3. Make Payment & Checkout\n";
        cout << "4. View Receipt\n";
        cout << "5. Logout\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: cout << "Event Registration selected...\n"; break;
            case 2: cout << "Food & Menu customization selected...\n"; break;
            case 3: cout << "Payment & Checkout selected...\n"; break;
            case 4: cout << "Receipt displayed...\n"; break;
            case 5: cout << "Logging out...\n"; break;
            default: cout << "Invalid choice!\n";
        }
    } while (choice != 5);
}


void adminMenu() {
    int choice;
    do {
        cout << "\n===== Organizer Page (Admin) =====\n";
        cout << "1. Create venue\n";
        cout << "2. Create Menu\n";
        cout << "3. Monitor Task Progress\n";
        cout << "4. Generate Reports\n";
        cout << "5. Logout\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: cout << "Create venue\n"; break;
            case 2: cout << "Create Menu\n"; break;
            case 3: cout << "Monitoring\n"; break;
            case 4: cout << "Generating reports...\n"; break;
            case 5: cout << "Logging out...\n"; break;
            default: cout << "Invalid choice!\n";
        }
    } while (choice != 5);
}

void registerUser() {
    User newUser;
    cout << "\n--- Register New User ---\n";

    cout << "Enter username: ";
    getline(cin >> ws, newUser.username);

    cout << "Enter password (min 6 chars, at least one number): ";
    getline(cin >> ws, newUser.password);
    if (!validatePassword(newUser.password)) {
        cout << "Weak password! Must be at least 6 chars and contain a number.\n";
        return;
    }

    cout << "Enter phone number (10–11 digits): ";
    getline(cin >> ws, newUser.phone);
    if (!validatePhone(newUser.phone)) {
        cout << "Invalid phone number format.\n";
        return;
    }

    ofstream outFile("users.txt", ios::app);
    if (!outFile) {
        cout << "Error opening file!\n";
        return;
    }
    outFile << newUser.username << " " << newUser.password << " " << newUser.phone << endl;
    outFile.close();

    cout << "Registration successful!\n";
}

// ----------------- Login -----------------
void loginUser() {
    string username, password;
    cout << "\n--- Login ---\n";
    cout << "Enter username: ";
    getline(cin >> ws, username);
    cout << "Enter password: ";
    getline(cin >> ws, password);

    // Admin login
    if (username == "admin" && password == "admin") {
        cout << "\nAdmin login successful! Welcome Organizer.\n";
        adminMenu();
        return;
    }

    ifstream inFile("users.txt");
    if (!inFile) {
        cout << "No users registered yet!\n";
        return;
    }

    User u;
    bool found = false;

    while (inFile >> u.username >> u.password >> u.phone) {
        if (u.username == username && u.password == password) {
            found = true;
            cout << "\n Login successful! Welcome " << u.username << ".\n";
            userMenu(u.username);
            break;
        }
    }
    inFile.close();

    if (!found) {
        cout << "Invalid username or password!\n";
    }
}

bool validatePhone(const string& phone) {
    regex phonePattern("^[0-9]{10,11}$");
    return regex_match(phone, phonePattern);
}

bool validatePassword(const string& password) {
    if (password.length() < 6) return false;
    regex passPattern("^(?=.*[0-9]).{6,}$");
    return regex_match(password, passPattern);
}

// ----------------- Main Menu -----------------
void mainMenu() {
    int choice;
    do {
        cout << "\n===== Wedding Event Management System =====\n";
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: registerUser(); break;
            case 2: loginUser(); break;
            case 3: cout << "👋 Goodbye!\n"; break;
            default: cout << "Invalid choice!\n";
        }
    } while (choice != 3);
}

// ----------------- Main -----------------
int main() {
    cout << "=============================================\n";
    cout << "     Wedding Event Management System\n";
    cout << "=============================================\n";

    mainMenu();
    return 0;
}
