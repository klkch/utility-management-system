//
// Created by danie on 3/13/2025.
//

#include "Service.h"

Service::Service(string name, int cost){
  this->name = name;
  this->bill_cost = cost;

  this->customers = customers;
}

vector<Customer> Service::getCustomers()const{
  return customers;
}

string Service::getName()const{
  return name;
}
int Service::getCost()const{
  return bill_cost;
}

void Service::billCustomers(){
    for(Customer i:customers){

    }

}