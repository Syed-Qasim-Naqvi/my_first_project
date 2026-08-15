#ifndef FILE_MANAGERS_H
#define FILE_MANAGERS_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

struct Customer {
    string cnic;
    string password;
    string fullName;
    string gender;
    string phone;
    string city;
    int isBlocked; // 0 for false,1 for true
};

struct CatalogItem {
    string filterID;
    string filterName;
    string category;
    int isEnabled; // 0 for false,1 for true
};

struct Session {
    string cnic;
    string timestamp;
    string filtersApplied;
    string outputFile;
};

// customer Manager 
class CustomerManager {
private:
    string filename;
    vector<Customer> customers;

public:
    CustomerManager(string file = "customers.txt");
    void loadCustomers();
    void saveCustomers();
    Customer* searchCustomer(const string& cnic);
    bool blockCustomer(const string& cnic);
    bool deleteCustomer(const string& cnic);
    void addCustomer(const Customer& c);
};

//  catalog Manager 
class CatalogManager {
private:
    string filename;
    vector<CatalogItem> catalog;

public:
    CatalogManager(string file = "catalog.txt");
    void loadCatalog();
    void saveCatalog();
    bool toggleFilter(const string& filterID);
    void displayEnabledFilters() const;
};

//  session manager 
class SessionManager {
private:
    string filename;

public:
    SessionManager(string file = "sessions.txt");
    void appendSession(const Session& session);
    vector<Session> loadSessionsForCNIC(const string& cnic);
};

#endif