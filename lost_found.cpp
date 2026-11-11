
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
using namespace std;

const int MAX_ITEMS = 200;

// Structure to store item details
struct Item {
    int id;
    string type;
    string name;
    string category;
    string description;
    string location;
    string date;
    string studentID;
    bool matched;
    int matchedWith;
};

Item items[MAX_ITEMS];
int itemCount = 0;
int nextID = 1;

// Convert string into lowercase
string toLowerCase(string str) {
    transform(str.begin(), str.end(), str.begin(),
              [](unsigned char c) {
                  return tolower(c);
              });
    return str;
}

// Read an integer safely
int getNumber(string message) {
    string input;
    int number;

    while (true) {
        cout << message;
        getline(cin, input);

        stringstream ss(input);
        char extra;

        if (ss >> number && !(ss >> extra)) {
            return number;
        }

        cout << "Invalid input! Enter a number.\n";
    }
}

// Save records into file
void saveRecords() {
    ofstream file("items.txt");

    for (int i = 0; i < itemCount; i++) {
        file << items[i].id << "|"
             << items[i].type << "|"
             << items[i].name << "|"
             << items[i].category << "|"
             << items[i].description << "|"
             << items[i].location << "|"
             << items[i].date << "|"
             << items[i].studentID << "|"
             << items[i].matched << "|"
             << items[i].matchedWith << "\n";
    }

    file.close();
}

// Load records from file
void loadRecords() {
    ifstream file("items.txt");
    string line;

    itemCount = 0;
    nextID = 1;

    while (getline(file, line) && itemCount < MAX_ITEMS) {
        stringstream ss(line);
        string field;
        Item temp;

        try {
            if (!getline(ss, field, '|')) continue;
            temp.id = stoi(field);

            if (!getline(ss, temp.type, '|')) continue;
            if (!getline(ss, temp.name, '|')) continue;
            if (!getline(ss, temp.category, '|')) continue;
            if (!getline(ss, temp.description, '|')) continue;
            if (!getline(ss, temp.location, '|')) continue;
            if (!getline(ss, temp.date, '|')) continue;
            if (!getline(ss, temp.studentID, '|')) continue;

            if (!getline(ss, field, '|')) continue;
            temp.matched = (field == "1");

            if (!getline(ss, field, '|')) continue;
            temp.matchedWith = stoi(field);

            items[itemCount++] = temp;

            if (temp.id >= nextID) {
                nextID = temp.id + 1;
            }
        }
        catch (...) {
            continue;
        }
    }

    file.close();
}

// Student Login Module
bool studentLogin() {
    string username, password;

    cout << "\n====================================\n";
    cout << "       SMART CAMPUS SYSTEM\n";
    cout << "====================================\n";

    cout << "Student ID: ";
    getline(cin, username);

    cout << "Password: ";
    getline(cin, password);

    // Demo login credentials
    if (username == "STU101" && password == "1234") {
        cout << "\nLogin Successful!\n";
        cout << "Welcome to Smart Campus!\n";
        return true;
    }

    cout << "\nInvalid Student ID or Password!\n";
    return false;
}

// Function to display one item
void displayItem(const Item &item) {
    cout << "\n------------------------------------\n";
    cout << "Record ID   : " << item.id << endl;
    cout << "Item Type   : " << item.type << endl;
    cout << "Item Name   : " << item.name << endl;
    cout << "Category    : " << item.category << endl;
    cout << "Description : " << item.description << endl;
    cout << "Location    : " << item.location << endl;
    cout << "Date        : " << item.date << endl;
    cout << "Student ID  : " << item.studentID << endl;

    cout << "Status      : "
         << (item.matched ? "Matched" : "Not Matched")
         << endl;

    if (item.matched) {
        cout << "Matched With: " << item.matchedWith << endl;
    }

    cout << "------------------------------------\n";
}

