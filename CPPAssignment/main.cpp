#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cstdio>
#include <vector>
#include <limits>
using namespace std;

struct User {
    string username;
    string password;
    string phone;
};

struct Date {
    int day;
    int month;
    int year;
};

struct Venue {
    int hall;
    int capacity;  
};

enum SlotType { MORNING, AFTERNOON, EVENING };

struct TimeSlot {
    SlotType type;
};

const int max_equipment = 4;

struct Equipment {
    string name;
    string detail;
};

struct Menu {
    string cuisine;
    string dietary;
    string servingStyle;
};

struct Event {
    string customer;
    Date date;
    Venue venue;
    TimeSlot slot;
    string theme;
    Equipment equipment[max_equipment];
    int equipmentcount;
    Menu menu; // attach menu directly
};

vector<Event> events;

// ---------- Date Validation ----------
bool isValidDate(int year, int month, int day) {
    if (month < 1 || month > 12) return false;
    int daysInMonth[] = {31, 28, 31, 30, 31, 30,
                         31, 31, 30, 31, 30, 31};
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
        daysInMonth[1] = 29;
    if (day < 1 || day > daysInMonth[month - 1]) return false;
    return true;
}

Date getTodayDate() {
    time_t t = time(0);
    tm* now = localtime(&t);
    Date today;
    today.day = now->tm_mday;
    today.month = now->tm_mon + 1;
    today.year = now->tm_year + 1900;
    return today;
}

Date parseDate(const string& input) {
    Date d;
    if (sscanf(input.c_str(), "%d-%d-%d", &d.year, &d.month, &d.day) != 3) {
        d.year = -1;
        return d;
    }
    if (!isValidDate(d.year, d.month, d.day)) {
        d.year = -1;
    }
    return d;
}

string dateToString(const Date& d) {
    char buffer[11];
    snprintf(buffer, sizeof(buffer), "%04d-%02d-%02d", d.year, d.month, d.day);
    return string(buffer);
}

bool isBefore(const Date& a, const Date& b) {
    if (a.year != b.year) return a.year < b.year;
    if (a.month != b.month) return a.month < b.month;
    return a.day < b.day;
}

string slotToString(const TimeSlot& slot) {
    switch (slot.type) {
        case MORNING: return "Morning (09:00-13:00)";
        case AFTERNOON: return "Afternoon (14:00-18:00)";
        case EVENING: return "Evening (18:00-22:00)";
    }
    return "Unknown";
}

// ---------- Conflict Check ----------
bool hasConflict(int excludeIndex, const Event& e) {
    for (int i = 0; i < events.size(); i++) {
        if (i == excludeIndex) continue; // skip self
        if (events[i].date.year == e.date.year &&
            events[i].date.month == e.date.month &&
            events[i].date.day == e.date.day &&
            events[i].venue.hall == e.venue.hall &&
            events[i].slot.type == e.slot.type) {
            return true;
        }
    }
    return false;
}

// ----------  File Handling for Event----------

void saveDataToFile() {
    ofstream file("data.txt");
    if (!file.is_open()) {
        cout << "Error: Cannot open data file for writing!\n";
        return;
    }

    for (const auto& event : events) {
        file << event.customer << "\n";
        file << event.date.year << " " << event.date.month << " " << event.date.day << "\n";
        file << event.venue.hall << " " << event.venue.capacity << "\n";
        file << event.slot.type << "\n";
        file << event.theme << "\n";
        file << event.equipmentcount << "\n";
        for (int j = 0; j < event.equipmentcount; j++) {
            file << event.equipment[j].name << "\n";
            file << event.equipment[j].detail << "\n";
        }

        // Save corresponding Menu
        file << event.menu.cuisine << "\n";
        file << event.menu.dietary << "\n";
        file << event.menu.servingStyle << "\n";

        file << "---\n"; // Separator between event+menu pairs
    }

    file.close();
    cout << "Events and menus saved together successfully.\n";
}

