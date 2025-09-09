#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>
#include <ctime>
#include <limits>
#include <iomanip>
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

struct Menu {
    int id;
    string cuisine;
    vector<string> foodItems;
    double price;
    string servingStyle;
};

struct Feedback {
    string feedbackId;
    string type;    // Complaint / Suggestion
    int rating;
    string content;
    string status;  // Pending / In Progress / Resolved
    string response;
};

struct Event {
    int id;
    string customer;
    string phone;
    Date date;
    Venue venue;
    string theme;
    int guestCount;
    string evstatus; // "Planned", "Comfirmed", "Started", "Postponed", "Ended", "Canceled"
    string issues;
    string notes; //user special request
    Menu menu;
    vector<string> customizations;
    double totalCost = 0.0;
    bool isPaid = false;
    string paymentMethod;
    vector<Feedback> feedbackList;

};


// Forward declarations
//user function
void registerUser();
void loginUser(vector<Event>& events, vector<Menu>& menus, vector<Venue>& venues, int& nextMenuID,int& nextEventID,int& nextVenueID);
void userMenu(const string& username,const string& phone, vector<Event>& events, vector<Menu>& menus, vector<Venue>& venues, int& nextMenuID,int& nextEventID);
//admin function
void adminMenu(vector<Event>& events, vector<Menu>& menus, vector<Venue>& venues, int& nextMenuID,int& nextVenueID);
void mainMenu(vector<Event>& events, vector<Menu>& menus, vector<Venue>& venues, int& nextMenuID,int& nextEventID,int& nextVenueID);
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
void loadDataFromFile(vector<Event>& events,int& nextEventID);
void saveMenuToFile(const vector<Menu>& menus);
void loadMenuFromFile(vector<Menu>& menus, int& nextMenuID);
void saveVenueToFile(const vector<Venue>& venues);
void loadVenueFromFile(vector<Venue>& venues,int& nextVenueID);
void registerEvent(const string& username,const string& phone,vector<Event>& events, vector<Menu>& menus, const vector<Venue>& venues,int& nextEventID);
void createMenu(vector<Menu>& menus, int& nextMenuID);
void viewMenus(const vector<Menu>& menus);
bool customizeMenu(Event& e, vector<Event>& events, vector<Menu>& menus, int& nextMenuID);
void createVenue(vector<Venue>& venues,int& nextVenueID);
void viewVenues(const vector<Venue>& venues);
double calculateTotalCost(const Event& e);
bool makePayment(Event &e);
void printReceipt(const string &username, const vector<Event> &events);
void eventMenu(vector<Event>& events);
void menuMenu(vector<Menu>& menus, int& nextMenuID);
void venueMenu(vector<Venue>& venues, int& nextVenueID);
void reportMenu(const vector<Event>& events);
void userFeedbackMenu(const string& username, vector<Event>& events);
void staffFeedbackMenu(vector<Event>& events);
// WQ
// user
void submitFeedback(const string& username, vector<Event>& events);
void deleteFeedback(const string& username, vector<Event>& events);
void viewFeedback(const string& username, const vector<Event>& events);
// admin
void updateFeedbackStatus(vector<Event>& events);
void viewAllFeedback(const vector<Event>& events);
void viewSummarizeFeedback(const vector<Event>& events);
void viewAllEvents(const vector<Event>& events);
void generateEventReport(const vector<Event>& events);
void generateMonthlyEventReport (const vector<Event>& events);
void updateEventStatus(vector<Event>& events);
void commentEventIssues(vector<Event>& events);
void checkEventIssues(const vector<Event>& events);
// both
void saveFeedbacks(const vector<Event>& events);
void loadFeedbacks(vector<Event>& events);

