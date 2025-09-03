#include <iostream>
#include <sstream>
#include <string>
#include <limits>
#include <fstream>
#include <vector>
#include <ctime>
#include <cstdio>
#include <regex>
using namespace std;

struct User {
    string username;
    string password;
    string phone;
};

// Forward declarations
void mainMenu();
void registerUser();
void loginUser();
void userMenu(const string& username);
void adminMenu();
bool validatePhone(const string& phone);
bool validatePassword(const string& password);

// Event-related structures and functions
struct Date {
    int day;
    int month;
    int year;
};

struct Venue {
    int hall;
    string location;
    int capacity;
    double price;
    string timeslot; // "Morning" or "Evening"
};

const int max_equipment = 10;

struct Equipment {
    string name;
    string detail;
};

struct Menu {
    int id;
    string cuisine;
    vector<string> foodItems;
    double price;
};

struct Event {
    string customer;
    Date date;
    Venue venue;
    string theme;
    string servingStyle;
    Equipment equipment[max_equipment];
    int equipmentcount;
    Menu menu;
    vector<string> customizations;
};

// Global variables
vector<Event> events;
vector<Menu> menus;
vector<Venue> venues;
int nextMenuId = 1;

// Function declarations
bool isValidDate(int year, int month, int day);
Date getTodayDate();
Date parseDate(const string& input);
string dateToString(const Date& d);
bool isBefore(const Date& a, const Date& b);
bool hasConflict(const Event& e);
void saveDataToFile();
void loadDataFromFile();
void saveMenuToFile();
void loadMenuFromFile();
void saveVenueToFile();
void loadVenueFromFile();
void registerEvent(const string& username);
void retrieveEvents();
void createMenu();
void viewMenus();
void customizeMenu(Event &e);
void createVenue();
void viewVenues();

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
            case 1:
                //cout << "Event Registration selected...\n";
                registerEvent(username);
                break;
            case 2:
                cout << "Food & Menu customization selected...\n";
                // Find user's event first
                {
                    Event* userEvent = nullptr;
                    for (auto& event : events) {
                        if (event.customer == username) {
                            userEvent = &event;
                            break;
                        }
                    }
                    if (userEvent) {
                        customizeMenu(*userEvent);
                    } else {
                        cout << "No event found for this user. Please register an event first.\n";
                    }
                }
                break;
            case 3:
                cout << "Payment & Checkout selected...\n";
                break;
            case 4:
                cout << "Receipt displayed...\n";
                break;
            case 5:
                cout << "Logging out...\n";
                break;
            default:
                cout << "Invalid choice!\n";
        }
    } while (choice != 5);
}

void adminMenu() {
    int choice;
    do {
        cout << "\n===== Organizer Page (Admin) =====\n";
        cout << "1. Create venue\n";
        cout << "2. Create Menu\n";
        cout << "3. View Events\n";
        cout << "4. View Venues\n";
        cout << "5. View Menus\n";
        cout << "6. Logout\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                cout << "Create venue\n";
                createVenue();
                break;
            case 2:
                cout << "Create Menu\n";
                createMenu();
                break;
            case 3:
                cout << "Viewing events...\n";
                retrieveEvents();
                break;
            case 4:
                cout << "Viewing venues...\n";
                viewVenues();
                break;
            case 5:
                cout << "Viewing menus...\n";
                viewMenus();
                break;
            case 6:
                cout << "Logging out...\n";
                break;
            default:
                cout << "Invalid choice!\n";
        }
    } while (choice != 6);
}

//register acc
void registerUser() {
    User newUser;
    cout << "--- Register New User ---\n";
    cout << "Enter username (press 0 to go back): ";
    cin >> newUser.username;
    if (newUser.username == "0") return;

    do {
        cout << "Enter password (min 6 chars, must include at least one digit): ";
        cin >> newUser.password;

        if (!validatePassword(newUser.password)) {
            cout << "Invalid password! Please try again.\n";
        }
    } while (!validatePassword(newUser.password));

    do {
        cout << "Enter phone number : ";
        cin >> newUser.phone;

        if (!validatePhone(newUser.phone)) {
            cout << "Invalid phone number! Please try again.\n";
        }
    } while (!validatePhone(newUser.phone));

    ofstream outFile("users.txt", ios::app);
    if (!outFile) {
        cout << "Error opening file!\n";
        return;
    }
    outFile << newUser.username << " " << newUser.password << " " << newUser.phone << endl;
    outFile.close();

    cout << "Registration successful!\n";
}