void loadDataFromFile() {
    ifstream file("data.txt");
    if (!file.is_open()) {
        cout << "No existing data file found. Starting fresh.\n";
        return;
    }

    events.clear();
    string line;

    while (getline(file, line)) {
        if (line.empty() || line == "---") continue;

        Event event;
        event.customer = line;

        // Read Event
        file >> event.date.year >> event.date.month >> event.date.day;
        file.ignore();

        file >> event.venue.hall >> event.venue.capacity;
        file.ignore();

        int slotType;
        file >> slotType;
        file.ignore();
        event.slot.type = static_cast<SlotType>(slotType);

        getline(file, event.theme);

        file >> event.equipmentcount;
        file.ignore();
        for (int i = 0; i < event.equipmentcount; i++) {
            getline(file, event.equipment[i].name);
            getline(file, event.equipment[i].detail);
        }

        // Read corresponding Menu
        getline(file, event.menu.cuisine);
        getline(file, event.menu.dietary);
        getline(file, event.menu.servingStyle);

        events.push_back(event);

        // Skip separator
        getline(file, line);
    }

    file.close();
    cout << "Data loaded successfully. " << events.size() << " event-menu pairs loaded.\n";
}

// ---------- Event Functions ----------
void createEvent(const string& username) {
    Event e;
    e.customer = username;

    Date today = getTodayDate();
    cin.ignore();
    while (true) {
        cout << "Enter event date (yyyy-mm-dd): ";
        string dateInput;
        getline(cin, dateInput);
        e.date = parseDate(dateInput);
        if (e.date.year == -1) {
            cout << "Invalid date format! Please try again.\n";
            continue;
        }
        if (isBefore(e.date, today)) {
            cout << "Cannot be in the past! Please try again.\n";
            continue;
        }
        break;
    }

    do{
        cout << "Enter hall (1-5): ";
        cin >> e.venue.hall;
        if (e.venue.hall < 1 || e.venue.hall > 5){
            cout << "Invalid hall! Please try again\n";
            continue;
        }
    }while (e.venue.hall < 1 || e.venue.hall > 5);
    
    do{
        cout << "Enter capacity: ";
        cin >> e.venue.capacity;
        if (e.venue.capacity < 0){
            cout << "Invalid! Please try again.\n";
            continue;
        };
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }while (e.venue.capacity < 0);

    while (true){
        cout << "Choose time slot:\n"
             << "1. Morning (09:00-13:00)\n"
             << "2. Afternoon (14:00-18:00)\n"
             << "3. Evening (18:00-22:00)\n"
             << "Enter choice: ";
        int slotChoice;
        cin >> slotChoice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (slotChoice == 1) {
            e.slot.type = MORNING;
            break;
        }
        else if (slotChoice == 2) {
            e.slot.type = AFTERNOON;
            break;
        }
        else if (slotChoice == 3) {
            e.slot.type = EVENING;
            break;
        }
        else {
            cout << "Invalid choice. Please try again\n";
            continue;
        }
    }
    
// Choose theme
while (true) {
    int themeChoice;
    cout << "Select theme: \n";
    cout << "1. Traditional Chinese\n";
    cout << "2. Traditional Indian\n";
    cout << "3. Traditional Malay\n";
    cout << "4. Western\n";
    cout << "5. Italian\n";
    cout << "6. Others\n";
    cout << "Enter choice: ";
    cin >> themeChoice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    switch (themeChoice) {
        case 1: 
            e.theme = "Traditional Chinese";
            break;
        case 2:
            e.theme = "Traditional Indian";
            break;
        case 3:
            e.theme = "Traditional Malay";
            break;
        case 4:
            e.theme = "Western";
            break;
        case 5:
            e.theme = "Italian";
            break;
        case 6:
            cout << "Enter custom theme: ";
            getline(cin, e.theme);
            break;
        default:
            cout << "Invalid choice, please try again.\n";
            continue; // loop again
    }
    break; // exit loop if valid choice
}

    // ----- Equipment Selection -----
    vector<string> availableEquipment = {
        "Microphone",
        "Speaker System",
        "Lighting System",
        "Decoration Set"
    };

    cout << "\nSelect equipment for the event (enter 0 to continue register):\n";
    int choice;
    e.equipmentcount = 0;

    while (true) {
        cout << "\nAvailable equipment:\n";
        for (int i = 0; i < availableEquipment.size(); i++) {
            cout << i + 1 << ". " << availableEquipment[i] << "\n";
        }
        cout << "0. Finish selection\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == 0) break; // stop selection

        if (choice < 1 || choice > availableEquipment.size()) {
            cout << "Invalid choice. Please try again.\n";
            continue;
        }

        if (e.equipmentcount >= max_equipment) {
            cout << "You have reached the maximum number of equipment (" 
                 << max_equipment << ").\n";
            break;
        }

        e.equipment[e.equipmentcount].name = availableEquipment[choice - 1];
        cout << "Enter detail for " << availableEquipment[choice - 1] << ": ";
        getline(cin, e.equipment[e.equipmentcount].detail);
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        e.equipmentcount++;
    }

    // check for conflict
    if (hasConflict(-1, e)) {
        cout << "Error: This venue and time slot is already booked!\n";
        return;
    }

    events.push_back(e);
    saveDataToFile();
    cout << "Event created successfully.\n";
}

