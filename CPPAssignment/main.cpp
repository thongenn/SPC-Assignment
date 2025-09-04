#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>
#include <ctime>
#include <limits>
using namespace std;

struct User {
    string username;
    string password;
    string phone;
};

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
    Equipment equipment[10];
    int equipmentcount;
    Menu menu;
    vector<string> customizations;
};

const int max_equipment = 10;

// Forward declarations
void mainMenu(vector<Event>& events, vector<Menu>& menus, vector<Venue>& venues, int& nextMenuID);
void registerUser();
void loginUser(vector<Event>& events, vector<Menu>& menus, vector<Venue>& venues, int& nextMenuID);
void userMenu(const string& username, vector<Event>& events, vector<Menu>& menus, vector<Venue>& venues, int& nextMenuID);
void adminMenu(vector<Event>& events, vector<Menu>& menus, vector<Venue>& venues, int& nextMenuID);
bool validatePhone(const string& phone);
bool validatePassword(const string& password);

void trim(string& str);
Date getTodayDate();
Date parseDate(const string& input);
string dateToString(const Date& d);
bool isBefore(const Date& a, const Date& b);
bool isValidDate(int year, int month, int day);
bool hasConflict(const Event& e, const vector<Event>& events);
void saveDataToFile(const vector<Event>& events);
void loadDataFromFile(vector<Event>& events);
void saveMenuToFile(const vector<Menu>& menus);
void loadMenuFromFile(vector<Menu>& menus, int& nextMenuID);
void saveVenueToFile(const vector<Venue>& venues);
void loadVenueFromFile(vector<Venue>& venues);
void registerEvent(const string& username, vector<Event>& events, const vector<Menu>& menus, const vector<Venue>& venues);
void retrieveEvents(const vector<Event>& events);
void createMenu(vector<Menu>& menus, int& nextMenuID);
void viewMenus(const vector<Menu>& menus);
void customizeMenu(Event& e, vector<Event>& event, vector<Menu>& menus, int& nextMenuID);
void createVenue(vector<Venue>& venues);
void viewVenues(const vector<Venue>& venues);

