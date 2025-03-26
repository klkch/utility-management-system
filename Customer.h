//
// Created by danie on 3/13/2025.
//

#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>
#include <vector>
#include <pqxx/pqxx>
#include "Bill.h"

using namespace std;

class Customer {
    string name;
    int customer_id;
    vector<Bill> bills;

public:
    Customer(string, int);
    string getName() const;
    int getId() const;
    void addBill(Bill);
    int balanceDue() const;

    // ✅ Database functions
    static void addCustomerToDB(pqxx::connection& c, const string& name, const string& address, const string& phone, const string& email);
    static void viewAllCustomers(pqxx::connection& c);
    static void removeCustomerById(pqxx::connection& c, int customerId);

    Customer& operator=(const Customer&) = default;
};

#endif // CUSTOMER_H
