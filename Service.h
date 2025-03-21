//
// Created by danie on 3/13/2025.
//

#ifndef SERVICE_H
#define SERVICE_H

#include <string>
#include <vector>
#include "Bill.h"
#include "Customer.h"

using namespace std;
class Service {
    string name;
    int bill_cost;

    vector<Customer> customers;

public:
    Service(string, int,vector<Customer>);

    string getName() const;
    int getCost() const;
	int getRevenue() const;
    vector<Customer> getCustomers() const;
    void billCustomers();

};



#endif //SERVICE_H
