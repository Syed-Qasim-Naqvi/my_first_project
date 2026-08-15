#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <fstream>
#include <stdexcept>
#include "FileManagers.h"
#include "Image.h"
#include "Filter.h"
#include "FilterSession.h"
#include "Users.h"

using namespace std;


// Validation Helpers

bool isValidCNIC(const string& cnic) {
    if (cnic.length() != 13) return false;
    for (char c : cnic) {
        if (!isdigit(c)) return false;
    }
    return true;
}

bool isValidPassword(const string& pass) {
    if (pass.length() != 9) return false;
    bool hasUpper = false, hasDigit = false;
    for (char c : pass) {
        if (isupper(c)) hasUpper = true;
        if (isdigit(c)) hasDigit = true;
    }
    return hasUpper && hasDigit;
}

bool isCNICBlocked(const string& cnic) {
    ifstream file("blocked_cnics.txt");
    string line;
    while (getline(file, line)) {
        if (line == cnic) return true;
    }
    return false;
}


// User Flows


void registerCustomer(CustomerManager& cm) {
    cout << "\n REGISTER NEW CUSTOMER \n";
    Customer newCust;
    string confirmPass;

    do {
        cout << "Enter CNIC (13 digits): ";
        cin >> newCust.cnic;
        if (!isValidCNIC(newCust.cnic)) cout << "Error: CNIC must be exactly 13 digits.\n";
        else if (cm.searchCustomer(newCust.cnic) != nullptr) cout << "Error: CNIC already registered.\n";
        else if (isCNICBlocked(newCust.cnic)) cout << "Error: This CNIC is blocked.\n";
    } while (!isValidCNIC(newCust.cnic) || cm.searchCustomer(newCust.cnic) != nullptr || isCNICBlocked(newCust.cnic));

    do {
        cout << "Enter Password (9 chars, 1 uppercase, 1 digit): ";
        cin >> newCust.password;
        cout << "Confirm Password: ";
        cin >> confirmPass;
        if (!isValidPassword(newCust.password)) cout << "Error: Password does not meet criteria.\n";
        else if (newCust.password != confirmPass) cout << "Error: Passwords do not match.\n";
    } while (!isValidPassword(newCust.password) || newCust.password != confirmPass);

    cout << "Enter Full Name: ";
    cin.ignore();
    getline(cin, newCust.fullName);
    
    cout << "Enter Gender (M/F/Other): ";
    cin >> newCust.gender;
    
    cout << "Enter Phone Number: ";
    cin >> newCust.phone;
    
    cout << "Enter City: ";
    cin >> newCust.city;

    newCust.isBlocked = 0;

    cm.addCustomer(newCust);
    cout << "Registration successful! You can now log in.\n";
}

string customerLogin(CustomerManager& cm) {
    cout << "\n CUSTOMER LOGIN \n";
    string cnic, pass;
    int attempts = 0;

    while (attempts < 3) {
        cout << "CNIC: ";
        cin >> cnic;
        cout << "Password: ";
        cin >> pass;

        Customer* cust = cm.searchCustomer(cnic);
        if (cust != nullptr && cust->password == pass) {
            if (cust->isBlocked == 1) {
                cout << "Access Denied: Account is blocked.\n";
                return "";
            }
            return cnic;
        } else {
            attempts++;
            cout << "Invalid credentials. Attempts remaining: " << (3 - attempts) << "\n";
        }
    }
    return "";
}


// Admin Main Panel



void adminPanel(CustomerManager& cm, CatalogManager& catalog) {
    cout << "\n--- Initializing Admin Session ---\n";
    AdminUser adminObj("admin"); 
    adminObj.showRole();
    

    int choice;
    do {
        cout << "\n ADMIN PANEL \n";
        cout << "1. Block a Customer\n";
        cout << "2. Delete a Customer\n";
        cout << "3. Toggle a Filter (Enable/Disable)\n";
        cout << "4. Logout\n";
        cout << "Your choice: ";
        cin >> choice;

        if (choice == 1) {
            string cnic;
            cout << "Enter Customer CNIC to block: ";
            cin >> cnic;
            if (cm.blockCustomer(cnic)) cout << "Success: Customer blocked.\n";
            else cout << "Error: Customer not found.\n";
        } 
        else if (choice == 2) {
            string cnic;
            cout << "Enter Customer CNIC to delete: ";
            cin >> cnic;
            if (cm.deleteCustomer(cnic)) cout << "Success: Customer deleted from database.\n";
            else cout << "Error: Customer not found.\n";
        } 
        else if (choice == 3) {
            string filterID;
            cout << "\n--- Current Enabled Filters ---\n";
            catalog.displayEnabledFilters();
            cout << "\nEnter Filter ID to toggle (e.g., 08): ";
            cin >> filterID;
            if (catalog.toggleFilter(filterID)) cout << "Success: Filter status updated.\n";
            else cout << "Error: Filter ID not found.\n";
        }
    } while (choice != 4);

    cout << "\n--- Ending Admin Session ---\n";
}


