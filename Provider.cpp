#include "Provider.h"

Provider::Provider(string name, int id, vector<Service> services){
    this->name = name;
    this->id = id;
    this->service_list = std::move(services);
}

string Provider::getProviderName()const{
  return name;
}

vector<Service> Provider::getServices()const{
  return service_list;
}


int Provider::getRevenue()const{
  int total_revenue=0;
  for(Service i:service_list){
    total_revenue += i.getRevenue();
  }
  return total_revenue;
}