void userFeedbackMenu(const string& username, vector<Event>& events) {
    int choice;
    do {
        cout << "\n+=================================================+\n";
        cout << "|              Feedback Management                |\n";
        cout << "+=================================================+\n";
        cout << "| 1. Submit Feedback                              |\n";
        cout << "| 2. View My Feedback & Reply                     |\n";
        cout << "| 3. Delete Feedback                              |\n";
        cout << "| 0. Back to Main Menu                            |\n";
        cout << "+=================================================+\n";

        while (true) {
            cout << "Enter your choice: ";
            if (cin >> choice) {
                if (choice >= 0 && choice <= 3) {
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;
                }
            } else {
                cout << "Invalid input! Please enter a number.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
        switch (choice) {
            case 1:
                submitFeedback(username, events);
                saveFeedbacks(events);
                break;
            case 2:
                viewFeedback(username, events);
                break;
            case 3:
                deleteFeedback(username, events);
                saveFeedbacks(events);
                break;
            case 0:
                return;
            default:
                cout << "Invalid choice. Try again.\n";
        }

    } while (choice != 0);

}
void viewFeedback(const string& username, const vector<Event>& events) {

    cout << "\n+=================================================+\n";
    cout << "|               My Feedback & Reply               |\n";
    cout << "+=================================================+\n";

    bool foundEvent = false;
    bool foundFeedback = false;

    for (const Event &ev : events) {
        if (ev.customer == username && ev.evstatus != "Canceled") {
            foundEvent = true;
            cout << "Event ID: " << ev.id << " | Theme: " << ev.theme << "\n";

            bool hasValidFeedback = false;
            for (const Feedback &fb : ev.feedbackList) {
                if (fb.status != "Deleted") {
                    hasValidFeedback = true;
                    foundFeedback = true;
                    cout << "Feedback ID: " << fb.feedbackId
                         << "\nType: " << fb.type
                         << "\nRating: " << fb.rating
                         << "\nContent: " << fb.content << "\n"
                         << "\n Status: " << fb.status << "\n";
                    cout << " Reply from Staff: "
                         << (fb.response.empty() ? "No reply yet." : fb.response)
                         << endl;
                    cout << "-------------------------------------\n";
                }
            }

            if (!hasValidFeedback) {
                cout << "No feedback yet.\n";
            }
        }
    }

    if (!foundEvent) {
        cout << "You have no registered events.\n";
    } else if (!foundFeedback) {
        cout << "You have not submitted any feedback yet.\n";
    }

}
void deleteFeedback(const string& username, vector<Event>& events) {

    cout << "\n+=================================================+\n";
    cout << "|               Delete My Feedback                |\n";
    cout << "+=================================================+\n";

    bool hasFeedback = false;
    string chooseID;
    for (Event &ev : events) {
        if (ev.customer == username && ev.evstatus != "Canceled") {
            for (Feedback &fb : ev.feedbackList) {
                if (fb.status != "Deleted") {
                    cout << "| Event ID: " << ev.id
                     << " | Feedback ID: " << fb.feedbackId
                     << " | Content: " << fb.content << " |" << endl;
                    cout << "-------------------------------------------------------\n";
                    chooseID = fb.feedbackId;
                    hasFeedback = true;
                }
            }
        }
    }

    if (!hasFeedback) {
        cout << "You have no feedback to delete.\n";
        return;
    }

    string choice;
    cout << "Are you sure you want to delete the above feedbacks? (y/n): ";
    getline(cin, choice);

    if (choice == "N" || choice == "n") {
        cout << "Cancelled.\n";

    }else if (choice == "Y" || choice == "y") {

        for (Event &ev : events) {
            for (Feedback &fb : ev.feedbackList) {
                if (ev.customer == username && fb.feedbackId == chooseID) {
                    fb.status = "Deleted";
                    cout << "Feedback ID " << ev.id << " deleted successfully.\n";
                    return;
                }
            }
        }
    }else {
        cout << "Invalid choice.\n";
    }

}
void loadFeedbacks(vector<Event>& events) {
    ifstream inFile("feedbacks.txt");
    //if (!inFile) {
    //    cout << "No feedbacks file found. Starting fresh.\n";
    //return;
    //}

    string line;
    while (getline(inFile, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string id, feedbackId, type, ratingStr, content, status, response;

        getline(ss, id, '|');
        int evId = stoi(id);
        getline(ss, feedbackId, '|');
        getline(ss, type, '|');
        getline(ss, ratingStr, '|');
        getline(ss, content, '|');
        getline(ss, status, '|');
        getline(ss, response, '|');

        Feedback fb;
        fb.feedbackId = feedbackId;
        fb.type = type;
        fb.rating = stoi(ratingStr);
        fb.content = content;
        fb.status = status;
        fb.response = response;

        for (Event &ev : events) {
            if (ev.id == evId) {
                ev.feedbackList.push_back(fb);
                break;
            }
        }
    }
    inFile.close();
}
void saveFeedbacks(const vector<Event>& events) {
    ofstream fout("feedbacks.txt");
    if (!fout) {
        cout << "Error saving feedbacks!\n";
        return;
    }

    for (const Event& ev : events) {
        for (const Feedback& fb : ev.feedbackList) {

            fout << ev.id << "|"
                    << fb.feedbackId << "|"
                    << fb.type << "|"
                    << fb.rating << "|"
                    << fb.content << "|"
                    << fb.status << "|"
                    << fb.response << "\n";
        }
    }

    fout.close();
}
void submitFeedback(const string& username, vector<Event>& events) {
    cout << "\n+=================================================+\n";
    cout << "|                 Submit Feedback                 |\n";
    cout << "+=================================================+\n";

    Event* userEvent = nullptr;
    for (Event& ev : events) {
        if (ev.customer == username && ev.evstatus != "Canceled") {
            userEvent = &ev;
            break;
        }
    }

    if (!userEvent) {
        cout << "No events found for your account.\n";
        return;
    }

    // Check if feedback already exists
    for (Feedback& fb : userEvent->feedbackList) {
        if (fb.status != "Deleted") {
            cout << "You have already submitted feedback for this event.\n";
            return;
        }
    }

    Feedback fb;
    int nextId = userEvent->feedbackList.size() + 1;
    stringstream ss;
    ss << "FB" << setw(5) << setfill('0') << nextId;
    fb.feedbackId = ss.str();
    fb.status = "Pending";
    fb.rating = 0;

    cout << "\nSelect feedback type:\n";
    cout << "1. Rating\n";
    cout << "2. Complaint\n";
    cout << "3. Suggestion\n";
    cout << "Choice: ";
    int typeChoice;
    cin >> typeChoice;
    cin.ignore();

    if (typeChoice == 1) {
        fb.type = "Rating";

        cout << "\nEnter rating (1-5): ";
        cin >> fb.rating;
        cin.ignore();
        while (fb.rating < 1 || fb.rating > 5) {
            cout << "Invalid rating number. Please enter 1 - 5: ";
            cin >> fb.rating;
            cin.ignore();
        }
        cout << "Additional comments (Press enter to skip) : ";
        getline(cin, fb.content);
    } else if (typeChoice == 2) {
        fb.type = "Complaint";
        cout << "Describe complaint: ";
        getline(cin, fb.content);
    } else if (typeChoice == 3) {
        fb.type = "Suggestion";
        cout << "Enter suggestion: ";
        getline(cin, fb.content);
    } else {
        cout << "Invalid type.\n";
        return;
    }

    userEvent->feedbackList.push_back(fb);
    cout << "Feedback submitted successfully!\n";
}

void staffFeedbackMenu(vector<Event>& events) {
    int choice;
    do {
        cout << "\n--- Feedback Management ---\n";
        cout << "1. Manage Feedback\n";
        cout << "2. View All Feedback\n";
        cout << "3. View Summarize Feedback\n";
        cout << "0. Back\n";

        while (true) {
            cout << "Enter your choice: ";
            if (cin >> choice) {
                if (choice >= 0 && choice <= 3) {
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;
                }
            } else {
                cout << "Invalid input! Please enter a number.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }

        switch (choice) {
            case 1:
                updateFeedbackStatus(events);
                saveFeedbacks(events);
                break;
            case 2:
                viewAllFeedback(events);
                break;
            case 3:
                viewSummarizeFeedback(events);
                break;
            case 0:
                break;
            default:
                cout << "Invalid choice. Try again.\n";
        }

    } while (choice != 0);

}
void checkEventIssues(const vector<Event>& events) {

    cout << "\n+=================================================+\n";
    cout << "|               Event Issues / Notes              |\n";
    cout << "+=================================================+\n";

    bool found = false;

    for (const Event& e : events) {
        if (e.notes != "None" || e.issues != "N/A" ) {
            found = true;
            cout << "Event ID: " << e.id << "\n";
            if (e.notes != "None") {
                cout << "  Notes : " << e.notes << "\n";
            }
            if (e.issues != "N/A") {
                cout << "  Issues: " << e.issues << "\n";
            }
            cout << "-------------------------------------\n";
        }
    }

    if (!found) {
        cout << "No events with notes or issues found.\n";
    }
}
void viewAllEvents(const vector<Event>& events) {

    cout << "\n+" << string(135, '=') << "+\n";
    cout << "| " << setw(6) << left << "ID"
         << " | " << setw(12) << left << "Customer"
         << " | " << setw(15) << left << "Event Date"
         << " | " << setw(13) << left << "Venue"
         << " | " << setw(15) << left << "Location"
         << " | " << setw(12) << left << "Guest"
         << " | " << setw(13) << left << "Theme"
         << " | " << setw(12) << left << "Status"
         << " | " << setw(10) << right << "Total (RM)"
         << " |\n";
    cout << "+" << string(135, '=') << "+\n";

    for (const Event& e : events) {
        cout << "| " << setw(6) << left << e.id
             << " | " << setw(12) << left << e.customer
             << " | " << setw(15) << left << dateToString(e.date)
             << " | " << setw(13) << left << e.venue.type
             << " | " << setw(15) << left << e.venue.location
             << " | " << setw(12) << left << e.guestCount
             << " | " << setw(  13) << left << e.theme
             << " | " << setw(12) << left << e.evstatus
             << " | " << setw(10) << right << fixed << setprecision(2) << e.totalCost
             << " |\n";
    }
    cout << "+" << string(135, '=') << "+\n";
}
void updateEventStatus(vector<Event>& events) {
    if (events.empty()) {
        cout << "No events available.\n";
        return;
    }

    int id;
    cout << "\nEnter Event ID to update status (or 0 to cancel): ";
    cin >> id;
    cin.ignore();

    if (id == 0) {
        cout << "Cancelled.\n";
        return;
    }

    int index = -1;
    for (int i = 0; i < events.size(); i++) {
        if (events[i].id == id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        cout << "Event ID not found.\n";
        return;
    }

    cout << "Select new status:\n";
    cout << "1. Started\n";
    cout << "2. Postponed\n";
    cout << "3. Ended\n";
    cout << "4. Canceled\n";
    cout << "Choice: ";
    int st;
    cin >> st;
    cin.ignore();

    if (st == 1) {
        events[index].evstatus = "Started";
    } else if (st == 2) {
        events[index].evstatus = "Postponed";
    } else if (st == 3) {
        events[index].evstatus = "Ended";
    } else if (st == 4) {
        events[index].evstatus = "Canceled";
    } else {
        cout << "Invalid option.\n";
        return;
    }
    cout << "Event status updated to: " << events[index].evstatus << endl;
}
void commentEventIssues(vector<Event>& events) {
    if (events.empty()) {
        cout << "No events available.\n";
        return;
    }

    int id;
    cout << "\nEnter Event ID to comment on issues (or 0 to cancel): ";
    cin >> id;
    cin.ignore();

    if (id == 0) {
        cout << "Cancelled.\n";
        return;
    }

    int index = -1;
    for (int i = 0; i < events.size(); i++) {
        if (events[i].id == id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        cout << "Event ID not found.\n";
        return;
    }

    cout << "Enter comment on technical issues/problems: ";
    string comment;
    getline(cin, comment);

    events[index].issues = comment;
    cout << "Comment updated for Event ID " << id << ".\n";
}
void updateFeedbackStatus(vector<Event>& events) {
    string searchType, searchStatus;

    cout << "\n+==============================================================+\n";
    cout << "|                        Manage Feedback                       |\n";
    cout << "+==============================================================+\n";
    cout << "Enter feedback TYPE to search (leave blank to skip)\n";
    cout << "Options: Rating / Complaint / Suggestion\n> ";
    getline(cin, searchType);

    cout << "Enter feedback STATUS to filter (leave blank to skip)\n";
    cout << "Options: Pending / In Progress / Resolved\n> ";
    getline(cin, searchStatus);

    bool found = false;
    for (Event &ev : events) {
        for (Feedback &fb : ev.feedbackList) {
            bool matchType = searchType.empty() || fb.type.find(searchType) != string::npos;
            bool matchStatus = searchStatus.empty() || fb.status.find(searchStatus) != string::npos;

            if (matchType || matchStatus) {
                if (fb.status != "Deleted") {
                    cout << "\nFeedback ID: " << fb.feedbackId << endl;
                    cout << "Type: " << fb.type << endl;
                    cout << "Content: " << fb.content << endl;
                    cout << "Status: " << fb.status << endl;
                    cout << "Response: " << (fb.response.empty() ? "No response yet" : fb.response) << endl;
                    cout << "-------------------------------------------------\n";
                    found = true;
                }
            }
        }
    }

    if (!found) {
        cout << "No feedback matches your search criteria.\n";
        return;
    }

    string feedbackId;
    cout << "\nEnter Feedback ID to update (or 0 to cancel): ";
    getline(cin, feedbackId);

    if (feedbackId == "0") {
        cout << "Cancelled.\n";
        return;
    }

    for (Event &ev : events) {
        for (Feedback &fb : ev.feedbackList) {
            if (fb.feedbackId == feedbackId) {
                cout << "Current Status: " << fb.status << "\n";

                string newStatus;
                while (true) {
                    cout << "Enter new status (Resolved / leave blank = In Progress): ";
                    getline(cin, newStatus);

                    if (newStatus.empty()) {
                        fb.status = "In Progress";
                        cout << "No Update.\n";
                        return;
                    }

                    string temp = newStatus;
                    transform(temp.begin(), temp.end(), temp.begin(), ::tolower);

                    if (temp == "resolved") {
                        fb.status = "Resolved";
                        break;
                    } else {
                        cout << "Invalid status. ";
                    }
                }

                cout << "Enter response to customer (leave blank to skip): ";
                string response;
                getline(cin, response);
                if (!response.empty()) fb.response = response;

                cout << "Feedback updated successfully!\n";
                return;
            }
        }
    }


    cout << "Feedback ID not found.\n";
}

void viewAllFeedback(const vector<Event>& events) {
    cout << "\n+================= All Feedbacks =================+\n";

    for (const Event& ev : events) {
        stringstream ss;
        ss << "EV" << setw(5) << setfill('0') << ev.id;
        string eventId = ss.str();

        cout << "Event ID: " << eventId << " | Customer: " << ev.customer << "\n";

        if (ev.feedbackList.empty()) {
            cout << "   No feedback yet.\n";
        } else {
            for (const Feedback& fb : ev.feedbackList) {
                if (fb.status != "Deleted") {
                    cout << "  [ID: " << fb.feedbackId
                         << " | Type: " << fb.type
                         << " | Rating: " << fb.rating
                         << " | Status: " << fb.status
                         << " | Content: " << fb.content << "]\n";
                }
            }
        }

        cout << "---------------------------------------------------\n";
    }
}

void viewSummarizeFeedback(const vector<Event>& events) {
    cout << "\n+================================================+\n";
    cout << "|             Summarize All Feedbacks            |\n";
    cout << "+================================================+\n";

    if (events.empty()) {
        cout << "No events available.\n";
        return;
    }

    int totalRatingAll = 0, countRatingAll = 0;
    int complaintsAll = 0, suggestionsAll = 0, othersAll = 0;
    int pendingAll = 0, inProgressAll = 0, resolvedAll = 0;

    for (const Event& ev : events) {
        int totalRating = 0, countRating = 0;
        int complaints = 0, suggestions = 0, others = 0;
        int pending = 0, inProgress = 0, resolved = 0;

        for (const Feedback& fb : ev.feedbackList) {
            if (fb.status == "Deleted") continue;

            if (fb.rating > 0) {
                totalRating += fb.rating;
                countRating++;
                totalRatingAll += fb.rating;
                countRatingAll++;
            }

            if (fb.type == "Complaint") { complaints++; complaintsAll++; }
            else if (fb.type == "Suggestion") { suggestions++; suggestionsAll++; }
            else { others++; othersAll++; }

            if (fb.status == "Pending") { pending++; pendingAll++; }
            else if (fb.status == "In Progress") { inProgress++; inProgressAll++; }
            else if (fb.status == "Resolved") { resolved++; resolvedAll++; }
        }
    }

    cout << "\n========= Overall Summary (All Events) =========\n";
    if (countRatingAll > 0) {
        double avgAll = (double)totalRatingAll / countRatingAll;
        cout << "\n Overall Average Rating: " << fixed << setprecision(2) << avgAll << endl;
    } else {
        cout << "\n Overall Average Rating: N/A (No feedback yet)\n";
    }

    cout << "\nTotal Complaints   : " << complaintsAll << endl;
    cout << "Total Suggestions  : " << suggestionsAll << endl;
    cout << "Total Others       : " << othersAll << endl;

    cout << "\nTotal Pending      : " << pendingAll << endl;
    cout << "Total In Progress  : " << inProgressAll << endl;
    cout << "Total Resolved     : " << resolvedAll << endl << "\n";
    cout << "=================================================\n";
}
void generateEventReport(const vector<Event>& events) {
    cout << "\n+===================================================+\n";
    cout << "|                    Event Report                   |\n";
    cout << "+===================================================+\n";

    if (events.empty()) {
        cout << "No events available to report.\n";
        return;
    }

    int totalEvents = 0;
    int started = 0, postponed = 0, ended = 0, canceled = 0;
    double totalRevenue = 0.0;
    int totalGuests = 0;
    double maxCost = 0.0, minCost = 1e9;
    int totalFeedbacks = 0, complaints = 0, suggestions = 0;

    for (const Event& ev : events) {
        totalEvents++;

        if (ev.evstatus == "Started") started++;
        else if (ev.evstatus == "Postponed") postponed++;
        else if (ev.evstatus == "Ended") ended++;
        else if (ev.evstatus == "Canceled") canceled++;

        if (ev.isPaid && ev.evstatus != "Canceled") {
            totalRevenue += ev.totalCost;
        }

        totalGuests += ev.guestCount;

        if (ev.totalCost > maxCost) maxCost = ev.totalCost;
        if (ev.totalCost < minCost) minCost = ev.totalCost;

        totalFeedbacks += ev.feedbackList.size();
        for (const Feedback& fb : ev.feedbackList) {
            if (fb.type == "Complaint") complaints++;
            else if (fb.type == "Suggestion") suggestions++;
        }
    }

    double avgGuests = (totalEvents > 0) ? (double)totalGuests / totalEvents : 0.0;
    double avgCostPerGuest = (totalGuests > 0) ? totalRevenue / totalGuests : 0.0;

    cout << left << setw(30) << "Total Events:" << right << setw(22) << totalEvents << "\n";
    cout << left << setw(30) << "   Started:" << right << setw(22) << started << "\n";
    cout << left << setw(30) << "   Postponed:" << right << setw(22) << postponed << "\n";
    cout << left << setw(30) << "   Ended:" << right << setw(22) << ended << "\n";
    cout << left << setw(30) << "   Canceled:" << right << setw(22) << canceled << "\n";
    cout << "-----------------------------------------------------\n";
    cout << left << setw(30) << "Total Revenue (RM):" << right << setw(22) << fixed << setprecision(2) << totalRevenue << "\n";
    cout << left << setw(30) << "Avg Revenue per Guest (RM):" << right << setw(22) << avgCostPerGuest << "\n";
    cout << "----------------------------------------------------\n";
    cout << left << setw(30) << "Total Guests:" << right << setw(22) << totalGuests << "\n";
    cout << left << setw(30) << "Avg Guests per Event:" << right << setw(22) << avgGuests << "\n";
    cout << left << setw(30) << "Highest Event Cost (RM):" << right << setw(22) << maxCost << "\n";
    cout << left << setw(30) << "Lowest Event Cost (RM):" << right << setw(22) << minCost << "\n";
    cout << "+===================================================+\n";
}
void generateMonthlyEventReport(const vector<Event>& events) {
    cout << "\n+================================================+\n";
    cout << "|               Monthly Event Report             |\n";
    cout << "+================================================+\n";

    if (events.empty()) {
        cout << "No events available.\n";
        return;
    }

    int month, year;
    cout << "Enter month (1-12): ";
    cin >> month;
    cout << "Enter year: ";
    cin >> year;
    cin.ignore();

    int totalEvents = 0;
    int started = 0, postponed = 0, ended = 0, canceled = 0;
    double totalRevenue = 0.0;

    for (const Event& ev : events) {
        if (ev.date.month == month && ev.date.year == year) {
            totalEvents++;

            if (ev.evstatus == "Started") started++;
            else if (ev.evstatus == "Postponed") postponed++;
            else if (ev.evstatus == "Ended") ended++;
            else if (ev.evstatus == "Canceled") canceled++;

            if (ev.isPaid && ev.evstatus != "Canceled") {
                totalRevenue += ev.totalCost;
            }
        }
    }

    if (totalEvents == 0) {
        cout << "No events found for " << month << "/" << year << ".\n";
        return;
    }

    cout << "\nReport for " << month << "/" << year << ":\n";
    cout << "-------------------------------------------------\n";
    cout << "Total Events: " << totalEvents << "\n";
    cout << "   Started:    " << started << "\n";
    cout << "   Postponed:  " << postponed << "\n";
    cout << "   Ended:      " << ended << "\n";
    cout << "   Canceled:   " << canceled << "\n";
    cout << "-------------------------------------------------\n";
    cout << "Total Revenue (only paid & non-canceled): RM " << totalRevenue << "\n";
    cout << "-------------------------------------------------\n";

    cout << "\n Event Details:\n\n";
    cout << left << setw(11) << "Event ID"
         << setw(15) << "Customer"
         << setw(15) << "Date"
         << setw(12) << "Status"
         << setw(8)  << "Paid"
         << right << setw(12) << "Cost (RM)"
         << "\n";

    cout << string(73, '-') << "\n";

    for (const Event& ev : events) {
        if (ev.date.month == month && ev.date.year == year) {
            cout << left << setw(11) << ev.id
                 << setw(15) << ev.customer
                 << setw(15) << dateToString(ev.date)
                 << setw(12) << ev.evstatus
                 << setw(8)  << (ev.isPaid ? "Yes" : "No")
                 << right << setw(12) << fixed << setprecision(2) << ev.totalCost
                 << "\n";
        }

    }
}

void venueMenu(vector<Venue>& venues, int& nextVenueID) {
    int choice;
    do {
        cout << "\n--- Venue Management ---\n";
        cout << "1. Create Venue\n";
        cout << "2. View Venues\n";
        cout << "0. Back\n";

        while (true) {
            cout << "Enter your choice: ";
            if (cin >> choice) {
                if (choice >= 0 && choice <= 2) {
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;
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
                viewVenues(venues);
                cout << "Press Enter to return to Admin Menu...";
                cin.get();
                break;
            case 0:
                break;
            default:
                cout << "Invalid choice.\n";
        }
    } while (choice != 0);
}
void menuMenu(vector<Menu>& menus, int& nextMenuID) {
    int choice;
    do {
        cout << "\n--- Menu Management ---\n";
        cout << "1. Create Menu\n";
        cout << "2. View Menus\n";
        cout << "0. Back\n";
        while (true) {
            cout << "Enter your choice: ";
            if (cin >> choice) {
                if (choice >= 0 && choice <= 2) {
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;
                }
            } else {
                cout << "Invalid input! Please enter a number.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }

        switch (choice) {
            case 1:
                createMenu(menus, nextMenuID);
                break;
            case 2:
                viewMenus(menus);
                cout << "Press Enter to return to Admin Menu...";
                cin.get();
                break;
            case 0:
                break;
            default:
                cout << "Invalid choice.\n";
        }
    } while (choice != 0);
}
void eventMenu(vector<Event>& events) {
    int choice;
    do {
        cout << "\n--- Event Management ---\n";
        cout << "1. View All Events\n";
        cout << "2. Update Event Status\n";
        cout << "3. Comment on Technical Issues/Problems\n";
        cout << "4. View Event Notes or Issues\n";
        cout << "0. Back\n";

        while (true) {
            cout << "Enter your choice: ";
            if (cin >> choice) {
                if (choice >= 0 && choice <= 4) {
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;
                }
            } else {
                cout << "Invalid input! Please enter a number.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
        switch (choice) {
            case 1:
                viewAllEvents(events);
                break;
            case 2:
                updateEventStatus(events);
                saveDataToFile(events);
                break;
            case 3:
                commentEventIssues(events);
                saveDataToFile(events);
                break;
            case 4:
                checkEventIssues(events);
                break;
            case 0:
                break;
            default:
                cout << "Invalid choice.\n";
        }
    } while (choice != 0);
}
void reportMenu(const vector<Event>& events) {
    int choice;
    do {
        cout << "\n--- Reports ---\n";
        cout << "1. Generate Event Report\n";
        cout << "2. Generate Monthly Event Report\n";
        cout << "0. Back\n";

        while (true) {
            cout << "Enter your choice: ";
            if (cin >> choice) {
                if (choice >= 0 && choice <= 2) {
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;
                }
            } else {
                cout << "Invalid input! Please enter a number.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }

        switch (choice) {
            case 1:
                generateEventReport(events);
                break;
            case 2:
                generateMonthlyEventReport(events);
                break;
            case 0:
                break;
            default:
                cout << "Invalid choice.\n";
        }
    } while (choice != 0);
}


void userMenu(const string& username,const string& phone, vector<Event>& events, vector<Menu>& menus, vector<Venue>& venues, int& nextMenuID,int& nextEventID) {
    int choice;
    do {
        cout << "\n===== User Page (" << username << ") =====\n";
        cout << "1. Register Wedding Event\n";
        cout << "2. My Feedback\n";
        cout << "3. View Receipt\n";
        cout << "0. Logout\n";

        //input validation
        while (true) {
            cout << "Enter your choice: ";
            if (cin >> choice) {
                if (choice >= 0 && choice <= 5) {
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;
                }
            } else {
                cout << "Invalid input! Please enter a number.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }

        switch (choice) {
            case 1:
                registerEvent(username,phone, events, menus, venues, nextEventID);
                break;
            case 2:
                userFeedbackMenu(username,events);
                break;
            case 3:
                printReceipt(username,events);
                break;
            case 0:
                cout << "Logging out...\n";
                break;
            default:
                cout << "Invalid choice! Please enter a number between 1-5.\n";

        }
    } while (choice != 0);
}
void adminMenu(vector<Event>& events, vector<Menu>& menus, vector<Venue>& venues, int& nextMenuID,int& nextVenueID) {
    int choice;
    do {

        cout << "\n===== Organizer Page (Admin) =====\n";
        cout << "1. Venue Management\n";
        cout << "2. Menu Management\n";
        cout << "3. Event Management\n";
        cout << "4. Feedback Management\n";
        cout << "5. Reports\n";
        cout << "0. Logout\n";

        //input validation
        while (true) {
            cout << "Enter your choice: ";
            if (cin >> choice) {
                if (choice >= 0 && choice <= 5) {
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;
                }
            } else {
                cout << "Invalid input! Please enter a number.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }

        switch (choice) {
            case 1:
                venueMenu(venues, nextVenueID);
                break;
            case 2:
                menuMenu(menus, nextMenuID);
                break;
            case 3:
                eventMenu(events);
                break;
            case 4:
                staffFeedbackMenu(events);
                break;
            case 5:
                reportMenu(events);
                break;
            case 0:
                cout << "Logging out...\n";
                break;
            default:
                cout << "Invalid input! Please enter a number between 1 - 6.\n";
        }
    } while (choice != 0);
}
void registerUser() {
    User newUser;
    cout << "---- Register New User (Enter 0 to go back) ----\n";


    while (true) {
        cout << "\nEnter username: ";
        getline(cin, newUser.username);


        if (newUser.username == "0") return;

        if (newUser.username.empty()) {
            cout<<"Username cannot be empty!\n";
            continue;
        }

        bool duplicateUsername = false;
        ifstream inFile("users.txt");
        if (inFile) {
            string line;
            while (getline(inFile, line)) {
                stringstream ss(line);
                string uname, pass, phone;
                getline(ss, uname, '|');
                getline(ss, pass, '|');
                getline(ss, phone, '|');

                if (uname == newUser.username) {
                    duplicateUsername = true;
                    break;
                }
            }
            inFile.close();
        }

        if (duplicateUsername) {
            cout << "Username already exists. Please choose a different one.\n";
        } else {
            break;
        }
    }

    do {
        cout << "Enter password (min 6 chars & at least one digit): ";
        getline(cin, newUser.password);

        if (newUser.password == "0") return;

        if (newUser.password.empty()) {
            cout << "Password cannot be empty !\n";
            continue;
        };
        if (!validatePassword(newUser.password)) {
            cout << "Invalid password! Please try again.\n";
        }
    } while (newUser.password.empty() || !validatePassword(newUser.password));

    while (true) {
        cout << "Enter phone number: ";
        getline(cin, newUser.phone);

        if (newUser.phone == "0") return;

        if (newUser.phone.empty()) {
            cout << "Phone number cannot be empty !\n";
            continue;
        }

        if (!validatePhone(newUser.phone)) {
            cout << "Invalid phone number! Please try again.\n";
            continue;
        }

        bool duplicatePhone = false;
        ifstream inFile("users.txt");
        if (inFile) {
            string line;
            while (getline(inFile, line)) {
                stringstream ss(line);
                string uname, pass, phone;
                getline(ss, uname, '|');
                getline(ss, pass, '|');
                getline(ss, phone, '|');

                if (phone == newUser.phone) {
                    duplicatePhone = true;
                    break;
                }
            }
            inFile.close();
        }

        if (duplicatePhone) {
            cout << "Phone number already registered. Please use a different one.\n";
        } else {
            break;
        }
    }

    ofstream outFile("users.txt", ios::app);
    if (!outFile) {
        cout << "Error opening file!\n";
        return;
    }
    outFile << newUser.username << "|" << newUser.password << "|" << newUser.phone << endl;
    outFile.close();

    cout << "Registration successful!\n";
}

// login(user n admin)
void loginUser(vector<Event>& events, vector<Menu>& menus, vector<Venue>& venues, int& nextMenuID, int& nextEventID,int& nextVenueID) {
    string username, password;
    cout << "---- Login (Enter 0 to go back) ----\n";

    while (true) {
        cout << "\nEnter username : ";
        getline(cin, username);
        if (username == "0") return;
        if (username.empty()) {
            cout << "Username cannot be empty!\n";
            continue;
        }break;
    }
    while (true) {
        cout << "Enter password : ";
        getline(cin, password);
        if (password == "0") return;
        if (password.empty()) {
            cout << "Password cannot be empty!\n";
            continue;
        }break;
    }

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

        if (!getline(ss, fileUser, '|')) continue;
        if (!getline(ss, filePass, '|')) continue;
        getline(ss, filePhone);

        if (!filePhone.empty() && filePhone.back() == '\r')
            filePhone.pop_back();

        if (fileUser == username && filePass == password) {
            found = true;
            cout << "\nLogin successful! Welcome " << fileUser << ".\n";
            userMenu(fileUser,filePhone, events, menus, venues, nextMenuID,nextEventID);
            break;
        }
    }
    inFile.close();

    if (!found) {
        cout << "Invalid username or password!\n";
    }
}

bool validatePhone(const string& phone) {
    regex phonePattern("^01[0-9]{8}$");
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

double calculateTotalCost(const Event &e) {
    int tables = (e.guestCount + 9) / 10; // 10 guests per table
    double menuCost = (e.menu.id != 0) ? (e.menu.price * tables) : 0.0;
    return e.venue.price + menuCost;
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
             << e.phone << ";"
             << dateToString(e.date) << ";"
             << e.venue.type << ";"
             << e.venue.location << ";"
             << e.venue.capacity << ";"
             << e.venue.price << ";"
             << e.venue.timeslot << ";"
             << e.guestCount << ";"
             << e.theme << ";"
             << e.notes << ";"
             << e.evstatus << ";"
             << e.issues <<";";
        // Menu
        file << e.menu.id << ";"
             << e.menu.cuisine << ";"
             << e.menu.price << ";"
             << e.menu.servingStyle << ";";

        // Food items (comma separated)
        for (size_t i = 0; i < e.menu.foodItems.size(); i++) {
            file << e.menu.foodItems[i];
            if (i < e.menu.foodItems.size() - 1) file << ",";
        }
        file << ";"; // end of food list

        // Customizations (comma separated)
        for (size_t i = 0; i < e.customizations.size(); i++) {
            file << e.customizations[i];
            if (i < e.customizations.size() - 1) file << ",";
        }
        file << ";";

        //payment
        file << e.totalCost << ";" << (e.isPaid ? "1" : "0") << ";" << e.paymentMethod;
        file << "\n";
    }

    file.close();
    cout << "Events saved successfully.\n";
}
// Load all events from file
void loadDataFromFile(vector<Event>& events,int& nextEventI) {
    ifstream file("data.txt");

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

        if (tokens.size() < 23) {
            cout << "Invalid event format: " << line << endl;
            continue;
        }

        Event e;
        int index = 0;

        e.id = stoi(tokens[index++]);
        e.customer = tokens[index++];
        e.phone = tokens[index++];
        e.date = parseDate(tokens[index++]);
        e.venue.type = tokens[index++];
        e.venue.location = tokens[index++];
        e.venue.capacity = stoi(tokens[index++]);
        e.venue.price = stod(tokens[index++]);
        e.venue.timeslot = tokens[index++];
        e.guestCount = stoi(tokens[index++]);
        e.theme = tokens[index++];
        e.notes = tokens[index++];
        e.evstatus = tokens[index++];
        e.issues = tokens[index++];

        //menu
        e.menu.id = stoi(tokens[index++]);
        e.menu.cuisine = tokens[index++];
        e.menu.price = stod(tokens[index++]);
        e.menu.servingStyle = tokens[index++];

        //food
        if (index < tokens.size()) {
            stringstream foodStream(tokens[index++]);
            string foodItem;
            while (getline(foodStream, foodItem, ',')) {
                trim(foodItem);
                if (!foodItem.empty()) e.menu.foodItems.push_back(foodItem);
            }
        }

        //Customizations
        if (index < tokens.size()) {
            stringstream custStream(tokens[index++]);
            string custItem;
            while (getline(custStream, custItem, ',')) {
                trim(custItem);
                if (!custItem.empty()) e.customizations.push_back(custItem);
            }
        }

        //payment
        if (index < tokens.size()) {
            try {
                e.totalCost = stod(tokens[index++]);
            } catch (...) {
                e.totalCost = 0.0;
            }
        } else {
            e.totalCost = 0.0;
        }

        if (index < tokens.size()) {
            e.isPaid = (tokens[index++] == "1");
        } else {
            e.isPaid = false;
        }
        e.paymentMethod = tokens[index++];

        if (e.id >= nextEventI) {
            nextEventI = e.id + 1;
        }

        events.push_back(e);
    }

    file.close();
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
}

void registerEvent(const string& username,const string& phone, vector<Event>& events,vector<Menu>& menus, const vector<Venue>& venues,int& nextEventID) {
    Event e;
    e.id = nextEventID++;
    e.customer = username;
    e.phone = phone;

    cout << "\n--- Register Event (Enter 0 to go back to main menu) ---\n";

    if (venues.empty()) {
        cout << "No venues available! Please contact admin to create venues first.\n";
        return;
    }

    Date today = getTodayDate();

    // Date validation
    while (true) {
        cout << "\n--- Event Date ---\n";
        cout << "Enter event date (yyyy-mm-dd, or 0 to cancel): ";
        string dateInput;
        getline(cin, dateInput);
        trim(dateInput);

        if (dateInput == "0") {
            cout << "Event registration cancelled.\n";
            return;
        }

        if (dateInput.empty()) {
            cout << "Date cannot be empty!\n";
            continue;
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
        cout << "\n--- Available Venues ---\n";
        for (const auto& v : venues) {
            cout << "[" << v.id << "] "
                 << "Venue: " << v.type
                 << " | Location: " << v.location
                 << " | Capacity: " << v.capacity
                 << " | Price: RM" << v.price << "\n";
        }

        int venueID;
        Venue chosenVenue;
        bool found = false;

        while (true) {
            cout << "Enter venue ID (or 0 to cancel): ";
            string input;
            getline(cin, input);
            trim(input);

            if (input.empty()) {
                cout << "Venue ID cannot be empty! Please try again.\n";
                continue;
            }

            bool isNumeric = all_of(input.begin(), input.end(), ::isdigit);
            if (!isNumeric) {
                cout << "Invalid input! Please enter a number.\n";
                continue;
            }

            venueID = stoi(input);
            if (venueID == 0) {
                cout << "Event registration cancelled.\n";
                return;
            }

            for (const auto& v : venues) {
                if (v.id == venueID) {
                    chosenVenue = v;
                    found = true;
                    break;
                }
            }

            if (found) break;
            cout << "Invalid venue ID! Try again.\n";
        }

        e.venue = chosenVenue;

        //slot
        int slotChoice;
        while (true) {
            cout << "\n--- Venue Slot ---\n";
            cout << "Choose slot (1=Morning (10:00 - 14:00), 2=Evening (18:00 - 22:00), 0=Cancel): ";
            string input;
            getline(cin, input);
            trim(input);

            if (input.empty()) {
                cout << "Slot selection cannot be empty! Please try again.\n";
                continue;
            }

            bool isNumeric = all_of(input.begin(), input.end(), ::isdigit);
            if (!isNumeric) {
                cout << "Invalid input! Please enter a number.\n";
                continue;
            }

            slotChoice = stoi(input);
            if (slotChoice == 0) {
                cout << "Event registration cancelled.\n";
                return;
            }

            if (slotChoice == 1 || slotChoice == 2) {
                break;
            } else {
                cout << "Invalid slot selection. Please enter 1 or 2.\n";
            }
        }

        chosenSlot = (slotChoice == 1) ? "Morning (10:00 - 14:00)" : "Evening (18:00 - 22:00)";
        e.venue.timeslot = chosenSlot;

        //check conflicts
        if (hasConflict(e, events)) {
            cout << "Error: This venue and time slot is already booked!\n"
                 << "Please choose a different venue or time slot.\n";
            validVenue = false;
        } else {
            validVenue = true;
        }
    }

    while (true) {
        cout << "\n--- Guest Information ---\n";
        cout << "Enter expected number of guests (min 10, max " << e.venue.capacity << ", 0=Cancel): ";
        string input;
        getline(cin, input);
        trim(input);

        if (input.empty()) {
            cout << "Guest count cannot be empty! Please try again.\n";
            continue;
        }

        // Check if input is numeric
        bool isNumeric = all_of(input.begin(), input.end(), ::isdigit);
        if (!isNumeric) {
            cout << "Invalid input! Please enter a number.\n";
            continue;
        }

        int guestCount = stoi(input);
        if (guestCount == 0) {
            cout << "Event registration cancelled.\n";
            return;
        }

        if (guestCount < 10) {
            cout << "Guest count must be at least 10. Try again.\n";
        } else if (guestCount > e.venue.capacity) {
            cout << "Guest count exceeds venue capacity (" << e.venue.capacity << "). Try again.\n";
        } else {
            e.guestCount = guestCount;
            break;
        }
    }

    e.totalCost = calculateTotalCost(e);
    e.isPaid = false;

    // Choose theme
    int themeChoice;
    while (true) {
        cout << "\n--- Theme Selection ---\n";
        cout << "1. Chinese Traditional\n2. Indian Traditional\n3. Malay Traditional\n"
             << "4. Vintage\n5. Natural\n6. Others\n";
        cout << "Enter choice (0 to cancel): ";

        string input;
        getline(cin, input);
        trim(input);

        if (input.empty()) {
            cout << "Theme choice cannot be empty! Please try again.\n";
            continue;
        }

        bool isNumeric = all_of(input.begin(), input.end(), ::isdigit);
        if (!isNumeric) {
            cout << "Invalid input! Please enter a number.\n";
            continue;
        }

        themeChoice = stoi(input);
        if (themeChoice == 0) {
            cout << "Event registration cancelled.\n";
            return;
        }

        if (themeChoice >= 1 && themeChoice <= 6) {
            break;
        } else {
            cout << "Invalid choice! Please enter a number between 1-6.\n";
        }
    }

    switch (themeChoice) {
        case 1: e.theme = "Chinese Traditional"; break;
        case 2: e.theme = "Indian Traditional"; break;
        case 3: e.theme = "Malay Traditional"; break;
        case 4: e.theme = "Vintage"; break;
        case 5: e.theme = "Natural"; break;
        case 6:
            while (true) {
                cout << "Enter custom theme (or 0 to cancel): ";
                getline(cin, e.theme);
                trim(e.theme);
                if (e.theme == "0") {
                    cout << "Event registration cancelled.\n";
                    return;
                }
                if (!e.theme.empty()) break;
                cout << "Theme cannot be empty! Please try again.\n";
            }
            break;
    }

    //special request
    cout << "\n--- Special Requests ---\n";
    cout << "Any special requests or notes? (Press Enter to skip, 0 to cancel): ";
    getline(cin, e.notes);
    trim(e.notes);

    if (e.notes == "0") {
        cout << "Event registration cancelled.\n";
        return;
    }

    if (e.notes.empty()) {
        e.notes = "None";
    }

    e.evstatus = "Planned";
    e.issues = "N/A";

    //ask confirmation
    string input;
    char confirm;
    while (true) {
        cout << "\nConfirm event registration? (y/n): ";
        getline(cin, input);
        trim(input);

        if (input.empty()) {
            cout << "Input cannot be empty! Please enter 'y', 'n'.\n";
            continue;
        }

        if (input == "y" || input == "Y" || input == "n" || input == "N") {
            confirm = input[0];
            break;
        }
        cout << "Invalid input! Please enter 'y', 'n', or '0'.\n";
    }

    if (confirm == 'y' || confirm == 'Y') {
        int nextMenuID = 0;
        if (!customizeMenu(e, events, const_cast<vector<Menu>&>(menus), nextMenuID)) {
            cout << "Customization cancelled. Event not saved.\n";
            return;
        }

        if (!makePayment(e)) {
            cout << "Payment cancelled. Event not saved.\n";
            return;
        }

        events.push_back(e);
        saveDataToFile(events);
        cout << "Press enter to back to main menu....";
        cin.get();
        return;
    }else {
        cout << "Event registration cancelled.\n";
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
        string input;
        getline(cin, input);
        trim(input);

        regex numberRegex("^[0-9]+(\\.[0-9]+)?$");

        if (!regex_match(input, numberRegex)) {
            cout << "Invalid input! Digits or Decimal only.\n";
            continue;
        }

        double price = stod(input);
        if (price >= 200) {
            m.price = price;
            break;
        } else {
            cout << "Price must be at least RM200! Please try again.\n";
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

    cout << "\n========================================\n";
    cout << "              Menu Listings              \n";
    cout << "========================================\n";

    for (const auto& m : menus) {
        cout << "Menu ID   : " << m.id << "\n";
        cout << "Cuisine   : " << m.cuisine << "\n";
        cout << "Price     : RM" << fixed << setprecision(2) << m.price
             << " per table (10 pax)\n";
        cout << "Food Items: ";
        for (size_t j = 0; j < m.foodItems.size(); j++) {
            cout << m.foodItems[j];
            if (j < m.foodItems.size() - 1) cout << ", ";
        }
        cout << "\n----------------------------------------\n";
    }
}


bool customizeMenu(Event& e, vector<Event>& events, vector<Menu>& menus, int& nextMenuID) {
    if (menus.empty()) {
        cout << "No menus available. Please ask admin to create a menu first.\n";
        return false;
    }
    cout << "Please select a menu package for your event from the options below:\n";
    cout << "\n========== Available Menus ==========\n";
    for (int i = 0; i < menus.size(); i++) {
        cout << "[" << i + 1 << "] "
             << menus[i].cuisine << " (RM" << menus[i].price << " per table)\n";

        cout << "     Food Items: ";
        for (int j = 0; j < menus[i].foodItems.size(); j++) {
            cout << menus[i].foodItems[j];
            if (j < menus[i].foodItems.size() - 1) cout << ", ";
        }
        cout << "\n--------------------------------------\n";
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
        return false;
    }

    e.menu = menus[menuChoice - 1];
    e.totalCost = calculateTotalCost(e);

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

    int styleChoice;
    while (true) {
        cout << "\nSelect serving style:\n1. Buffet\n2. Plated\nEnter choice: ";
        if (cin >> styleChoice && (styleChoice == 1 || styleChoice == 2)) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            e.menu.servingStyle = (styleChoice == 1) ? "Buffet" : "Plated";
            break;
        }
        cout << "Invalid choice! Please enter 1 or 2.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
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
        cout << "Menu customized successfully!\n";
        return true;
    } else {
        cout << "Menu customization cancelled.\n";
        return false;
    }
}

bool makePayment(Event &e) {
    //summary
    cout << "\nHere is your event summary. Please review the details carefully before proceeding to payment.";
    cout << "\n============= Event Summary =============\n";
    cout << left << setw(18) << "Event ID:"      << e.id << "\n";
    cout << left << setw(18) << "Customer:"      << e.customer << "\n";
    cout << left << setw(18) << "Venue:"         << e.venue.type << "\n";
    cout << left << setw(18) << "Date:"          << dateToString(e.date) << "\n";
    cout << left << setw(18) << "Guests:"        << e.guestCount << "\n";
    cout << left << setw(18) << "Cuisine:"       << e.menu.cuisine << "\n";
    cout << left << setw(18) << "Serving Style:" << e.menu.servingStyle << "\n";
    cout << left << setw(18) << "Menu Items:";
    if (!e.menu.foodItems.empty()) {
        for (size_t i = 0; i < e.menu.foodItems.size(); ++i) {
            cout << e.menu.foodItems[i];
            if (i < e.menu.foodItems.size() - 1) cout << ", ";
        }
    } else {
        cout << "None";
    }
    cout << "\n";
    cout << left << setw(18) << "Custom Notes:"      << (e.notes.empty() ? "-" : e.notes) << "\n";
    cout << left << setw(18) << "Total Amount (RM):" << fixed << setprecision(2) << e.totalCost << "\n";
    cout << "========================================\n";

    char confirm;
    cout << "Proceed to payment? (Y/N): ";
    cin >> confirm;
    cin.ignore();
    if (tolower(confirm) != 'y') {
        cout << "Payment cancelled.\n";
        return false;
    }

    cout << "\n--- Payment ---\n";
    while (true) {
        cout << "Choose method (1.Card  2.Tng eWallet  3.Online Banking): ";
        string input;
        getline(cin, input);
        trim(input);

        if (input.empty()) {
            cout << "Payment method cannot be empty! Please try again.\n";
            continue;
        }

        bool isNumeric = all_of(input.begin(), input.end(), ::isdigit);
        if (!isNumeric) {
            cout << "Invalid input! Please enter a number.\n";
            continue;
        }

        int method = stoi(input);

        if (method == 1) {
            e.paymentMethod = "Card";

            string cardNumber;
            while (true) {
                cout << "Enter Card Number (16 digits): ";
                getline(cin, cardNumber);
                trim(cardNumber);

                if (cardNumber.empty()) {
                    cout << "Card number cannot be empty!\n";
                    continue;
                }
                if (cardNumber.size() != 16 || !all_of(cardNumber.begin(), cardNumber.end(), ::isdigit)) {
                    cout << "Invalid card number! Must be 16 digits.\n";
                    continue;
                }
                break;
            }
            break;

        } else if (method == 2) {
            e.paymentMethod = "Tng eWallet";

            string ePhone;
            while (true) {
                cout << "Phone Number: ";
                getline(cin, ePhone);
                trim(ePhone);

                if (ePhone.empty()) {
                    cout << "Phone number cannot be empty!\n";
                    continue;
                }
                if (!validatePhone(ePhone)) {
                    cout << "Invalid phone number!.\n";
                    continue;
                }
                break;
            }
            string pin;
            while (true) {
                cout << "Enter 6-digit eWallet PIN: ";
                getline(cin, pin);
                trim(pin);

                if (pin.empty()) {
                    cout << "PIN cannot be empty!\n";
                    continue;
                }
                if (pin.size() != 6 || !all_of(pin.begin(), pin.end(), ::isdigit)) {
                    cout << "Invalid PIN! Must be exactly 6 digits.\n";
                    continue;
                }
                break;
            }
            break;

        } else if (method == 3) {
            e.paymentMethod = "Online Banking";

            string bankName, refNo;
            while (true) {
                cout << "Enter Bank Name : ";
                getline(cin, bankName);
                trim(bankName);

                if (bankName.empty()) {
                    cout << "Bank name cannot be empty!\n";
                    continue;
                }
                if (any_of(bankName.begin(), bankName.end(), ::isdigit)) {
                    cout << "Invalid bank name! It cannot contain numbers.\n";
                    continue;
                }
                break;
            }

            while (true) {
                cout << "Enter Transaction Reference Number: ";
                getline(cin, refNo);
                trim(refNo);

                if (refNo.empty()) {
                    cout << "Reference Number cannot be empty!\n";
                    continue;
                }
                if (!all_of(refNo.begin(), refNo.end(), ::isdigit)) {
                    cout << "Reference number must be numeric!\n";
                    continue;
                }
                if (refNo.size() < 10) {
                    cout << "Reference number must be at least 10 digits!\n";
                    continue;
                }
                break;
            }
            break;

        } else {
            cout << "Invalid choice! Please enter 1, 2, or 3.\n";
        }
    }

    e.isPaid = true;
    e.evstatus = "Confirmed";
    cout << "\nPayment successful via " << e.paymentMethod << "!\n";
    return true;
}


void printReceipt(const string &username, const vector<Event> &events) {
    //find event
    const Event* userEvent = nullptr;
    for (const auto &e : events) {
        if (e.customer == username) {
            userEvent = &e;
            break;
        }
    }

    if (!userEvent) {
        cout << "No event found for this user.\n";
        return;
    }

    const Event &e = *userEvent;

    if (!e.isPaid) {
        cout << "No receipt available. Please complete your payment first.\n";
        cout << "Press enter to back to user page....";
        cin.get();
        return;
    }

    // Print receipt only if paid
    cout << "\n=============================================\n";
    cout << "              PAYMENT RECEIPT\n";
    cout << "=============================================\n";
    cout << left << setw(18) << "Receipt No:"      << "R-" << e.id << "\n";
    cout << left << setw(18) << "Event ID:"        << e.id << "\n";
    cout << left << setw(18) << "Customer:"        << e.customer << "\n";
    cout << left << setw(18) << "Phone:"           << e.phone << "\n";
    cout << left << setw(18) << "Date:"            << dateToString(e.date) << "\n";
    cout << left << setw(18) << "Venue:"           << e.venue.type << " (" << e.venue.location << ")\n";
    cout << left << setw(18) << "Guests:"          << e.guestCount << "\n";
    cout << left << setw(18) << "Cuisine:"         << e.menu.cuisine << "\n";
    cout << left << setw(18) << "Serving Style:"   << e.menu.servingStyle << "\n";
    cout << left << setw(18) << "Menu Items:";
    if (!e.menu.foodItems.empty()) {
        for (size_t i = 0; i < e.menu.foodItems.size(); ++i) {
            cout << e.menu.foodItems[i];
            if (i < e.menu.foodItems.size() - 1) cout << ", ";
        }
    } else {
        cout << "None";
    }
    cout << "\n";
    cout << left << setw(18) << "Custom Notes:"           << (e.notes.empty() ? "-" : e.notes) << "\n";
    cout << left << setw(18) << "Payment Method:"  << e.paymentMethod << "\n";
    cout << left << setw(18) << "Total Amount (RM):"
         << fixed << setprecision(2) << e.totalCost << "\n";
    cout << "=============================================\n";
    cout << "  Thank you for your payment.\n";
    cout << "=============================================\n";

    cout << "Press enter to back to user page...";
    cin.get();
    return;
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

    // Capacity validation
    while (true) {
        cout << "Enter venue capacity : ";
        string input;
        getline(cin, input);
        trim(input);

        regex numberRegex("^[0-9]+$");

        if (!regex_match(input, numberRegex)) {
            cout << "Invalid input! Numbers only.\n";
            continue;
        }

        int capacity = stoi(input);
        if (capacity >= 100) {
            v.capacity = capacity;
            break;
        } else {
            cout << "Capacity must be at least 100! Please try again.\n";
        }
    }

    // Price validation
    while (true) {
        cout << "Enter price for venue rental: RM";
        string input;
        getline(cin, input);
        trim(input);

        regex numberRegex("^[0-9]+(\\.[0-9]+)?$");

        if (!regex_match(input, numberRegex)) {
            cout << "Invalid input! Numbers only.\n";
            continue;
        }

        double price = stod(input);
        if (price >= 1000) {
            v.price = price;
            break;
        } else {
            cout << "Price must be at least 1000! Please try again.\n";
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
    cout << "\n========================================\n";
    cout << "         List of Available Venues       \n";
    cout << "========================================\n";

    if (venues.empty()) {
        cout << "No venues available.\n";
        return;
    }

    cout << left << setw(8)  << "ID"
         << setw(20) << "Type"
         << setw(20) << "Location"
         << setw(15) << "Capacity"
         << setw(15) << "Price (RM)" << "\n";

    cout << string(70, '=') << "\n";

    for (const auto& v : venues) {
        cout << left << setw(8)  << v.id
             << setw(20) << v.type
             << setw(20) << v.location
             << setw(15) << v.capacity
             << setw(15) << fixed << setprecision(2) << v.price
             << "\n";
    }

    cout << string(70, '=') << "\n";
}

void mainMenu(vector<Event>& events, vector<Menu>& menus, vector<Venue>& venues, int& nextMenuID,int& nextEventID,int& nextVenueID) {
    int choice;
    do {
        cout << "\n===== Wedding Event Management System =====\n";
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "0. Exit\n";

        while (true) {
            cout << "Enter your choice: ";
            if (cin >> choice) {
                if (choice >= 0 && choice <= 2) {
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;
                }
            } else {
                cout << "Invalid input! Please enter a number.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }

        switch (choice) {
            case 1:
                registerUser();
                break;
            case 2:
                loginUser(events, menus, venues, nextMenuID, nextEventID,nextVenueID);
                break;
            case 0:
                cout << "Thank you for using our system. Have a nice day!\n";
                break;
            default:
                cout << "Invalid choice! Please enter a number between 1-3.\n";
        }
    } while (choice != 0);
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
    loadDataFromFile(events,nextEventID);
    loadFeedbacks(events);


    cout << "=============================================\n";
    cout << "     Wedding Event Management System\n";
    cout << "=============================================\n";

    mainMenu(events, menus, venues, nextMenuID, nextEventID,nextVenueID);
    return 0;
}
