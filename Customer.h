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
    int customer_id;

  public:
    Customer(string,int);
    string getName()const;
    int getId()const;
};



#endif //CUSTOMER_H