// Common function to add lost or found item
void addItem(string itemType) {
    if (itemCount >= MAX_ITEMS) {
        cout << "\nRecord storage is full!\n";
        return;
    }

    Item newItem;

    newItem.id = nextID++;
    newItem.type = itemType;
    newItem.matched = false;
    newItem.matchedWith = -1;

    cout << "\nEnter Item Name: ";
    getline(cin, newItem.name);

    cout << "Enter Category (Electronics, Documents, etc.): ";
    getline(cin, newItem.category);

    cout << "Enter Description: ";
    getline(cin, newItem.description);

    cout << "Enter Location: ";
    getline(cin, newItem.location);

    cout << "Enter Date (DD-MM-YYYY): ";
    getline(cin, newItem.date);

    cout << "Enter Student ID: ";
    getline(cin, newItem.studentID);

    // Replace delimiter to protect file format
    string* fields[] = {
        &newItem.name, &newItem.category,
        &newItem.description, &newItem.location,
        &newItem.date, &newItem.studentID
    };

    for (string* field : fields) {
        replace(field->begin(), field->end(), '|', '/');
    }

    items[itemCount++] = newItem;

    saveRecords();

    cout << "\n" << itemType << " Item Reported Successfully!\n";
    cout << "Your Record ID is: " << newItem.id << endl;
}

// Module 2: Report Lost Item
void reportLostItem() {
    cout << "\n===== REPORT LOST ITEM =====\n";
    addItem("LOST");
}

// Module 3: Report Found Item
void reportFoundItem() {
    cout << "\n===== REPORT FOUND ITEM =====\n";
    addItem("FOUND");
}

// Module 4: Search Item
void searchItem() {
    string searchText;
    bool found = false;

    cout << "\nEnter Item Name or Category to Search: ";
    getline(cin, searchText);

    searchText = toLowerCase(searchText);

    for (int i = 0; i < itemCount; i++) {
        string name = toLowerCase(items[i].name);
        string category = toLowerCase(items[i].category);

        if (name.find(searchText) != string::npos ||
            category.find(searchText) != string::npos) {

            displayItem(items[i]);
            found = true;
        }
    }

    if (!found) {
        cout << "\nNo Matching Item Found!\n";
    }
}

// Check whether two items can match
bool isMatching(Item lost, Item found) {
    string lostName = toLowerCase(lost.name);
    string foundName = toLowerCase(found.name);

    string lostCategory = toLowerCase(lost.category);
    string foundCategory = toLowerCase(found.category);

    // Match if names are equal
    if (lostName == foundName) {
        return true;
    }

    // Match if category is same and one name
    // contains the other name
    if (lostCategory == foundCategory &&
        (lostName.find(foundName) != string::npos ||
         foundName.find(lostName) != string::npos)) {
        return true;
    }

    return false;
}

// Module 5: Automatic Lost and Found Matching
void autoMatch() {
    int matchCount = 0;

    cout << "\n===== AUTOMATIC MATCHING =====\n";

    for (int i = 0; i < itemCount; i++) {

        if (items[i].type != "LOST" || items[i].matched) {
            continue;
        }

        for (int j = 0; j < itemCount; j++) {

            if (items[j].type != "FOUND" || items[j].matched) {
                continue;
            }

            if (isMatching(items[i], items[j])) {

                items[i].matched = true;
                items[j].matched = true;

                items[i].matchedWith = items[j].id;
                items[j].matchedWith = items[i].id;

                cout << "\nMatch Found!\n";

                cout << "Lost Item ID : " << items[i].id << endl;
                cout << "Found Item ID: " << items[j].id << endl;
                cout << "Item Name    : " << items[i].name << endl;

                matchCount++;
                break;
            }
        }
    }

    if (matchCount == 0) {
        cout << "\nNo New Matches Found!\n";
    }
    else {
        cout << "\nTotal New Matches: " << matchCount << endl;
    }

    saveRecords();
}

// Module 6: View All Records
void viewAllRecords() {
    if (itemCount == 0) {
        cout << "\nNo Records Available!\n";
        return;
    }

    // Sort records by ID using Bubble Sort
    for (int i = 0; i < itemCount - 1; i++) {
        for (int j = 0; j < itemCount - i - 1; j++) {

            if (items[j].id > items[j + 1].id) {
                swap(items[j], items[j + 1]);
            }
        }
    }

    cout << "\n===== ALL LOST AND FOUND RECORDS =====\n";

    for (int i = 0; i < itemCount; i++) {
        displayItem(items[i]);
    }

    saveRecords();
}

