#include <iostream>
#include <pqxx/pqxx>
#include "Provider.h"

Provider::Provider(string name, vector<Service> services){
    this->name = name;
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

// adding provider to DB
void Provider::addProviderToDB(pqxx::connection& c, const string& name, const string& phone, const string& email, const string& serviceType) {
  try {
      pqxx::work txn(c);
      txn.exec_params(
          "INSERT INTO providers (provider_name, phone_number, provider_email, service_type) VALUES ($1, $2, $3, $4)",
          name, phone, email, serviceType
      );
      txn.commit();
      cout << "Provider added successfully.\n";
  } catch (const std::exception& e) {
      cerr << "Error adding provider: " << e.what() << "\n";
  }
}

// ouputting all providers from DB
void Provider::viewAllProviders(pqxx::connection& c) {
  try {
      pqxx::nontransaction txn(c);
      pqxx::result res = txn.exec("SELECT provider_id, provider_name, phone_number, provider_email, service_type FROM providers");

      cout << "\nProvider List:\n";
      for (auto row : res) {
          cout << "ID: " << row["provider_id"].as<int>()
               << ", Name: " << row["provider_name"].c_str()
               << ", Phone: " << row["phone_number"].c_str()
               << ", Email: " << row["provider_email"].c_str()
               << ", Service Type: " << row["service_type"].c_str() << "\n";
      }
  } catch (const std::exception& e) {
      cerr << "Error viewing providers: " << e.what() << "\n";
  }
}

// remove provider from DB
void Provider::removeProviderById(pqxx::connection& c, int providerId) {
  try {
      pqxx::work txn(c);
      txn.exec_params("DELETE FROM providers WHERE provider_id = $1", providerId);
      txn.commit();
      cout << "Provider removed successfully.\n";
  } catch (const std::exception& e) {
      cerr << "Error removing provider: " << e.what() << "\n";
  }
}

// edit an existing provider
void Provider::editProviderById(pqxx::connection& c, int providerId, const string& newName, const string& newPhone, const string& newEmail, const string& newServiceType) {
  try {
      pqxx::work txn(c);

      //check if provider exists
      pqxx::result check = txn.exec_params(
          "SELECT provider_id FROM providers WHERE provider_id = $1",
          providerId
      );

      if (check.empty()) {
          cout << "Error: Provider with ID " << providerId << " does not exist.\n";
          return; 
      }

      txn.exec_params(
          "UPDATE providers SET provider_name = $1, phone_number = $2, provider_email = $3, service_type = $4 WHERE provider_id = $5",
          newName, newPhone, newEmail, newServiceType, providerId
      );

      txn.commit();
      cout << "Provider updated successfully.\n";
  } catch (const std::exception& e) {
      cerr << "Error editing provider: " << e.what() << "\n";
  }
}