// login
void loginUser() {
    string username, password;
    cout << "\n--- Login ---\n";
    cout << "Enter username (enter 0 to go back) : ";
    getline(cin >> ws, username);
    if (username == "0") return;
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

    string line;
    bool found = false;
    while (getline(inFile, line)) {
        if (line.empty()) continue;

        istringstream ss(line);
        string fileUser, filePass, filePhone;

        // Correct usage of getline with delimiter
        if (!getline(ss, fileUser, '|')) continue;
        if (!getline(ss, filePass, '|')) continue;
        getline(ss, filePhone); // phone is last field (no delimiter needed)

        // Remove possible carriage return ('\r') at end (Windows issue)
        if (!filePhone.empty() && filePhone.back() == '\r')
            filePhone.pop_back();

        if (fileUser == username && filePass == password) {
            found = true;
            cout << "\nLogin successful! Welcome " << fileUser << ".\n";
            userMenu(fileUser);
            break;
        }
    }
    inFile.close();

    if (!found) {
        cout << "Invalid username or password!\n";
    }
}

bool validatePhone(const string& phone) {
    if (phone.size() < 10 || phone.size() > 11) {
        return false;
    }
    // Must start with "01"
    if (phone.substr(0, 2) != "01") {
        return false;
    }
    // Must be all digits
    for (char c : phone) {
        if (!isdigit(c)) return false;
    }
    return true;
}

bool validatePassword(const string& password) {
    if (password.length() < 6) return false;
    regex passPattern("^(?=.*[0-9]).{6,}$");
    return regex_match(password, passPattern);
}

// Date Validation
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

// Conflict Check
bool hasConflict(const Event& e) {
    for (const auto& existingEvent : events) {
        if (existingEvent.date.year == e.date.year &&
            existingEvent.date.month == e.date.month &&
            existingEvent.date.day == e.date.day &&
            existingEvent.venue.hall == e.venue.hall &&
            existingEvent.venue.timeslot == e.venue.timeslot) {
            return true;
        }
    }
    return false;
}

// File Handling for Event
void saveDataToFile() {
    ofstream file("data.txt");
    if (!file.is_open()) {
        cout << "Error: Cannot open data file for writing!\n";
        return;
    }

    for (const auto& event : events) {
        file << event.customer << "\n";
        file << event.date.year << " " << event.date.month << " " << event.date.day << "\n";
        file << event.venue.hall << " " << event.venue.location << " " << event.venue.capacity << " "
             << event.venue.price << " " << event.venue.timeslot << "\n";
        file << event.theme << "\n";
        file << event.servingStyle << "\n";
        file << event.equipmentcount << "\n";
        for (int j = 0; j < event.equipmentcount; j++) {
            file << event.equipment[j].name << "\n";
            file << event.equipment[j].detail << "\n";
        }
        file << event.menu.id << "\n";
        file << event.menu.cuisine << "\n";
        file << event.menu.price << "\n";
        file << event.menu.foodItems.size() << "\n";
        for (const auto& food : event.menu.foodItems) {
            file << food << "\n";
        }
        file << event.customizations.size() << "\n";
        for (const auto& custom : event.customizations) {
            file << custom << "\n";
        }
        file << "---\n";
    }

    file.close();
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

        file >> event.venue.hall >> event.venue.location >> event.venue.capacity
             >> event.venue.price >> event.venue.timeslot;
        file.ignore();

        getline(file, event.theme);
        getline(file, event.servingStyle);

        file >> event.equipmentcount;
        file.ignore();
        for (int i = 0; i < event.equipmentcount; i++) {
            getline(file, event.equipment[i].name);
            getline(file, event.equipment[i].detail);
        }

        // Read menu
        file >> event.menu.id;
        file.ignore();
        getline(file, event.menu.cuisine);
        file >> event.menu.price;
        file.ignore();
        int foodCount;
        file >> foodCount;
        file.ignore();
        for (int i = 0; i < foodCount; i++) {
            getline(file, line);
            event.menu.foodItems.push_back(line);
        }

        // Read customizations
        int customCount;
        file >> customCount;
        file.ignore();
        for (int i = 0; i < customCount; i++) {
            getline(file, line);
            event.customizations.push_back(line);
        }

        events.push_back(event);

        // Skip separator
        getline(file, line);
    }

    file.close();
    cout << "Data loaded successfully. " << events.size() << " event-menu pairs loaded.\n";
}

