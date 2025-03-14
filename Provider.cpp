#include "Provider.h"

Provider::Provider(string name, int id, vector<Service> services, vector<Customer> customers){
    this->name = name;
    this->id = id;
    this->service_list = services;
    this->customers = customers;
}

string Provider::getProviderName()const{
  return name;
}

vector<Service> Provider::getServices()const{
  return service_list;
}

vector<Customer> Provider::getCustomers()const{
  return customers;
}

int Provider::getRevenue()const{
  return 1;
}