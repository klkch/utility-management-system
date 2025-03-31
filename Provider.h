//
// Created by danie on 3/13/2025.
//

#ifndef PROVIDER_H
#define PROVIDER_H

#include <string>
#include <vector>
#include "Service.h"
#include "Customer.h"

using namespace std;
class Provider{
    string name;
    vector<Service> service_list;

public:
    Provider(string, vector<Service>);

    string getProviderName() const;
    vector<Service> getServices() const;
    int getRevenue() const;

    static void addProviderToDB(pqxx::connection& c, const string& name, const string& phone, const string& email, const string& serviceType);
    static void viewAllProviders(pqxx::connection& c);
    static void removeProviderById(pqxx::connection& c, int providerId);
    static void editProviderById(pqxx::connection& c, int providerId, const string& newName, const string& newPhone, const string& newEmail, const string& newServiceType);
};



#endif //PROVIDER_H