// Save all menus to file
void saveMenuToFile() {
    ofstream file("menus.txt");
    if (!file.is_open()) {
        cout << "Error: Cannot open menus file for writing!\n";
        return;
    }

    for (const auto& menu : menus) {
        file << menu.id << "\n";
        file << menu.cuisine << "\n";
        file << menu.price << "\n";
        file << menu.foodItems.size() << "\n";
        for (const auto& food : menu.foodItems) {
            file << food << "\n";
        }
        file << "---\n"; // separator between menus
    }
    file.close();
    cout << "Menus saved successfully.\n";
}

// Load menus from file
void loadMenuFromFile() {
    ifstream file("menus.txt");
    if (!file.is_open()) {
        cout << "No existing menus file found. Starting fresh.\n";
        return;
    }

    menus.clear();
    string line;
    while (getline(file, line)) {
        if (line.empty() || line == "---") continue;

        Menu m;
        m.id = stoi(line);
        if (m.id >= nextMenuId) nextMenuId = m.id + 1;

        getline(file, m.cuisine);

        getline(file, line);
        m.price = stod(line);

        int foodCount;
        file >> foodCount;
        file.ignore();
        for (int i = 0; i < foodCount; i++) {
            getline(file, line);
            m.foodItems.push_back(line);
        }

        menus.push_back(m);
        getline(file, line); // read separator
    }

    file.close();
    cout << "Menus loaded successfully. " << menus.size() << " menus available.\n";
}

// Save all venues to file
void saveVenueToFile() {
    ofstream file("venues.txt");
    if (!file.is_open()) {
        cout << "Error: Cannot open venues file for writing!\n";
        return;
    }

    for (const auto& v : venues) {
        file << v.hall << "\n"
             << v.location << "\n"
             << v.capacity << "\n"
             << v.price << "\n"
             << "---\n";
    }

    file.close();
    cout << "Venues saved successfully.\n";
}

// Load venues from file
void loadVenueFromFile() {
    ifstream file("venues.txt");
    if (!file.is_open()) {
        cout << "No existing venues file found. Starting fresh.\n";
        return;
    }

    venues.clear();
    string line;
    while (getline(file, line)) {
        if (line.empty() || line == "---") continue;

        Venue v;
        v.hall = stoi(line);

        getline(file, v.location);

        getline(file, line);
        v.capacity = stoi(line);

        getline(file, line);
        v.price = stod(line);

        venues.push_back(v);
        getline(file, line); // read separator
    }

    file.close();
    cout << "Venues loaded successfully. " << venues.size() << " venues available.\n";
}

