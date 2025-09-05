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
    int id;
    string type; //venue type(eg,garden,beach)
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
    int id;
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
void mainMenu(vector<Event>& events, vector<Menu>& menus, vector<Venue>& venues, int& nextMenuID,int& nextEventID,int& nextVenueID);
void registerUser();
void loginUser(vector<Event>& events, vector<Menu>& menus, vector<Venue>& venues, int& nextMenuID,int& nextEventID,int& nextVenueID);
void userMenu(const string& username, vector<Event>& events, vector<Menu>& menus, vector<Venue>& venues, int& nextMenuID,int& nextEventID,int& nextVenueID);
void adminMenu(vector<Event>& events, vector<Menu>& menus, vector<Venue>& venues, int& nextMenuID,int& nextVenueID);
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
void loadVenueFromFile(vector<Venue>& venues,int& nextVenueID);
void registerEvent(const string& username, vector<Event>& events, const vector<Menu>& menus, const vector<Venue>& venues,int& nextEventID);
void retrieveEvents(const vector<Event>& events);
void createMenu(vector<Menu>& menus, int& nextMenuID);
void viewMenus(const vector<Menu>& menus);
void customizeMenu(Event& e, vector<Event>& event, vector<Menu>& menus, int& nextMenuID);
void createVenue(vector<Venue>& venues,int& nextVenueID);
void viewVenues(const vector<Venue>& venues);

