//
// Created by danie on 3/13/2025.
//

#ifndef SERVICE_H
#define SERVICE_H

#include <string>
#include "Customer.h"
#include <vector>

using namespace std;
class Service {
    string name;
    int bill_cost;

public:
    Service(string, int);

    string getName() const;
    int getCost() const;

};



#endif //SERVICE_H
