#include <iostream>
#include <pqxx/pqxx>
#include "Customer.h"

using namespace std;

void showMenu() {
    cout << "\n\n\n";
    cout << "1. Add a Customer" << endl;
    cout << "2. View all Customers" << endl;
    cout << "3. Remove a Customer" << endl;
    cout << "4. Exit" << endl;
    cout << "Select an option: ";
}

int main() {
    try {
        pqxx::connection c(
            "host=aws-0-ca-central-1.pooler.supabase.com "
            "port=5432 "
            "dbname=postgres "
            "user=postgres.nbjzlzreuwryagkoqvgi "
            "password=COMP3400Project "
            "sslmode=require"
        );

        if (!c.is_open()) {
            cerr << "Unable to open database." << endl;
            return 1;
        }

        int choice;
        while (true) {
            showMenu();
            cin >> choice;

            if (cin.fail()) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Invalid input. Try again.\n";
                continue;
            }

            if (choice == 1) {
                string name, address, phone, email;
                cin.ignore(); 
                cout << "Enter name: ";
                getline(cin, name);
                cout << "Enter address: ";
                getline(cin, address);
                cout << "Enter phone number: ";
                getline(cin, phone);
                cout << "Enter email: ";
                getline(cin, email);

                Customer::addCustomerToDB(c, name, address, phone, email);

            } else if (choice == 2) {
                Customer::viewAllCustomers(c);

            } else if (choice == 3) {
                int id;
                cout << "Enter Customer ID to remove: ";
                cin >> id;
                Customer::removeCustomerById(c, id);

            } else if (choice == 4) {
                cout <<  "Goodbye" << endl;
                break;

            } else {
                cout << " Invalid choice. Please select 1-4." << endl;
            }
        }

    } catch (const exception& e) {
        cerr << "Connection failed: " << e.what() << endl;
        return 1;
    }

    return 0;
}
