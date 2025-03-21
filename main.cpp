#include <iostream>


#include "Service.h"
#include "Bill.h"
#include "Customer.h"
#include "Provider.h"
#include <string>

int main() {
    Customer customer1("Steve",12345);
    Customer customer2("Amy",63453);

    vector<Customer> customerlist;
    customerlist.emplace_back(customer1);
    customerlist.emplace_back(customer2);

    Service service1("Natural Gas",25,customerlist);
    vector<Service> servicelist;
    servicelist.emplace_back(service1);

    Provider provider1("Bayside's Power",servicelist);

    std::cout<<"asdadasd"+std::to_string(provider1.getRevenue());
}
