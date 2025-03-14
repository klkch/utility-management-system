//
// Created by danie on 3/13/2025.
//

#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>
#include <map>
#include "Service.h"

using namespace std;

class Customer {
    string name;
    int id;
    vector<Service> subscriptions;

  public:
    Customer(string, int,map<Service,int>);
    string getName()const;
    vector<Service> getSubscriptions()const;

};



#endif //CUSTOMER_H
