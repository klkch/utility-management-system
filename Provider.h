//
// Created by danie on 3/13/2025.
//

#ifndef PROVIDER_H
#define PROVIDER_H

#include <string>
#include <vector>
#include "Service.h"
#include "Customer.h"

using namespace std;
class Provider{
    string name;
    vector<Service> service_list;
    vector<Customer> customers;
    int id;

public:
    Provider(string, int, vector<Service>, vector<Customer>);

    string getProviderName() const;
    vector<Service> getServices() const;
    vector<Customer> getCustomers() const;
    int getRevenue() const;
};



#endif //PROVIDER_H