// Show events`
void retrieveEvents() {
    if (events.empty()) {
        cout << "No events found.\n";
        return;
    }
    cout << "\n--- Event List ---\n";
    for (int i = 0; i < events.size(); i++) {
        cout << i + 1 << ". Customer: " << events[i].customer << endl;
    }

    int j;
    while (true) {
        cout << "Enter event number to check detail (0 to main menu): ";
        cin >> j;
        if (j == 0) break;
        if (j > 0 && j <= events.size()) {
            cout << "Customer: " << events[j - 1].customer
                 << "\nDate: " << dateToString(events[j - 1].date)
                 << "\nVenue: " << events[j - 1].venue.hall
                 << "\nCapacity: " << events[j - 1].venue.capacity
                 << "\nTimeSlot: " << slotToString(events[j - 1].slot)
                 << "\nTheme: " << events[j - 1].theme             
                 << "\nEquipment: \n";
            if (events[j - 1].equipmentcount == 0) {
                cout << "None\n";
            } else {
                for (int k = 0; k < events[j - 1].equipmentcount; k++) {
                    cout << "  - " << events[j - 1].equipment[k].name 
                         << "  (" << events[j - 1].equipment[k].detail << ")\n";
                }
            }
            cout << "Menu:\n";
                if (events[j - 1].menu.cuisine.empty() && 
                    events[j - 1].menu.dietary.empty() && 
                    events[j - 1].menu.servingStyle.empty()) {
                    cout << "  Not customized yet\n";
                } else {
                    cout << "  Cuisine: " << events[j - 1].menu.cuisine << "\n"
                        << "  Dietary: " << events[j - 1].menu.dietary << "\n"
                        << "  Serving Style: " << events[j - 1].menu.servingStyle << "\n";
                }
        } else {
            cout << "Invalid selection.\n";
        }
    }
}

void customizeFoodPack(const string& username) {
    if (events.empty()) {
        cout << "No events found for customization.\n";
        return;
    }

    // Filter: show only this customer's events
    vector<int> userEventIndexes;
    cout << "\n--- Your Events ---\n";
    for (int i = 0; i < events.size(); i++) {
        if (events[i].customer == username) {
            cout << userEventIndexes.size() + 1 << ". " 
                 << dateToString(events[i].date)
                 << " (Hall " << events[i].venue.hall << ")\n";
            userEventIndexes.push_back(i);
        }
    }

    if (userEventIndexes.empty()) {
        cout << "You have no events registered.\n";
        return;
    }

    int choice;
    cout << "Select event to customize menu (0 to cancel): ";
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (choice <= 0 || choice > userEventIndexes.size()) {
        cout << "Cancelled.\n";
        return;
    }

    Event &e = events[userEventIndexes[choice - 1]];

    Menu m;
    int opt;

    // Cuisine
    while (true) {
        cout << "Select cuisine type:\n";
        cout << "1. Chinese\n2. Indian\n3. Malay\n4. Western\n5. Italian\n";
        cout << "Enter choice: ";
        cin >> opt;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (opt == 1) { m.cuisine = "Chinese"; break; }
        else if (opt == 2) { m.cuisine = "Indian"; break; }
        else if (opt == 3) { m.cuisine = "Malay"; break; }
        else if (opt == 4) { m.cuisine = "Western"; break; }
        else if (opt == 5) { m.cuisine = "Italian"; break; }
        else cout << "Invalid choice!\n";
    }

    // Dietary
    while (true) {
        cout << "Select dietary requirement:\n";
        cout << "1. None\n2. Vegetarian\n3. Vegan\n4. Halal\n5. Seafood-free\n6. Others\n";
        cout << "Enter choice: ";
        cin >> opt;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (opt == 1) { m.dietary = "None"; break; }
        else if (opt == 2) { m.dietary = "Vegetarian"; break; }
        else if (opt == 3) { m.dietary = "Vegan"; break; }
        else if (opt == 4) { m.dietary = "Halal"; break; }
        else if (opt == 5) { m.dietary = "Seafood-free"; break; }
        else if (opt == 6) { cout << "Enter custom dietary: "; getline(cin, m.dietary); break; }
        else cout << "Invalid choice!\n";
    }

    // Serving style
    while (true) {
        cout << "Select serving style:\n";
        cout << "1. Buffet\n2. Plated\n";
        cout << "Enter choice: ";
        cin >> opt;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (opt == 1) { m.servingStyle = "Buffet"; break; }
        else if (opt == 2) { m.servingStyle = "Plated"; break; }
        else cout << "Invalid choice!\n";
    }

    e.menu = m; //
    saveDataToFile();
    cout << "Menu added/updated successfully for this event.\n";
}

