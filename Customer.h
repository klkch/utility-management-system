//
// Created by danie on 3/13/2025.
//

#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>
#include "Bill.h"
#include <vector>

using namespace std;

class Customer {
    string name;
    int customer_id;
    vector<Bill> bills;

  public:
    Customer(string,int);
    string getName()const;
    int getId()const;
    void addBill(Bill);
	int balanceDue()const;

	Customer& operator=(const Customer&) = default;
};



#endif //CUSTOMER_H