void userMenu(const string& username, vector<Event>& events, vector<Menu>& menus, vector<Venue>& venues, int& nextMenuID,int& nextEventID) {
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
                registerEvent(username, events, menus, venues, nextEventID);
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

void adminMenu(vector<Event>& events, vector<Menu>& menus, vector<Venue>& venues, int& nextMenuID,int& nextVenueID) {
    int choice;
    do {
        cout << "\n===== Organizer Page (Admin) =====\n";
        cout << "1. Create venue\n";
        cout << "2. Create menu\n";
        cout << "3. View events\n";
        cout << "4. View venues\n";
        cout << "5. View menus\n";
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
                createVenue(venues, nextVenueID);
                break;
            case 2:
                createMenu(menus, nextMenuID);
                break;
            case 3:
                retrieveEvents(events);
                cout << "Press Enter to return to Admin Menu...";
                cin.get();
                break;
            case 4:
                viewVenues(venues);
                cout << "Press Enter to return to Admin Menu...";
                cin.get();
                break;
            case 5:
                viewMenus(menus);
                cout << "Press Enter to return to Admin Menu...";
                cin.get();
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
void loginUser(vector<Event>& events, vector<Menu>& menus, vector<Venue>& venues, int& nextMenuID, int& nextEventID,int& nextVenueID) {
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
        adminMenu(events, menus, venues, nextMenuID, nextVenueID);
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
            userMenu(fileUser, events, menus, venues, nextMenuID,nextEventID);
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
            existingEvent.venue.type == e.venue.type &&
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

    for (const auto& e : events) {
        file << e.id << ";"
             << e.customer << ";"
             << dateToString(e.date) << ";"
             << e.venue.type << ";"
             << e.venue.location << ";"
             << e.venue.capacity << ";"
             << e.venue.price << ";"
             << e.venue.timeslot << ";"
             << e.theme << ";"
             << e.servingStyle << ";";

        // Equipment (name:detail pairs)
        for (int i = 0; i < e.equipmentcount; i++) {
            file << e.equipment[i].name << ":" << e.equipment[i].detail;
            if (i < e.equipmentcount - 1) file << ";";
        }
        file << ";";

        // Menu
        file << e.menu.id << ";"
             << e.menu.cuisine << ";"
             << e.menu.price << ";";

        for (size_t i = 0; i < e.menu.foodItems.size(); i++) {
            file << e.menu.foodItems[i];
            if (i < e.menu.foodItems.size() - 1) file << ";";
        }
        file << ";";

        // Customizations
        for (size_t i = 0; i < e.customizations.size(); i++) {
            file << e.customizations[i];
            if (i < e.customizations.size() - 1) file << ";";
        }

        file << "\n"; // one line per event
    }

    file.close();
    cout << "Events saved successfully.\n";
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
        trim(line);
        if (line.empty()) continue;

        vector<string> tokens;
        string token;
        stringstream ss(line);

        while (getline(ss, token, ';')) {
            tokens.push_back(token);
        }

        if (tokens.size() < 14) { // need at least up to menu.price
            cout << "Invalid event format: " << line << endl;
            continue;
        }

        Event e;
        int index = 0;

        e.id = stoi(tokens[index++]);
        e.customer = tokens[index++];
        e.date = parseDate(tokens[index++]);
        e.venue.type = tokens[index++];
        e.venue.location = tokens[index++];
        e.venue.capacity = stoi(tokens[index++]);
        e.venue.price = stod(tokens[index++]);
        e.venue.timeslot = tokens[index++];
        e.theme = tokens[index++];
        e.servingStyle = tokens[index++];

        // Equipment
        e.equipmentcount = 0;
        string eqBlock = tokens[index++];
        if (!eqBlock.empty()) {
            stringstream eqStream(eqBlock);
            string eqToken;
            while (getline(eqStream, eqToken, ',')) {
                size_t pos = eqToken.find(':');
                if (pos != string::npos && e.equipmentcount < max_equipment) {
                    e.equipment[e.equipmentcount].name = eqToken.substr(0, pos);
                    e.equipment[e.equipmentcount].detail = eqToken.substr(pos + 1);
                    e.equipmentcount++;
                }
            }
        }

        // Menu
        e.menu.id = stoi(tokens[index++]);
        e.menu.cuisine = tokens[index++];
        e.menu.price = stod(tokens[index++]);

        // Food items
        if (index < tokens.size()) {
            stringstream foodStream(tokens[index++]);
            string foodItem;
            while (getline(foodStream, foodItem, ',')) {
                if (!foodItem.empty()) e.menu.foodItems.push_back(foodItem);
            }
        }

        // Customizations
        if (index < tokens.size()) {
            stringstream custStream(tokens[index++]);
            string custItem;
            while (getline(custStream, custItem, ',')) {
                if (!custItem.empty()) e.customizations.push_back(custItem);
            }
        }

        events.push_back(e);
    }

    file.close();
    cout << "Events loaded successfully. " << events.size() << " events available.\n";
}




void saveMenuToFile(const vector<Menu>& menus) {
    ofstream file("menus.txt");
    if (!file.is_open()) {
        cout << "Error: Cannot open menus file for writing!\n";
        return;
    }

    for (const auto& menu : menus) {
        file << menu.id << ";"
             << menu.cuisine << ";"
             << menu.price << ";";

        // Join all food items with `;`
        for (size_t i = 0; i < menu.foodItems.size(); i++) {
            file << menu.foodItems[i];
            if (i < menu.foodItems.size() - 1) file << ";";
        }
        file << "\n"; // one line per menu
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
        if (line.empty()) continue;

        vector<string> tokens;
        string token;
        stringstream ss(line);

        while (getline(ss, token, ';')) {
            tokens.push_back(token);
        }

        if (tokens.size() < 3) {
            cout << "Invalid menu format: " << line << endl;
            continue;
        }

        Menu m;
        try {
            m.id = stoi(tokens[0]);
        } catch (...) {
            cout << "Invalid menu ID: " << tokens[0] << endl;
            continue;
        }

        if (m.id >= nextMenuID) nextMenuID = m.id + 1;

        m.cuisine = tokens[1];

        try {
            m.price = stod(tokens[2]);
        } catch (...) {
            cout << "Invalid price for menu ID " << m.id << endl;
            continue;
        }

        // Remaining tokens = food items
        m.foodItems.clear();
        for (size_t i = 3; i < tokens.size(); i++) {
            if (!tokens[i].empty()) m.foodItems.push_back(tokens[i]);
        }

        menus.push_back(m);
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
        file << v.id << ";"
             << v.type << ";"
             << v.location << ";"
             << v.capacity << ";"
             << v.price << ";"
             << v.timeslot << "\n"; // single line per venue
    }

    file.close();
    cout << "Venues saved successfully.\n";
}


void loadVenueFromFile(vector<Venue>& venues, int& nextVenueID) {
    ifstream file("venues.txt");
    if (!file.is_open()) {
        cout << "No existing venues file found.\n";
        return;
    }

    venues.clear();
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        Venue v;
        string idStr, capacityStr, priceStr;

        if (!getline(ss, idStr, ';')) continue;
        if (!getline(ss, v.type, ';')) continue;
        if (!getline(ss, v.location, ';')) continue;
        if (!getline(ss, capacityStr, ';')) continue;
        if (!getline(ss, priceStr, ';')) continue;
        if (!getline(ss, v.timeslot, ';')) continue;

        try {
            v.id = stoi(idStr);
            v.capacity = stoi(capacityStr);
            v.price = stod(priceStr);
        } catch (...) {
            cout << "Error: invalid number in venue line -> " << line << endl;
            continue;
        }

        venues.push_back(v);
        if (v.id >= nextVenueID) nextVenueID = v.id + 1;
    }

    file.close();
    cout << "Venues loaded successfully. " << venues.size() << " venues available.\n";
}



void registerEvent(const string& username, vector<Event>& events, const vector<Menu>& menus, const vector<Venue>& venues,int& nextEventID) {
    Event e;
    e.id = nextEventID++;
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
    bool validVenue = false;
    string chosenSlot;

    while (!validVenue) {
        cout << "\nAvailable Venues:\n";
        for (const auto& v : venues) {
            cout << "[" << v.id << "] "
                 << " | Venue: " << v.type
                 << " | Location: " << v.location
                 << " | Capacity: " << v.capacity
                 << " | Price: RM" << v.price << "\n";
        }

        int venueID;
        Venue chosenVenue;
        bool found = false;

        while (true) {
            cout << "Enter venue ID: ";
            if (cin >> venueID) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                for (const auto& v : venues) {
                    if (v.id == venueID) {
                        chosenVenue = v;
                        found = true;
                        break;
                    }
                }
                if (found) break;
                cout << "Invalid venue ID! Try again.\n";
            } else {
                cout << "Invalid input! Please enter a number.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }

        e.venue = chosenVenue;

        // Now ask for slot
        int slotChoice;
        while (true) {
            cout << "Choose slot (1=Morning (10:00 - 14:00), 2=Evening (18:00 - 22:00): ";
            if (cin >> slotChoice) {
                if (slotChoice == 1 || slotChoice == 2) {
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
            cout << "Error: This venue and time slot is already booked!\n"
                 << "Please choose a different venue or time slot.\n";
            validVenue = false;
        } else {
            validVenue = true;
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
        cout << "Event id : " << events[i].id << "\n";
        cout << "Customer: " << events[i].customer << "\n";
        cout << "Date: " << dateToString(events[i].date) << "\n";
        cout << "Venue: " << events[i].venue.type << "\n";
        cout << "Location : " << events[i].venue.location << "\n";
        cout << "Capacity: " << events[i].venue.capacity << "\n";
        cout << "Time Slot: " << events[i].venue.timeslot << "\n";
        cout << "Theme: " << events[i].theme << "\n";
        cout << "Serving Style: " << events[i].servingStyle << "\n";

        cout << "Equipment: ";
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
            cout <<events[i].menu.cuisine<< "Cuisine :"
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

void createVenue(vector<Venue>& venues,int& nextVenueID) {
    Venue v;
    v.id = nextVenueID++;
    cout << "\n=== Create Venue ===\n";

    while (true) {
        cout << "Enter venue type (Enter 0 to cancel) : ";
        getline(cin, v.type);
        trim(v.type);
        if (v.type == "0") return;
        if (!v.type.empty()) break;
        cout << "Venue type cannot be empty! Please try again.\n";
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
        cout << "Enter venue capacity : ";
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
        cout << "Enter price for venue rental: RM";
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
        cout << " | Venue ID : " << v.id
             << " | Venue Type : " << v.type
             << " | Location: " << v.location
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
        cout << " | Venue ID: " << v.id
             << " | Venue Type :  " << v.type
             << " | Location: " << v.location
             << " | Capacity: " << v.capacity
             << " | Price: RM" << v.price
             << "\n";
    }
}

void mainMenu(vector<Event>& events, vector<Menu>& menus, vector<Venue>& venues, int& nextMenuID,int& nextEventID,int& nextVenueID) {
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
            case 2: loginUser(events, menus, venues, nextMenuID, nextEventID,nextVenueID); break;
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
    int nextVenueID = 2001;
    int nextEventID = 1001;

    loadVenueFromFile(venues, nextVenueID);
    loadMenuFromFile(menus, nextMenuID);
    loadDataFromFile(events);

    system("CLS");

    cout << "=============================================\n";
    cout << "     Wedding Event Management System\n";
    cout << "=============================================\n";

    mainMenu(events, menus, venues, nextMenuID, nextEventID,nextVenueID);
    return 0;
}
