//
// Created by danie on 3/13/2025.
//

#include "Bill.h"
Bill::Bill(int cost,int c_id){
  this->customer_id = c_id;
  this->cost = cost;
  this->due = cost;
}

void Bill::payCost(int amount){
  int value = this->due-amount;
  if (value < 0){
    value=0;
  }
  this->due = value;
}

int Bill::getDue()const{
	return due;
}