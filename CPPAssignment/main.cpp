#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct User {
    string username;
    string password;
    string phone;
};

// ----------------- User Functions -----------------
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

// ----------------- Organizer Functions -----------------
void adminMenu() {
    int choice;
    do {
        cout << "\n===== Organizer Page (Admin) =====\n";
        cout << "1. View Registered Events\n";
        cout << "2. Monitor Task Progress\n";
        cout << "3. Update Logistics\n";
        cout << "4. Track Payment Status\n";
        cout << "5. Generate Reports\n";
        cout << "6. Logout\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: cout << "Showing all registered events...\n"; break;
            case 2: cout << "Monitoring tasks...\n"; break;
            case 3: cout << "Updating logistics...\n"; break;
            case 4: cout << "Tracking payments...\n"; break;
            case 5: cout << "Generating reports...\n"; break;
            case 6: cout << "Logging out...\n"; break;
            default: cout << "Invalid choice!\n";
        }
    } while (choice != 6);
}

// ----------------- Registration & Login -----------------
void registerUser() {
    User newUser;
    cout << "\n--- Register New User ---\n";
    cout << "Enter username: ";
    cin >> newUser.username;
    cout << "Enter password: ";
    cin >> newUser.password;
    cout << "Enter phone number: ";
    cin >> newUser.phone;

    ofstream outFile("users.txt", ios::app);
    if (!outFile) {
        cout << "Error opening file!\n";
        return;
    }
    outFile << newUser.username << " " << newUser.password << " " << newUser.phone << endl;
    outFile.close();

    cout << "Registration successful!\n";
}

void loginUser() {
    string username, password;
    cout << "\n--- Login ---\n";
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    // Organizer/Admin login
    if (username == "admin" && password == "admin") {
        cout << "\nLogin successful! Welcome Organizer.\n";
        adminMenu();
        return;
    }

    // Normal customer login
    ifstream inFile("users.txt");
    if (!inFile) {
        cout << "Error: No users registered yet!\n";
        return;
    }

    User u;
    bool found = false;

    while (inFile >> u.username >> u.password >> u.phone) {
        if (u.username == username && u.password == password) {
            found = true;
            cout << "\nLogin successful! Welcome " << u.username << ".\n";
            userMenu(u.username);
            break;
        }
    }

    inFile.close();

    if (!found) {
        cout << "\nError: Invalid username or password!\n";
    }
}

// ----------------- Main Menu -----------------
int main() {
    int choice;
    do {
        cout << "\n===== Wedding Event Management System =====\n";
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: registerUser(); break;
            case 2: loginUser(); break;
            case 3: cout << "Goodbye!\n"; break;
            default: cout << "Invalid choice!\n";
        }
    } while (choice != 3);

    return 0;
}
