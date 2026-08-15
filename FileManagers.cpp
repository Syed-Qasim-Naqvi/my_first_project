#include "FileManagers.h"

using namespace std;

// customerManager implementation basically manages the filters

CustomerManager::CustomerManager(string file) : filename(file) {
    loadCustomers();
}

void CustomerManager::loadCustomers() {
    customers.clear();
    ifstream file(filename);
    if (!file.is_open()) return; // file might not exist yet which is fine

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        Customer c;

        getline(ss, c.cnic, '|');
        getline(ss, c.password, '|');
        getline(ss, c.fullName, '|');
        getline(ss, c.gender, '|');
        getline(ss, c.phone, '|');
        getline(ss, c.city, '|');
        
        getline(ss, token, '|');
        c.isBlocked = (token == "1") ? 1 : 0;

        customers.push_back(c);
    }
    file.close();
}

void CustomerManager::saveCustomers() {
    ofstream file(filename, ios::trunc); // Overwrites the entire file
    if (!file.is_open()) {
        cerr << "Error opening " << filename << " for writing.\n";
        return;
    }

    for (const auto& c : customers) {
        file << c.cnic << "|" << c.password << "|" << c.fullName << "|" 
             << c.gender << "|" << c.phone << "|" << c.city << "|" 
             << c.isBlocked << "\n";
    }
    file.close();
}

void CustomerManager::addCustomer(const Customer& c) {
    customers.push_back(c);
    saveCustomers();
}

Customer* CustomerManager::searchCustomer(const string& cnic) {
    for (auto& c : customers) {
        if (c.cnic == cnic) return &c;
    }
    return nullptr;
}

bool CustomerManager::blockCustomer(const string& cnic) {
    Customer* c = searchCustomer(cnic);
    if (c) {
        c->isBlocked = 1;
        saveCustomers();
        
        // add to blocked_cnics.txt
        ofstream blockedFile("blocked_cnics.txt", ios::app);
        if (blockedFile.is_open()) {
            blockedFile << cnic << "\n";
            blockedFile.close();
        }
        return true;
    }
    return false;
}

bool CustomerManager::deleteCustomer(const string& cnic) {
    for (auto it = customers.begin(); it != customers.end(); ++it) {
        if (it->cnic == cnic) {
            customers.erase(it);
            saveCustomers(); // rewrites the file without the deleted line
            return true;
        }
    }
    return false;
}


// catalogManager implementation to highly classify the things

CatalogManager::CatalogManager(string file) : filename(file) {
    loadCatalog();
}

void CatalogManager::loadCatalog() {
    catalog.clear();
    ifstream file(filename);
    if (!file.is_open()) return;

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        CatalogItem item;

        getline(ss, item.filterID, '|');
        getline(ss, item.filterName, '|');
        getline(ss, item.category, '|');
        
        getline(ss, token, '|');
        item.isEnabled = (token == "1") ? 1 : 0;

        catalog.push_back(item);
    }
    file.close();
}

void CatalogManager::saveCatalog() {
    ofstream file(filename, ios::trunc);
    for (const auto& item : catalog) {
        file << item.filterID << "|" << item.filterName << "|" 
             << item.category << "|" << item.isEnabled << "\n";
    }
    file.close();
}

bool CatalogManager::toggleFilter(const string& filterID) {
    for (auto& item : catalog) {
        if (item.filterID == filterID) {
            item.isEnabled = (item.isEnabled == 1) ? 0 : 1;
            saveCatalog();
            return true;
        }
    }
    return false;
}

void CatalogManager::displayEnabledFilters() const {
    for (const auto& item : catalog) {
        if (item.isEnabled == 1) {
            cout << item.filterID << "  " << item.filterName 
                 << "  [" << item.category << "]\n";
        }
    }
}


// sessionManager implementation

SessionManager::SessionManager(string file) : filename(file) {}

void SessionManager::appendSession(const Session& session) {
     ofstream file(filename, ios::app); // Open in append mode
        if (file.is_open()) {
        file << session.cnic << "|" << session.timestamp << "|" 
             << session.filtersApplied << "|" << session.outputFile << "\n";
        file.close();
    }
}

vector<Session> SessionManager::loadSessionsForCNIC(const string& cnic) {
    vector<Session> userSessions;
    ifstream file(filename);
    if (!file.is_open()) return userSessions;

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        Session s;

        getline(ss, s.cnic, '|');
        getline(ss, s.timestamp, '|');
        getline(ss, s.filtersApplied, '|');
        getline(ss, s.outputFile, '|');

        if (s.cnic == cnic) {
            userSessions.push_back(s);
        }
    }
    file.close();
    return userSessions;
}