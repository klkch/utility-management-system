#include <iostream>
#include <pqxx/pqxx>
#include "Customer.h"
#include "Provider.h"
#include "Invoice.h"
#include <QApplication>
#include "MainWindow.h"


using namespace std;

void showMainMenu() {
    cout << "\n==== Utility Service Management System ====\n";
    cout << "1. Manage Customers\n";
    cout << "2. Manage Providers\n";
    cout << "3. Manage Invoices\n";
    cout << "4. Exit\n";
    cout << "Select an option: ";
}

void showCustomerMenu() {
    cout << "\n-- Customer Management --\n";
    cout << "1. Add a Customer\n";
    cout << "2. View all Customers\n";
    cout << "3. Remove a Customer\n";
    cout << "4. Back to Main Menu\n";
    cout << "Select an option: ";
}

void showProviderMenu() {
    cout << "\n-- Provider Management --\n";
    cout << "1. Add a Provider\n";
    cout << "2. View all Providers\n";
    cout << "3. Remove a Provider\n";
    cout << "4. Edit a Provider\n";
    cout << "5. Back to Main Menu\n";
    cout << "Select an option: ";
}

void showInvoiceMenu() {
    cout << "\n-- Invoice Management --\n";
    cout << "1. Add Invoice\n";
    cout << "2. View All Invoices\n";
    cout << "3. Mark Invoice as Paid\n";
    cout << "4. Delete Invoice\n";
    cout << "5. Back to Main Menu\n";
    cout << "Select an option: ";
}

string selectServiceType() {
    vector<string> serviceTypes = {
        "Natural Gas",
        "Internet - TV",
        "Internet - Mobile Phone",
        "Internet - Home Phone",
        "Hydro - Electric",
        "Hydro - Water",
        "Hydro - Sewerage"
    };

    int choice = -1;
    while (true) {
        cout << "\nSelect Service Type:\n";
        for (int i = 0; i < serviceTypes.size(); ++i) {
            cout << "  " << (i + 1) << ". " << serviceTypes[i] << "\n";
        }
        cout << "Enter number (1-" << serviceTypes.size() << "): ";
        cin >> choice;

        if (cin.fail() || choice < 1 || choice > serviceTypes.size()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid choice. Try again.\n";
        } else {
            cin.ignore(10000, '\n'); 
            return serviceTypes[choice - 1];
        }
    }
}

void manageCustomers(pqxx::connection& c) {
    int choice;
    while (true) {
        showCustomerMenu();
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
            cin.ignore();
            Customer::removeCustomerById(c, id);

        } else if (choice == 4) {
            break;

        } else {
            cout << "Invalid choice. Please select 1-4." << endl;
        }
    }
}

void manageProviders(pqxx::connection& c) {
    int choice;
    while (true) {
        showProviderMenu();
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. Try again.\n";
            continue;
        }

        if (choice == 1) {
            string name, phone, email, serviceType;
            cin.ignore(); 
            cout << "Enter provider name: ";
            getline(cin, name);
            cout << "Enter phone number: ";
            getline(cin, phone);
            cout << "Enter email: ";
            getline(cin, email);
            serviceType = selectServiceType();

            Provider::addProviderToDB(c, name, phone, email, serviceType);

        } else if (choice == 2) {
            Provider::viewAllProviders(c);

        } else if (choice == 3) {
            int id;
            cout << "Enter Provider ID to remove: ";
            cin >> id;
            cin.ignore();

            // Check if provider exists before removing
            pqxx::nontransaction txn(c);
            pqxx::result res = txn.exec(pqxx::zview{"SELECT * FROM providers WHERE provider_id = $1"}, id);
;
            if (res.empty()) {
                cout << "Error: Provider with ID " << id << " does not exist.\n";
                continue;
            }

            Provider::removeProviderById(c, id);

        } else if (choice == 4) {
            int id;
            cout << "Enter Provider ID to edit: ";
            cin >> id;
            cin.ignore();

            // Check if provider exists before continuing
            pqxx::nontransaction txn(c);
            pqxx::result res = txn.exec(pqxx::zview{"SELECT * FROM providers WHERE provider_id = $1"}, id);
;
            if (res.empty()) {
                cout << "Error: Provider with ID " << id << " does not exist.\n";
                continue;
            }

            string name, phone, email, serviceType;
            cout << "Enter new provider name: ";
            getline(cin, name);
            cout << "Enter new phone number: ";
            getline(cin, phone);
            cout << "Enter new email: ";
            getline(cin, email);
            serviceType = selectServiceType();

            Provider::editProviderById(c, id, name, phone, email, serviceType);

        } else if (choice == 5) {
            break;

        } else {
            cout << "Invalid choice. Please select 1-5.\n";
        }
    }
}

void manageInvoices(pqxx::connection& c) {
    int choice;
    while (true) {
        showInvoiceMenu();
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. Try again.\n";
            continue;
        }

        if (choice == 1) {
            int customerId, providerId;
            string dueDate;
            double amountDue;
            cin.ignore();
            cout << "Enter Customer ID: ";
            cin >> customerId;
            cout << "Enter Provider ID: ";
            cin >> providerId;
            cin.ignore();
            cout << "Enter Due Date (YYYY-MM-DD): ";
            getline(cin, dueDate);
            cout << "Enter Amount Due: ";
            cin >> amountDue;

            Invoice::addInvoice(c, customerId, providerId, dueDate, amountDue);

        } else if (choice == 2) {
            Invoice::viewAllInvoices(c);

        } else if (choice == 3) {
            int id;
            cout << "Enter Invoice ID to mark as paid: ";
            cin >> id;
            Invoice::markInvoiceAsPaid(c, id);

        } else if (choice == 4) {
            int id;
            cout << "Enter Invoice ID to delete: ";
            cin >> id;
            Invoice::deleteInvoice(c, id);

        } else if (choice == 5) {
            break;

        } else {
            cout << "Invalid choice. Please select 1-5.\n";
        }
    }
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // basic css for the app
    app.setStyleSheet(R"(
        QDialog {
            background-color: #1E3D3B;
            color: white;
        }
        QLabel {
            color: white;
        }
        QPushButton {
            background-color: #2E5D5B;
            color: white;
            border: none;
            padding: 8px 16px;
            border-radius: 4px;
        }
        QPushButton:hover {
            background-color: #3E6D6B;
        }
        QPushButton:pressed {
            background-color: #1E3D3B;
        }
        QLineEdit, QComboBox, QDateEdit {
            background-color: #2E5D5B;
            color: white;
            border: 1px solid #4E7D7B;
            padding: 4px;
            border-radius: 4px;
        }
        QTableWidget {
            background-color: #2E5D5B;
            color: white;
            gridline-color: #4E7D7B;
        }
        QHeaderView::section {
            background-color: #1E3D3B;
            color: white;
            padding: 4px;
            border: 1px solid #4E7D7B;
        }
        QGroupBox {
            color: white;
            border: 1px solid #4E7D7B;
            margin-top: 1em;
        }
        QGroupBox::title {
            color: white;
        }
    )");

    MainWindow window;
    window.show();

    return app.exec();
}
