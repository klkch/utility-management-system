//
// Created by danie on 3/13/2025.
//

#include "Service.h"

Service::Service(string name, int cost){
  this->name = name;
  this->bill_cost = cost;
}


string Service::getName()const{
  return name;
}
int Service::getCost()const{
  return bill_cost;
}
