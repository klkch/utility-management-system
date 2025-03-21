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

void Customer::addBill(Bill bill){
  this->bills.push_back(bill);
}

int Customer::balanceDue()const{
  int total = 0;
  for(const Bill i:bills){
	total += i.getDue();
  }
	return total;
}