// Event Functions
void registerEvent(const string& username) {
    Event e;
    e.customer = username;

    Date today = getTodayDate();
    //cin.ignore();
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

    // Venue Selection
    bool validHall = false;
    string chosenSlot;
    while (!validHall) {
        cout << "\nAvailable Venues:\n";
        for (const auto& v : venues) {
            cout << "Hall " << v.hall << " | Location: " << v.location
                 << " | Capacity: " << v.capacity
                 << " | Price: RM" << v.price << "\n";
        }

        int chosenHall;
        cout << "Enter hall number: ";
        cin >> chosenHall;

        cout << "Choose slot (1=Morning, 2=Evening): ";
        int slotChoice;
        cin >> slotChoice;

        if (slotChoice < 1 || slotChoice > 2) {
            cout << "Invalid slot selection. Try again.\n";
            continue;
        }

        chosenSlot = (slotChoice == 1) ? "Morning" : "Evening";

        // Find the selected venue
        bool hallFound = false;
        for (const auto& v : venues) {
            if (v.hall == chosenHall) {
                e.venue = v;
                e.venue.timeslot = chosenSlot;
                hallFound = true;
                validHall = true;
                break;
            }
        }

        if (!hallFound) {
            cout << "Invalid hall number. Try again.\n";
            continue;
        }

        // Check for conflicts
        if (hasConflict(e)) {
            cout << "Error: This venue and time slot is already booked! Please choose a different hall or time slot.\n";
            validHall = false;
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

    // Serving style
    while (true) {
        int styleChoice;
        cout << "Select serving style:\n";
        cout << "1. Buffet\n2. Plated\n";
        cout << "Enter choice: ";
        cin >> styleChoice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (styleChoice == 1) { e.servingStyle = "Buffet"; break; }
        else if (styleChoice == 2) { e.servingStyle = "Plated"; break; }
        else cout << "Invalid choice!\n";
    }

    // Equipment Selection
    vector<string> availableEquipment = {
        "Microphone",
        "Speaker System",
        "Lighting System",
        "Seat"
    };

    cout << "\nSelect equipment for the event (enter 0 to finish):\n";
    int choice;
    e.equipmentcount = 0;

    while (true) {
        cout << "\nAvailable equipment:\n";
        for (int i = 0; i < availableEquipment.size(); i++) {
            cout << i + 1 << ". " << availableEquipment[i] << "\n";
        }
        cout << "0. Finish selection\n";
        cout << "Enter your choice: ";

        // Validate input
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a number.\n";
            continue;
        }

        if (choice == 0) break; // stop selection

        if (choice < 1 || choice > (int)availableEquipment.size()) {
            cout << "Invalid choice. Please try again.\n";
            continue;
        }

        if (e.equipmentcount >= max_equipment) {
            cout << "You have reached the maximum number of equipment ("
                << max_equipment << ").\n";
            break;
        }

        // Store equipment name
        e.equipment[e.equipmentcount].name = availableEquipment[choice - 1];

        // Clear buffer before getline
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        // Ask for custom needs
        cout << "Enter detail/needs for " << availableEquipment[choice - 1] << ": ";
        getline(cin, e.equipment[e.equipmentcount].detail);

        e.equipmentcount++;
    }

    // Show what user selected
    cout << "\nYou selected:\n";
    for (int i = 0; i < e.equipmentcount; i++) {
        cout << "- " << e.equipment[i].name
             << " (" << e.equipment[i].detail << ")\n";
    }

    events.push_back(e);
    saveDataToFile();
    cout << "Event created successfully.\n";
}

// Show events
void retrieveEvents() {
    if (events.empty()) {
        cout << "No events found.\n";
        return;
    }
    cout << "\n--- Customer List ---\n";
    for (int i = 0; i < events.size(); i++) {
        cout << i + 1 << ". Customer: " << events[i].customer << endl;
    }

    int j;
    while (true) {
        cout << "Enter num of customer to check detail (0 to main menu): ";
        cin >> j;
        if (j == 0) break;
        if (j > 0 && j <= events.size()) {
            cout << "Customer: " << events[j - 1].customer
                 << "\nDate: " << dateToString(events[j - 1].date)
                 << "\nVenue: Hall " << events[j - 1].venue.hall
                 << "\nLocation: " << events[j - 1].venue.location
                 << "\nCapacity: " << events[j - 1].venue.capacity
                 << "\nTime Slot: " << events[j - 1].venue.timeslot
                 << "\nTheme: " << events[j - 1].theme
                 << "\nServing Style: " << events[j - 1].servingStyle
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
            if (events[j - 1].menu.cuisine.empty()) {
                cout << "  Not customized yet\n";
            } else {
                cout << "  Cuisine: " << events[j - 1].menu.cuisine << "\n";
                cout << "  Price: RM" << events[j - 1].menu.price << "\n";
                cout << "  Food Items:\n";
                for (const auto& food : events[j - 1].menu.foodItems) {
                    cout << "    - " << food << "\n";
                }
            }
            if (!events[j - 1].customizations.empty()) {
                cout << "  Customizations:\n";
                for (const auto& custom : events[j - 1].customizations) {
                    cout << "    - " << custom << "\n";
                }
            }
        } else {
            cout << "Invalid selection.\n";
        }
    }
}

// Create Menu
void createMenu(){
    Menu m;
    m.id = nextMenuId++;

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

    // Input food items
    cout << "\nEnter food items for " << m.cuisine << " cuisine.\n";
    string item;
    while (true) {
        cout << "Enter food item (or type 'done' to finish): ";
        getline(cin, item);
        if (item == "done" || item == "DONE") break;
        if (!item.empty()) {
            m.foodItems.push_back(item);
            cout << "Added: " << item << endl;
        }
    }

    cout << "Enter price per table for this menu: RM";
    cin >> m.price;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    menus.push_back(m);
    saveMenuToFile();
    cout << "Menu created successfully with ID: " << m.id << "\n";
}

// View all menus
void viewMenus() {
    if (menus.empty()) {
        cout << "No menus available.\n";
        return;
    }

    cout << "\n--- Available Menus ---\n";
    for (int i = 0; i < menus.size(); i++) {
        cout << "ID: " << menus[i].id << "\n";
        cout << "Cuisine: " << menus[i].cuisine << "\n";
        cout << "Price per table: RM" << menus[i].price << " /per table\n";
        cout << "Food Items: ";
        for (int j = 0; j < menus[i].foodItems.size(); j++) {
            cout << menus[i].foodItems[j];
            if (j < menus[i].foodItems.size() - 1) cout << ", ";
        }
        cout << "\n\n";
    }
}

void customizeMenu(Event &e) {
    if (menus.empty()) {
        cout << "No menus available. Please ask admin to create a menu first.\n";
        return;
    }

    cout << "\nAvailable Menus:\n";
    for (int i = 0; i < menus.size(); i++) {
        cout << i + 1 << ". " << menus[i].cuisine << " (RM" << menus[i].price << ")\n";
        cout << "   Food Items: ";
        for (int j = 0; j < menus[i].foodItems.size(); j++) {
            cout << menus[i].foodItems[j];
            if (j < menus[i].foodItems.size() - 1) cout << ", ";
        }
        cout << "\n\n";
    }

    int menuChoice;
    cout << "Select a menu you want for event (0 to cancel): ";
    cin >> menuChoice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (menuChoice <= 0 || menuChoice > menus.size()) {
        cout << "Menu selection cancelled.\n";
        return;
    }

    e.menu = menus[menuChoice - 1];

    // Ask for customizations
    cout << "\nDo you have any dietary restrictions or special requests? (y/n): ";
    char customizationChoice;
    cin >> customizationChoice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (customizationChoice == 'y' || customizationChoice == 'Y') {
        cout << "Enter your dietary restrictions or special requests (type 'done' when finished):\n";
        string custom;
        while (true) {
            getline(cin, custom);
            if (custom == "done" || custom == "DONE") break;
            if (!custom.empty()) {
                e.customizations.push_back(custom);
            }
        }
    }

    cout << "Menu customized successfully!\n";
    saveDataToFile();
}

// Create venue
void createVenue() {
    Venue v;

    cout << "\n=== Create Venue ===\n";
    cout << "Enter hall number: ";
    cin >> v.hall;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter location: ";
    getline(cin, v.location);

    cout << "Enter hall capacity: ";
    cin >> v.capacity;

    cout << "Enter price for hall rental: RM";
    cin >> v.price;

    venues.push_back(v);
    saveVenueToFile();

    cout << "\nVenue created successfully!\n";
    cout << "Hall " << v.hall << " | Location: " << v.location
         << " | Capacity: " << v.capacity
         << " | Price: RM" << v.price
         << "\n";
}

// View all venues
void viewVenues() {
    if (venues.empty()) {
        cout << "No venues available.\n";
        return;
    }

    cout << "\n--- Available Venues ---\n";
    for (int i = 0; i < venues.size(); i++) {
        cout << "Hall " << venues[i].hall << "\n";
        cout << "Location: " << venues[i].location << "\n";
        cout << "Capacity: " << venues[i].capacity << "\n";
        cout << "Price: RM" << venues[i].price << "\n";
        cout << "\n";
    }
}

// Main Menu
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
            case 3:
                cout << "👋 Goodbye!\n";
                // Save all data before exiting
                saveDataToFile();
                saveMenuToFile();
                saveVenueToFile();
                break;
            default: cout << "Invalid choice!\n";
        }
    } while (choice != 3);
}

// Main function
int main() {
    // Load data first
    loadVenueFromFile();
    loadMenuFromFile();
    loadDataFromFile();

    cout << "=============================================\n";
    cout << "     Wedding Event Management System\n";
    cout << "=============================================\n";

    mainMenu();
    return 0;
}
