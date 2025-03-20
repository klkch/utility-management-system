//
// Created by danie on 3/13/2025.
//

#include "Customer.h"

Customer::Customer(string name, int id){
  this->name = name;
  this->customer_id = id;

}

string Customer::getName()const{
  return this->name;
}

int Customer::getId()const{
  return this->customer_id;
}