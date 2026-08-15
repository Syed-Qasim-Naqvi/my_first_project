#ifndef USERS_H
#define USERS_H
#include <iostream>
#include <string>
using namespace std;
class User {
protected:
    string username;
public:
    User(string name) : username(name) {
        cout << " user base constructor called for " << username << "\n";
    }
    virtual ~User() {
        cout << " user base destructor called\n";
    }
    virtual void showRole() const = 0; 
};

class ILogger {
public:
    ILogger() { cout << " ILogger Constructor called\n"; }
    virtual ~ILogger() { cout << " ILogger Destructor called\n"; }
    void logAction(const string& msg) const {
        cout << "Log: " << msg << "\n";
    }
};

class CustomerUser : public User, public ILogger {
public:
    CustomerUser(string name) : User(name) {
        std::cout << " customerUser constructor called\n";
    }
    ~CustomerUser() {
        cout << " customerUser destructor called\n";
    }
    void showRole() const override {
        cout << "Role: Customer\n";
    }
};

class AdminUser : public User {
public:
    AdminUser(string name) : User(name) {
        cout << " adminUser constructor called\n";
    }
    ~AdminUser() {
        cout << "adminUser destructor called\n";
    }
    void showRole() const override {
        cout << "role: administrator\n";
    }
};
#endif