// ---------- Update Logistics ----------
void updateLogistics() {
    if (events.empty()) {
        cout << "No events to update.\n";
        return;
    }

    cout << "\n--- Update Logistics ---\n";
    for (int i = 0; i < events.size(); i++) {
        cout << i + 1 << ". " << events[i].customer 
             << " on " << dateToString(events[i].date) 
             << " (Hall " << events[i].venue.hall << ")\n";
    }

    int choice;
    cout << "Select event number to update (0 to cancel): ";
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (choice <= 0 || choice > events.size()) {
        cout << "Cancelled.\n";
        return;
    }

    Event &e = events[choice - 1];

    // Update venue hall
    int newHall;
    cout << "Current hall: " << e.venue.hall << "\n";
    cout << "Enter new hall (1-5) or 0 to keep current: ";
    cin >> newHall;
    if (newHall >= 1 && newHall <= 5) {
        e.venue.hall = newHall;
    }
    
    //update timeslot
    int newTime;
    cout << "Current time: " << slotToString(e.slot) << "\n";
    cout << "Enter new timeslot (0 to keep current):\n1. Morning\n2. Afternoon\n3. Evening\nChoice: ";
    cin >> newTime;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    switch(newTime){
        case 0: break;
        case 1: e.slot.type = MORNING; break;
        case 2: e.slot.type = AFTERNOON; break;
        case 3: e.slot.type = EVENING; break;
        default: cout << "Invalid choice. Keeping current time.\n";
    }

    // Update venue capacity
    int newCap;
    cout << "Current capacity: " << e.venue.capacity << "\n";
    cout << "Enter new capacity or 0 to keep current: ";
    cin >> newCap;
    if (newCap > 0) {
        e.venue.capacity = newCap;
    }

    // Update theme
    int newTheme;
    cout << "Current theme: " << e.theme << "\n";
    cout << "Select new theme or 0 to keep current:\n";
    cout << "1. Traditional Chinese\n2. Traditional Indian\n3. Traditional Malay\n4. Western\n5. Italian\n6. Others\n0. Keep current\nChoice: ";
    cin >> newTheme;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    switch (newTheme) {
        case 0: break;
        case 1: e.theme = "Traditional Chinese"; break;
        case 2: e.theme = "Traditional Indian"; break;
        case 3: e.theme = "Traditional Malay"; break;
        case 4: e.theme = "Western"; break;
        case 5: e.theme = "Italian"; break;
        case 6: cout << "Enter custom theme: "; getline(cin, e.theme); break;
        default: cout << "Invalid choice. Keeping current theme.\n";
    }

    // Update equipment
    if (e.equipmentcount > 0) {
        cout << "\nCurrent equipment:\n";
        for (int i = 0; i < e.equipmentcount; i++) {
            cout << i + 1 << ". " << e.equipment[i].name 
                 << " (x" << e.equipment[i].detail << ")\n";
        }

        int eqChoice;
        cout << "Select equipment number to update (0 to skip): ";
        cin >> eqChoice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (eqChoice > 0 && eqChoice <= e.equipmentcount) {
            string newDetail;
            cout << "Enter new detail for " << e.equipment[eqChoice - 1].name << ": ";
            getline(cin, newDetail);
                e.equipment[eqChoice - 1].detail = newDetail;
        }
    }

    saveDataToFile();
    cout << "Logistics updated successfully.\n";
}



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
            case 1: cout << "Event Registration selected...\n"; createEvent(username); break;
            case 2: cout << "Food & Menu customization selected...\n"; customizeFoodPack(username); break;
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
            case 1: cout << "Showing all registered events...\n"; retrieveEvents(); break;
            case 2: cout << "Monitoring tasks...\n"; break;
            case 3: cout << "Updating logistics...\n"; updateLogistics(); break;
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
