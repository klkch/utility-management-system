//
// Created by danie on 3/13/2025.
//

#ifndef SERVICE_H
#define SERVICE_H

#include <string>
#include "Customer.h"
#include "Bill.h"
#include <vector>

using namespace std;
class Service {
    string name;
    int bill_cost;

    vector<Customer> customers;
    vector<Bill> bills;

public:
    Service(string, int);

    string getName() const;
    int getCost() const;
	int getRevenue() const;
    vector<Customer> getCustomers() const;
    void billCustomers();
};



#endif //SERVICE_H
