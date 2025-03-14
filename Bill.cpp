//
// Created by danie on 3/13/2025.
//

#include "Bill.h"
Bill::Bill(int provider_id,int cost){
  this->provider_id = provider_id;
  this->cost = cost;
  this->due = cost;
}

void Bill::payCost(int amount){
  int value = this->due-amount;
  if (value < 0{
    value=0;
  }
  this->due = value;
}