void userMenu(const string& username, vector<Event>& events, vector<Menu>& menus, vector<Venue>& venues, int& nextMenuID) {
    int choice;
    do {
        cout << "\n===== User Page (" << username << ") =====\n";
        cout << "1. Register Wedding Event\n";
        cout << "2. Customize Food & Menu Packages\n";
        cout << "3. Make Payment & Checkout\n";
        cout << "4. View Receipt\n";
        cout << "5. Logout\n";

        // Input validation for choice
        while (true) {
            cout << "Enter your choice: ";
            if (cin >> choice) {
                if (choice >= 1 && choice <= 5) {
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;
                } else {
                    cout << "Invalid choice! Please enter a number between 1-5.\n";
                }
            } else {
                cout << "Invalid input! Please enter a number.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }

        switch (choice) {
            case 1:
                registerEvent(username, events, menus, venues);
                break;
            case 2:
                {
                    Event* userEvent = nullptr;
                    for (auto& event : events) {
                        if (event.customer == username) {
                            userEvent = &event;
                            break;
                        }
                    }
                    if (userEvent) {
                        customizeMenu(*userEvent, events, menus, nextMenuID);
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
        }
    } while (choice != 5);
}

void adminMenu(vector<Event>& events, vector<Menu>& menus, vector<Venue>& venues, int& nextMenuID) {
    int choice;
    do {
        cout << "\n===== Organizer Page (Admin) =====\n";
        cout << "1. Create venue\n";
        cout << "2. Create Menu\n";
        cout << "3. View Events\n";
        cout << "4. View Venues\n";
        cout << "5. View Menus\n";
        cout << "6. Logout\n";

        // Input validation for choice
        while (true) {
            cout << "Enter your choice: ";
            if (cin >> choice) {
                if (choice >= 1 && choice <= 6) {
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;
                } else {
                    cout << "Invalid choice! Please enter a number between 1-6.\n";
                }
            } else {
                cout << "Invalid input! Please enter a number.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }

        switch (choice) {
            case 1:
                cout << "Create venue\n";
                createVenue(venues);
                break;
            case 2:
                cout << "Create Menu\n";
                createMenu(menus, nextMenuID);
                break;
            case 3:
                cout << "Viewing events...\n";
                retrieveEvents(events);
                break;
            case 4:
                cout << "Viewing venues...\n";
                viewVenues(venues);
                cout << "Press Enter to return to Admin Menu...";
                cin.ignore();
                break;
            case 5:
                cout << "Viewing menus...\n";
                viewMenus(menus);
                cout << "Press Enter to return to Admin Menu...";
                cin.ignore();
                break;
            case 6:
                cout << "Logging out...\n";
                break;
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
    outFile << newUser.username << "|" << newUser.password << "|" << newUser.phone << endl;
    outFile.close();

    cout << "Registration successful!\n";
}

// login
void loginUser(vector<Event>& events, vector<Menu>& menus, vector<Venue>& venues, int& nextMenuID) {
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
        adminMenu(events, menus, venues, nextMenuID);
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
            userMenu(fileUser, events, menus, venues, nextMenuID);
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

bool isValidDate(int year, int month, int day) {
    if (year < 1900 || year > 2100) return false;
    if (month < 1 || month > 12) return false;

    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
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

bool hasConflict(const Event& e, const vector<Event>& events) {
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

void trim(string& str) {
    size_t start = str.find_first_not_of(" \t\n\r\f\v");
    if (start != string::npos) {
        str = str.substr(start);
    }

    size_t end = str.find_last_not_of(" \t\n\r\f\v");
    if (end != string::npos) {
        str = str.substr(0, end + 1);
    }

    if (str.empty()) {
        str = "";
    }
}

// Save all events to file
void saveDataToFile(const vector<Event>& events) {
    ofstream file("data.txt");
    if (!file.is_open()) {
        cout << "Error: Cannot open data file for writing!\n";
        return;
    }

    for (const auto& event : events) {
        file << event.customer << "\n";
        file << event.date.year << " " << event.date.month << " " << event.date.day << "\n";

        file << event.venue.hall << "\n";
        file << event.venue.location << "\n";
        file << event.venue.capacity << "\n";
        file << event.venue.price << "\n";
        file << event.venue.timeslot << "\n";

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

        file << "---\n"; // separator between events
    }

    file.close();
}

// Load all events from file
void loadDataFromFile(vector<Event>& events) {
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
        trim(event.customer);

        // Read date
        if (!getline(file, line)) break;
        istringstream dateStream(line);
        if (!(dateStream >> event.date.year >> event.date.month >> event.date.day)) {
            cout << "Error reading date for event: " << event.customer << endl;
            continue;
        }

        // Read venue details - each on separate lines
        if (!getline(file, line)) break;
        try {
            event.venue.hall = stoi(line);
        } catch (const invalid_argument&) {
            cout << "Error reading hall number for event: " << event.customer << endl;
            continue;
        }

        if (!getline(file, event.venue.location)) break;
        trim(event.venue.location);

        if (!getline(file, line)) break;
        try {
            event.venue.capacity = stoi(line);
        } catch (const invalid_argument&) {
            cout << "Error reading capacity for event: " << event.customer << endl;
            continue;
        }

        if (!getline(file, line)) break;
        try {
            event.venue.price = stod(line);
        } catch (const invalid_argument&) {
            cout << "Error reading price for event: " << event.customer << endl;
            continue;
        }

        if (!getline(file, event.venue.timeslot)) break;
        trim(event.venue.timeslot);

        // Read theme
        if (!getline(file, event.theme)) break;
        trim(event.theme);

        // Read serving style
        if (!getline(file, event.servingStyle)) break;
        trim(event.servingStyle);

        // Read equipment count
        if (!getline(file, line)) break;
        try {
            event.equipmentcount = stoi(line);
        } catch (const invalid_argument&) {
            cout << "Invalid equipment count for event: " << event.customer << endl;
            continue;
        }

        // Read equipment
        for (int i = 0; i < event.equipmentcount; i++) {
            if (!getline(file, event.equipment[i].name)) break;
            trim(event.equipment[i].name);

            if (!getline(file, event.equipment[i].detail)) break;
            trim(event.equipment[i].detail);
        }

        // Read menu ID
        if (!getline(file, line)) break;
        try {
            event.menu.id = stoi(line);
        } catch (const invalid_argument&) {
            cout << "Invalid menu ID for event: " << event.customer << endl;
            continue;
        }

        // Read cuisine
        if (!getline(file, event.menu.cuisine)) break;
        trim(event.menu.cuisine);

        // Read menu price
        if (!getline(file, line)) break;
        try {
            event.menu.price = stod(line);
        } catch (const invalid_argument&) {
            cout << "Invalid menu price for event: " << event.customer << endl;
            continue;
        }

        // Read number of food items
        if (!getline(file, line)) break;
        int foodCount;
        try {
            foodCount = stoi(line);
        } catch (const invalid_argument&) {
            cout << "Invalid food count for event: " << event.customer << endl;
            continue;
        }

        // Read food items
        event.menu.foodItems.clear();
        for (int i = 0; i < foodCount; i++) {
            if (!getline(file, line)) break;
            trim(line);
            if (!line.empty()) {
                event.menu.foodItems.push_back(line);
            }
        }

        // Read number of customizations
        if (!getline(file, line)) break;
        int customCount;
        try {
            customCount = stoi(line);
        } catch (const invalid_argument&) {
            cout << "Invalid customization count for event: " << event.customer << endl;
            continue;
        }

        // Read customizations
        event.customizations.clear();
        for (int i = 0; i < customCount; i++) {
            if (!getline(file, line)) break;
            trim(line);
            if (!line.empty()) {
                event.customizations.push_back(line);
            }
        }

        events.push_back(event);

        // Read the separator line
        getline(file, line);
    }

    file.close();
    cout << "Data loaded successfully. " << events.size() << " events loaded.\n";
}

void saveMenuToFile(const vector<Menu>& menus) {
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
        file << "---\n";
    }
    file.close();
    cout << "Menus saved successfully.\n";
}

void loadMenuFromFile(vector<Menu>& menus, int& nextMenuID) {
    ifstream file("menus.txt");
    if (!file.is_open()) {
        cout << "No existing menus file found. Starting fresh.\n";
        return;
    }

    menus.clear();
    string line;
    while (getline(file, line)) {
        trim(line);
        if (line.empty() || line == "---") continue;

        Menu m;
        try {
            m.id = stoi(line);
        } catch (...) {
            cout << "Invalid menu ID in file: '" << line << "' — skipping menu.\n";
            continue;
        }
        if (m.id >= nextMenuID) nextMenuID = m.id + 1;

        if (!getline(file, m.cuisine)) break;
        trim(m.cuisine);

        if (!getline(file, line)) break;
        trim(line);
        try {
            m.price = stod(line);
        } catch (...) {
            cout << "Invalid price in file for menu ID " << m.id << " — skipping menu.\n";
            continue;
        }

        int foodCount = 0;
        if (!(file >> foodCount)) {
            cout << "Invalid food count for menu ID " << m.id << " — skipping menu.\n";
            file.clear();
            file.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        file.ignore(); // discard newline

        m.foodItems.clear();
        for (int i = 0; i < foodCount; i++) {
            if (!getline(file, line)) break;
            trim(line);
            if (!line.empty()) m.foodItems.push_back(line);
        }

        menus.push_back(m);

        // Read and discard separator if present
        if (getline(file, line)) {
            trim(line);
            if (!line.empty() && line != "---") {
                // If it's not the separator, put it back into the stream
                file.seekg(-static_cast<int>(line.size()) - 1, ios::cur);
            }
        }
    }

    file.close();
    cout << "Menus loaded successfully. " << menus.size() << " menus available.\n";
}

void saveVenueToFile(const vector<Venue>& venues) {
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
             << v.timeslot << "\n"
             << "---\n";
    }

    file.close();
    cout << "Venues saved successfully.\n";
}

void loadVenueFromFile(vector<Venue>& venues) {
    ifstream file("venues.txt");
    if (!file.is_open()) {
        cout << "No existing venues file found. Starting fresh.\n";
        return;
    }

    venues.clear();
    string line;
    while (getline(file, line)) {
        trim(line);
        if (line.empty() || line == "---") continue;

        Venue v;
        try {
            v.hall = stoi(line);
        } catch (...) {
            cout << "Invalid hall number in file: '" << line << "' — skipping venue.\n";
            continue;
        }

        if (!getline(file, v.location)) break;
        trim(v.location);

        if (!getline(file, line)) break;
        trim(line);
        try {
            v.capacity = stoi(line);
        } catch (...) {
            cout << "Invalid capacity in file for hall " << v.hall << " — skipping venue.\n";
            continue;
        }

        if (!getline(file, line)) break;
        trim(line);
        try {
            v.price = stod(line);
        } catch (...) {
            cout << "Invalid price in file for hall " << v.hall << " — skipping venue.\n";
            continue;
        }

        if (!getline(file, v.timeslot)) break;
        trim(v.timeslot);

        // Read and discard separator if present
        if (file.peek() != EOF) getline(file, line);

        venues.push_back(v);
    }

    file.close();
    cout << "Venues loaded successfully. " << venues.size() << " venues available.\n";
}

void registerEvent(const string& username, vector<Event>& events, const vector<Menu>& menus, const vector<Venue>& venues) {
    Event e;
    e.customer = username;

    if (venues.empty()) {
        cout << "No venues available! Please contact admin to create venues first.\n";
        return;
    }

    Date today = getTodayDate();

    // Date validation
    while (true) {
        cout << "Enter event date (yyyy-mm-dd) or 0 to cancel: ";
        string dateInput;
        getline(cin, dateInput);

        if (dateInput == "0") {
            cout << "Event registration cancelled.\n";
            return;
        }

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
        while (true) {
            cout << "Enter hall number: ";
            if (cin >> chosenHall) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            } else {
                cout << "Invalid input! Please enter a number.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }

        bool hallFound = false;
        for (const auto& v : venues) {
            if (v.hall == chosenHall) {
                e.venue = v;
                hallFound = true;
                break;
            }
        }

        if (!hallFound) {
            cout << "Invalid hall number. Try again.\n";
            continue; // go back to hall selection
        }

        // Now ask for slot only if hall is valid
        int slotChoice;
        while (true) {
            cout << "Choose slot (1=Morning (10:00 - 14:00) , 2=Evening (18:00 - 22:00): ";
            if (cin >> slotChoice) {
                if (slotChoice >= 1 && slotChoice <= 2) {
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;
                } else {
                    cout << "Invalid slot selection. Please enter 1 or 2.\n";
                }
            } else {
                cout << "Invalid input! Please enter a number.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }

        chosenSlot = (slotChoice == 1) ? "Morning" : "Evening";
        e.venue.timeslot = chosenSlot;

        // Check for conflicts
        if (hasConflict(e, events)) {
            cout << "Error: This venue and time slot is already booked! "
                 << "Please choose a different hall or time slot.\n";
            validHall = false;
        } else {
            validHall = true;
        }
    }


    // Choose theme
    int themeChoice;
    while (true) {
        cout << "Select theme: \n";
        cout << "1. Traditional Chinese\n";
        cout << "2. Traditional Indian\n";
        cout << "3. Traditional Malay\n";
        cout << "4. Western\n";
        cout << "5. Italian\n";
        cout << "6. Others\n";
        cout << "Enter choice: ";

        if (cin >> themeChoice) {
            if (themeChoice >= 1 && themeChoice <= 6) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            } else {
                cout << "Invalid choice! Please enter a number between 1-6.\n";
            }
        } else {
            cout << "Invalid input! Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    switch (themeChoice) {
        case 1: e.theme = "Traditional Chinese"; break;
        case 2: e.theme = "Traditional Indian"; break;
        case 3: e.theme = "Traditional Malay"; break;
        case 4: e.theme = "Western"; break;
        case 5: e.theme = "Italian"; break;
        case 6:
            while (true) {
                cout << "Enter custom theme: ";
                getline(cin, e.theme);
                trim(e.theme);
                if (!e.theme.empty()) break;
                cout << "Theme cannot be empty! Please try again.\n";
            }
            break;
    }

    // Serving style
    int styleChoice;
    while (true) {
        cout << "Select serving style:\n1. Buffet\n2. Plated\nEnter choice: ";
        if (cin >> styleChoice) {
            if (styleChoice == 1 || styleChoice == 2) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            } else {
                cout << "Invalid choice! Please enter 1 or 2.\n";
            }
        } else {
            cout << "Invalid input! Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    e.servingStyle = (styleChoice == 1) ? "Buffet" : "Plated";

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

        // Show current selections
        if (e.equipmentcount > 0) {
            cout << "\nCurrent selections:\n";
            for (int i = 0; i < e.equipmentcount; i++) {
                cout << "- " << e.equipment[i].name << " (" << e.equipment[i].detail << ")\n";
            }
        }

        // Equipment choice validation
        while (true) {
            cout << "Enter your choice: ";
            if (cin >> choice) {
                if (choice >= 0 && choice <= availableEquipment.size()) {
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;
                } else {
                    cout << "Invalid choice! Please enter a number between 0-" << availableEquipment.size() << ".\n";
                }
            } else {
                cout << "Invalid input! Please enter a number.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }

        if (choice == 0) break;

        string selectedEquipment = availableEquipment[choice - 1];

        // Check if this equipment type is already selected
        int existingIndex = -1;
        for (int i = 0; i < e.equipmentcount; i++) {
            if (e.equipment[i].name == selectedEquipment) {
                existingIndex = i;
                break;
            }
        }

        // Equipment detail
        string detail;
        while (true) {
            cout << "Enter detail/needs for " << selectedEquipment << ": ";
            getline(cin, detail);
            trim(detail);
            if (!detail.empty()) break;
            cout << "Detail cannot be empty! Please try again.\n";
        }

        if (existingIndex != -1) {
            // Update existing equipment
            e.equipment[existingIndex].detail = detail;
            cout << "Updated: " << selectedEquipment << " (" << detail << ")\n";
        } else {
            // Add new equipment
            if (e.equipmentcount >= max_equipment) {
                cout << "You have reached the maximum number of equipment ("
                    << max_equipment << ").\n";
                break;
            }
            e.equipment[e.equipmentcount].name = selectedEquipment;
            e.equipment[e.equipmentcount].detail = detail;
            e.equipmentcount++;
            cout << "Added: " << selectedEquipment << " (" << detail << ")\n";
        }
    }

    // Confirmation
    char confirm;
    while (true) {
        cout << "Confirm event registration? (y/n): ";
        cin >> confirm;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (confirm == 'y' || confirm == 'Y' || confirm == 'n' || confirm == 'N') {
            break;
        }
        cout << "Invalid input! Please enter 'y' or 'n'.\n";
    }

    if (confirm == 'y' || confirm == 'Y') {
        events.push_back(e);
        saveDataToFile(events);
        cout << "Event created successfully.\n";
    } else {
        cout << "Event registration cancelled.\n";
    }
}

void retrieveEvents(const vector<Event>& events) {
    if (events.empty()) {
        cout << "No events found.\n";
        return;
    }

    cout << "\n--- Available Events ---\n";
    for (int i = 0; i < events.size(); i++) {
        cout << "Event " << i + 1 << "\n";
        cout << "Customer: " << events[i].customer << "\n";
        cout << "Date: " << dateToString(events[i].date) << "\n";
        cout << "Venue: Hall " << events[i].venue.hall
             << " (" << events[i].venue.location << ")\n";
        cout << "Capacity: " << events[i].venue.capacity << "\n";
        cout << "Time Slot: " << events[i].venue.timeslot << "\n";
        cout << "Theme: " << events[i].theme << "\n";
        cout << "Serving Style: " << events[i].servingStyle << "\n";

        cout << "Equipment: \n";
        if (events[i].equipmentcount == 0) {
            cout << "None";
        } else {
            for (int k = 0; k < events[i].equipmentcount; k++) {
                cout << events[i].equipment[k].name
                     << " (" << events[i].equipment[k].detail << ")";
                if (k < events[i].equipmentcount - 1) cout << "\n";
            }
        }
        cout << "\n";

        cout << "Menu: ";
        if (events[i].menu.cuisine.empty()) {
            cout << "Not customized yet\n";
        } else {
            cout << "\n  Cuisine: " << events[i].menu.cuisine
                 << "\n  Food Items: \n  ";
            for (int j = 0; j < events[i].menu.foodItems.size(); j++) {
                cout << events[i].menu.foodItems[j];
                if (j < events[i].menu.foodItems.size() - 1) cout << "\n  ";
            }
            cout << "\n";
        }

        if (!events[i].customizations.empty()) {
            cout << "Customizations: ";
            for (int j = 0; j < events[i].customizations.size(); j++) {
                cout << events[i].customizations[j];
                if (j < events[i].customizations.size() - 1) cout << ", ";
            }
            cout << "\n";
        }

        cout << "\n";  // spacing between events
    }
}

void createMenu(vector<Menu>& menus, int& nextMenuID) {
    Menu m;
    m.id = nextMenuID++;

    // Cuisine choice (with cancel option)
    int opt;
    while (true) {
        cout << "Select cuisine type (Enter 0 to cancel)\n"
             << "1. Chinese\n2. Indian\n3. Malay\n4. Western\n5. Italian\n"
             << "Enter choice: ";
        if (cin >> opt) {
            if (opt == 0) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return;
            }
            if (opt >= 1 && opt <= 5) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            } else {
                cout << "Invalid choice! Please enter a number between 1-5 or 0 to cancel.\n";
            }
        } else {
            cout << "Invalid input! Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    switch (opt) {
        case 1: m.cuisine = "Chinese"; break;
        case 2: m.cuisine = "Indian"; break;
        case 3: m.cuisine = "Malay"; break;
        case 4: m.cuisine = "Western"; break;
        case 5: m.cuisine = "Italian"; break;
    }

    cout << "\nEnter food items for " << m.cuisine << " cuisine.\n";
    string item;

    while (true) {
        cout << "Enter food item (type 'done' to finish): ";
        getline(cin, item);
        trim(item);

        if (item == "done" || item == "DONE") {
            if (!m.foodItems.empty()) break;
            cout << "You must enter at least one food item before finishing.\n";
            continue;
        }
        if (!item.empty()) {
            m.foodItems.push_back(item);
            cout << "Added: " << item << endl;
        } else {
            cout << "Food item cannot be empty! Please try again.\n";
        }
    }

    // Price validation
    while (true) {
        cout << "Enter price per table (10 ppl) for this menu: RM";
        if (cin >> m.price) {
            if (m.price >= 200) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            } else {
                cout << "Price must be at least RM200! Please try again.\n";
            }
        } else {
            cout << "Invalid input! Please enter a valid number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    // Confirmation
    char confirm;
    while (true) {
        cout << "Confirm menu creation? (y/n): ";
        cin >> confirm;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (confirm == 'y' || confirm == 'Y' || confirm == 'n' || confirm == 'N') {
            break;
        }
        cout << "Invalid input! Please enter 'y' or 'n'.\n";
    }

    if (confirm == 'y' || confirm == 'Y') {
        menus.push_back(m);
        saveMenuToFile(menus);
        cout << "\nMenu created successfully!\n";
        cout << "ID: " << m.id << " | Cuisine: " << m.cuisine
             << " | Price: RM" << m.price << "\n";
    } else {
        cout << "Menu creation cancelled.\n";
    }
}


void viewMenus(const vector<Menu>& menus) {
    if (menus.empty()) {
        cout << "No menus available.\n";
        return;
    }

    cout << "\n--- Available Menus ---\n";
    for (int i = 0; i < menus.size(); i++) {
        cout << "Menu: " << menus[i].id << "\n";
        cout << "Cuisine: " << menus[i].cuisine << "\n";
        cout << "Price per table (10 people): RM" << menus[i].price << "\n";
        cout << "Food Items: ";
        for (int j = 0; j < menus[i].foodItems.size(); j++) {
            cout << menus[i].foodItems[j];
            if (j < menus[i].foodItems.size() - 1) cout << ", ";
        }
        cout << "\n\n";
    }
}

void customizeMenu(Event& e, vector<Event>& events, vector<Menu>& menus, int& nextMenuID) {
    if (menus.empty()) {
        cout << "No menus available. Please ask admin to create a menu first.\n";
        return;
    }

    cout << "\nAvailable Menus:\n";
    for (int i = 0; i < menus.size(); i++) {
        cout << i + 1 << ". " << menus[i].cuisine << " (RM" << menus[i].price << " per table)\n";
        cout << "   Food Items: ";
        for (int j = 0; j < menus[i].foodItems.size(); j++) {
            cout << menus[i].foodItems[j];
            if (j < menus[i].foodItems.size() - 1) cout << ", ";
        }
        cout << "\n\n";
    }

    // Menu choice validation
    int menuChoice;
    while (true) {
        cout << "Select a menu you want for event (0 to cancel): ";
        if (cin >> menuChoice) {
            if (menuChoice >= 0 && menuChoice <= menus.size()) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            } else {
                cout << "Invalid selection! Please enter a number between 0-" << menus.size() << ".\n";
            }
        } else {
            cout << "Invalid input! Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    if (menuChoice == 0) {
        cout << "Menu selection cancelled.\n";
        return;
    }

    e.menu = menus[menuChoice - 1];

    // Customization choice validation
    char customizationChoice;
    while (true) {
        cout << "Do you have any dietary restrictions or special requests? (y/n): ";
        cin >> customizationChoice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (customizationChoice == 'y' || customizationChoice == 'Y' ||
            customizationChoice == 'n' || customizationChoice == 'N') {
            break;
        }
        cout << "Invalid input! Please enter 'y' or 'n'.\n";
    }

    if (customizationChoice == 'y' || customizationChoice == 'Y') {
        cout << "Enter your dietary restrictions or special requests (type 'done' when finished):\n";
        string custom;
        while (true) {
            getline(cin, custom);
            trim(custom);
            if (custom == "done" || custom == "DONE") break;
            if (!custom.empty()) {
                e.customizations.push_back(custom);
            } else {
                cout << "Customization cannot be empty! Please try again.\n";
            }
        }
    }

    // Confirmation
    char confirm;
    while (true) {
        cout << "Confirm menu customization? (y/n): ";
        cin >> confirm;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (confirm == 'y' || confirm == 'Y' || confirm == 'n' || confirm == 'N') {
            break;
        }
        cout << "Invalid input! Please enter 'y' or 'n'.\n";
    }

    if (confirm == 'y' || confirm == 'Y') {
        saveDataToFile(events);
        cout << "Menu customized successfully!\n";
    } else {
        cout << "Menu customization cancelled.\n";
    }
}

void createVenue(vector<Venue>& venues) {
    Venue v;
    cout << "\n=== Create Venue ===\n";

    // Hall number validation + duplicate check
    while (true) {
        cout << "Enter hall number (Enter 0 to cancel) : ";
        if (cin >> v.hall) {
            if (v.hall == 0) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return;
            }

            if (v.hall > 0) {
                // Check duplicate hall number
                bool exists = false;
                for (const auto& existing : venues) {
                    if (existing.hall == v.hall) {
                        exists = true;
                        break;
                    }
                }

                if (exists) {
                    cout << "Hall number already exists! Please try again.\n";
                } else {
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break; // valid and unique hall number
                }
            } else {
                cout << "Hall number must be positive! Please try again.\n";
            }
        } else {
            cout << "Invalid input! Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    // Location validation
    while (true) {
        cout << "Enter location: ";
        getline(cin, v.location);
        trim(v.location);
        if (!v.location.empty()) break;
        cout << "Location cannot be empty! Please try again.\n";
    }

    // Capacity validation (100–1000)
    while (true) {
        cout << "Enter hall capacity : ";
        if (cin >> v.capacity) {
            if (v.capacity >= 100) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            } else {
                cout << "Capacity must be at least 100 ! Please try again.\n";
            }
        } else {
            cout << "Invalid input! Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    // Price validation
    while (true) {
        cout << "Enter price for hall rental: RM";
        if (cin >> v.price) {
            if (v.price >= 1000) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            } else {
                cout << "Price must be at least 1000! Please try again.\n";
            }
        } else {
            cout << "Invalid input! Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    v.timeslot = "Both";

    // Confirmation
    char confirm;
    while (true) {
        cout << "Confirm venue creation? (y/n): ";
        cin >> confirm;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (confirm == 'y' || confirm == 'Y' || confirm == 'n' || confirm == 'N') {
            break;
        }
        cout << "Invalid input! Please enter 'y' or 'n'.\n";
    }

    if (confirm == 'y' || confirm == 'Y') {
        venues.push_back(v);
        saveVenueToFile(venues);
        cout << "\nVenue created successfully!\n";
        cout << "Hall " << v.hall << " | Location: " << v.location
             << " | Capacity: " << v.capacity
             << " | Price: RM" << v.price
             << "\n";
    } else {
        cout << "Venue creation cancelled.\n";
    }
}

void viewVenues(const vector<Venue>& venues) {
    cout << "\n=== Available Venues ===\n";
    if (venues.empty()) {
        cout << "No venues available.\n";
        return;
    }

    for (const auto& v : venues) {
        cout << "Hall " << v.hall
             << " | Location: " << v.location
             << " | Capacity: " << v.capacity
             << " | Price: RM" << v.price
             << "\n";
    }
}

void mainMenu(vector<Event>& events, vector<Menu>& menus, vector<Venue>& venues, int& nextMenuID) {
    int choice;
    do {
        cout << "\n===== Wedding Event Management System =====\n";
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "3. Exit\n";

        while (true) {
            cout << "Enter your choice: ";
            if (cin >> choice) {
                if (choice >= 1 && choice <= 3) {
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;
                } else {
                    cout << "Invalid choice! Please enter a number between 1-3.\n";
                }
            } else {
                cout << "Invalid input! Please enter a number.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }

        switch (choice) {
            case 1: registerUser(); break;
            case 2: loginUser(events, menus, venues, nextMenuID); break;
            case 3:
                cout << "👋 Goodbye!\n";
                saveDataToFile(events);
                saveMenuToFile(menus);
                saveVenueToFile(venues);
                break;
        }
    } while (choice != 3);
}

int main() {
    vector<Event> events;
    vector<Menu> menus;
    vector<Venue> venues;
    int nextMenuID = 1;

    loadVenueFromFile(venues);
    loadMenuFromFile(menus, nextMenuID);
    loadDataFromFile(events);

    system("CLS");

    cout << "=============================================\n";
    cout << "     Wedding Event Management System\n";
    cout << "=============================================\n";

    mainMenu(events, menus, venues, nextMenuID);
    return 0;
}