// Customer Main Panel



void customerPanel(const string& activeCNIC, CustomerManager& cm, CatalogManager& catalog) {
    cout << "\n--- Initializing User Session ---\n";
    CustomerUser activeUser(activeCNIC); 
    activeUser.showRole();
    activeUser.logAction("Customer Panel Accessed");
    

    int choice;
    do {
        cout << "\n=== CUSTOMER PANEL ===\n";
        cout << "1. Apply Filters to an Image\n";
        cout << "2. Logout\n";
        cout << "Your choice: ";
        cin >> choice;

        if (choice == 1) {
            string imagePath;
            cout << "\nEnter image filename (e.g., photo.jpg): ";
            cin >> imagePath;

            try {
                Image myImage(imagePath);
                
                cout << "Image loaded successfully!\n\n--- Available Filters ---\n";
                catalog.displayEnabledFilters();
                
                FilterSession session; 
                string filterChoice;
                
                cout << "\nEnter Filter IDs (type '0' to stop and apply): \n";
                
                while (true) {
                    cout << "Filter ID: ";
                    cin >> filterChoice;
                    
                    if (filterChoice == "0") break;

                    if (filterChoice == "01" || filterChoice == "1") { session.addFilter(new GrayscaleFilter()); cout << "   [Added Grayscale]\n"; }
                    else if (filterChoice == "02" || filterChoice == "2") { session.addFilter(new InvertFilter()); cout << "   [Added Invert]\n"; }
                    else if (filterChoice == "03" || filterChoice == "3") {
                        int amt; cout << "   Brightness amount (-100 to 100): "; cin >> amt;
                        session.addFilter(new BrightnessAdjustFilter(amt));
                        cout << "   [Added Brightness Adjust]\n";
                    }
                    else if (filterChoice == "04" || filterChoice == "4") { session.addFilter(new ContrastStretchFilter()); cout << "   [Added Contrast Stretch]\n"; }
                    else if (filterChoice == "05" || filterChoice == "5") { session.addFilter(new RedChannelFilter()); cout << "   [Added Red Channel]\n"; }
                    else if (filterChoice == "06" || filterChoice == "6") { session.addFilter(new GreenChannelFilter()); cout << "   [Added Green Channel]\n"; }
                    else if (filterChoice == "07" || filterChoice == "7") { session.addFilter(new BlueChannelFilter()); cout << "   [Added Blue Channel]\n"; }
                    else if (filterChoice == "08" || filterChoice == "8") { session.addFilter(new BoxBlurFilter()); cout << "   [Added Box Blur]\n"; }
                    else if (filterChoice == "09" || filterChoice == "9") { session.addFilter(new FlipHorizontalFilter()); cout << "   [Added Flip Horizontal]\n"; }
                    else if (filterChoice == "10") { session.addFilter(new FlipVerticalFilter()); cout << "   [Added Flip Vertical]\n"; }
                    else { cout << "Invalid ID.\n"; }
                }

                if (!session.isEmpty()) {
                    session.applyPipeline(myImage); 
                    
                    cout << "\n=== ASCII Preview (Scaled Down) ===\n";
                    myImage.displayASCII(); 
                    

                    string outputPath = activeCNIC + "_output.png";
                    myImage.savePNG(outputPath);
                    cout << "\nProcess complete! Saved as: " << outputPath << "\n";
                }
            } catch (const exception& e) {
                cout << "\n[EXCEPTION CAUGHT]: " << e.what() << "\nReturning to menu...\n";
            }
        }
    } while (choice != 2);
    
    cout << "\n--- Ending User Session ---\n";
}


// Application Entry Point



int main() {
    CustomerManager cm("customers.txt");
    CatalogManager catalog("catalog.txt");
    SessionManager sm("sessions.txt");

    int choice;
    do {
        
        cout << "     IMAGE FILTER STUDIO\n";
        
        cout << "1. Admin Login\n";
        cout << "2. Customer Login\n";
        cout << "3. New Customer? Register here\n";
        cout << "4. Exit\n";
        cout << "Your choice: ";
        cin >> choice;

        if (choice == 1) {
            string user, pass;
            cout << "Admin Username: "; cin >> user;
            cout << "Admin Password: "; cin >> pass;
            if (user == "admin" && pass == "admin123") {
                adminPanel(cm, catalog); // now calls the new admin panel
            } else {
                cout << "Invalid Admin credentials.\n";
            }
        } 
        else if (choice == 2) {
            string activeCNIC = customerLogin(cm);
            if (!activeCNIC.empty()) {
                customerPanel(activeCNIC, cm, catalog);
            }
        } 
        else if (choice == 3) {
            registerCustomer(cm);
        }

    } while (choice != 4);

    cout << "Exiting Image Filter Studio. Goodbye!\n";
    return 0;
}