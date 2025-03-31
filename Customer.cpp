//
// Created by danie on 3/13/2025.
//

#include "Customer.h"
#include <iostream>
#include <pqxx/pqxx>

Customer::Customer(std::string name, int id) {
    this->name = name;
    this->customer_id = id;
}

std::string Customer::getName() const {
    return this->name;
}

int Customer::getId() const {
    return this->customer_id;
}

void Customer::addBill(Bill bill) {
    this->bills.push_back(bill);
}

int Customer::balanceDue() const {
    int total = 0;
    for (const Bill& i : bills) {
        total += i.getDue();
    }
    return total;
}

// Add customer to database 
void Customer::addCustomerToDB(pqxx::connection& c, const std::string& name, const std::string& address, const std::string& phone, const std::string& email) {
    try {
        pqxx::work txn(c);
        txn.exec_params(
            pqxx::zview{
                "INSERT INTO public.customers (customer_name, customer_address, phone_number, customer_email, total_due) VALUES ($1, $2, $3, $4, 0.00)"
            },
            name, address, phone, email
        );
        
        
        txn.commit();
        std::cout << "Customer added successfully.\n";
    } catch (const std::exception& e) {
        std::cerr << "Error adding customer: " << e.what() << "\n";
    }
}



// View all customers 
void Customer::viewAllCustomers(pqxx::connection& c) {
    try {
        pqxx::nontransaction txn(c);
        pqxx::result res = txn.exec("SELECT customer_id, customer_name, customer_address, phone_number, customer_email, total_due FROM public.customers");

        std::cout << "\nCustomer List:\n";
        for (auto row : res) {
            std::cout << "ID: " << row["customer_id"].as<int>() << ", "
                      << "Name: " << row["customer_name"].c_str() << ", "
                      << "Address: " << row["customer_address"].c_str() << ", "
                      << "Phone: " << row["phone_number"].c_str() << ", "
                      << "Email: " << row["customer_email"].c_str() << ", "
                      << "Total Due: $" << row["total_due"].as<std::string>() << "\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "Error viewing customers: " << e.what() << "\n";
    }
}

// Remove customer from database
void Customer::removeCustomerById(pqxx::connection& c, int customerId) {
    try {
        pqxx::work txn(c);
        txn.exec(pqxx::zview{"DELETE FROM public.customers WHERE customer_id = $1"}, customerId);
        txn.commit();
        std::cout << "Customer with ID " << customerId << " removed successfully.\n";
    } catch (const std::exception& e) {
        std::cerr << "Error removing customer: " << e.what() << "\n";
    }
}