// Module 7: Delete Record
void deleteRecord() {
    int deleteID;
    bool found = false;

    deleteID = getNumber("\nEnter Record ID to Delete: ");

    for (int i = 0; i < itemCount; i++) {

        if (items[i].id == deleteID) {

            // Clear the linked match if it exists
            int linkedID = items[i].matchedWith;

            if (items[i].matched) {
                for (int j = 0; j < itemCount; j++) {
                    if (items[j].id == linkedID) {
                        items[j].matched = false;
                        items[j].matchedWith = -1;
                        break;
                    }
                }
            }

            // Shift records to remove deleted item
            for (int j = i; j < itemCount - 1; j++) {
                items[j] = items[j + 1];
            }

            itemCount--;
            saveRecords();

            cout << "\nRecord Deleted Successfully!\n";
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "\nRecord ID Not Found!\n";
    }
}

// Module 8: Item Statistics
void itemStatistics() {
    int lostCount = 0;
    int foundCount = 0;
    int matchedCount = 0;

    for (int i = 0; i < itemCount; i++) {

        if (items[i].type == "LOST") {
            lostCount++;
        }
        else if (items[i].type == "FOUND") {
            foundCount++;
        }

        if (items[i].type == "LOST" && items[i].matched) {
            matchedCount++;
        }
    }

    cout << "\n===== ITEM STATISTICS =====\n";

    cout << "Total Records       : " << itemCount << endl;
    cout << "Total Lost Items    : " << lostCount << endl;
    cout << "Total Found Items   : " << foundCount << endl;
    cout << "Total Matched Pairs : " << matchedCount << endl;
    cout << "Unmatched Lost Items: "
         << lostCount - matchedCount << endl;
    cout << "Unmatched Found Items: "
         << foundCount - matchedCount << endl;
}

// Module 9: Export Report
void exportReport() {
    ofstream file("lost_found_report.txt");

    if (!file) {
        cout << "\nUnable to Create Report File!\n";
        return;
    }

    file << "====================================\n";
    file << " SMART CAMPUS LOST & FOUND REPORT\n";
    file << "====================================\n";

    for (int i = 0; i < itemCount; i++) {

        file << "\nRecord ID   : " << items[i].id << endl;
        file << "Item Type   : " << items[i].type << endl;
        file << "Item Name   : " << items[i].name << endl;
        file << "Category    : " << items[i].category << endl;
        file << "Description : " << items[i].description << endl;
        file << "Location    : " << items[i].location << endl;
        file << "Date        : " << items[i].date << endl;
        file << "Student ID  : " << items[i].studentID << endl;

        file << "Status      : "
             << (items[i].matched ? "Matched" : "Not Matched")
             << endl;

        file << "Matched With: " << items[i].matchedWith << endl;
        file << "------------------------------------\n";
    }

    file.close();

    cout << "\nReport Exported Successfully!\n";
    cout << "File Name: lost_found_report.txt\n";
}

// Main Menu
void displayMenu() {
    cout << "\n\n====================================\n";
    cout << " SMART CAMPUS LOST & FOUND SYSTEM\n";
    cout << "====================================\n";

    cout << "1. Student Login Information\n";
    cout << "2. Report Lost Item\n";
    cout << "3. Report Found Item\n";
    cout << "4. Search Item\n";
    cout << "5. Auto Match Lost & Found\n";
    cout << "6. View All Records\n";
    cout << "7. Delete Record\n";
    cout << "8. Item Statistics\n";
    cout << "9. Export Report\n";
    cout << "10. Exit\n";

    cout << "====================================\n";
}

// Main Function
int main() {
    int choice;
    bool loggedIn = false;

    loadRecords();

    cout << "Welcome to Smart Campus Lost & Found System!\n";

    // Login with maximum three attempts
    for (int attempt = 1; attempt <= 3; attempt++) {

        if (studentLogin()) {
            loggedIn = true;
            break;
        }

        cout << "Attempts Remaining: " << 3 - attempt << endl;
    }

    if (!loggedIn) {
        cout << "\nToo Many Failed Attempts. Exiting...\n";
        return 0;
    }

    // Main menu loop
    do {
        displayMenu();

        choice = getNumber("Enter Your Choice: ");

        switch (choice) {

            case 1:
                cout << "\nDemo Student ID: STU101\n";
                cout << "Demo Password: 1234\n";
                break;

            case 2:
                reportLostItem();
                break;

            case 3:
                reportFoundItem();
                break;

            case 4:
                searchItem();
                break;

            case 5:
                autoMatch();
                break;

            case 6:
                viewAllRecords();
                break;

            case 7:
                deleteRecord();
                break;

            case 8:
                itemStatistics();
                break;

            case 9:
                exportReport();
                break;

            case 10:
                saveRecords();
                cout << "\nThank You for Using Smart Campus System!\n";
                cout << "Exiting Program...\n";
                break;

            default:
                cout << "\nInvalid Choice! Please Try Again.\n";
        }

    } while (choice != 10);

    return 0;
}
