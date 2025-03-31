#include "Invoice.h"
#include <iostream>
#include <pqxx/pqxx>

// add invoice
void Invoice::addInvoice(pqxx::connection& c, int customerId, int providerId, const std::string& dueDate, double amountDue) {
    try {
        pqxx::work txn(c);
        
    
        txn.exec_params(
            "INSERT INTO invoices (customer_id, provider_id, due_date, amount_due) VALUES ($1, $2, $3, $4)",
            customerId, providerId, dueDate, amountDue
        );

        // Update customer's total_due
        txn.exec_params(
            "UPDATE customers SET total_due = total_due + $1 WHERE customer_id = $2",
            amountDue, customerId
        );

        txn.commit();
        std::cout << "Invoice added successfully.\n";
    } catch (const std::exception& e) {
        std::cerr << "Error adding invoice: " << e.what() << "\n";
    }
}


//view all invoices
void Invoice::viewAllInvoices(pqxx::connection& c) {
    try {
        pqxx::nontransaction txn(c);
        pqxx::result res = txn.exec("SELECT invoice_id, customer_id, provider_id, due_date, amount_due, status FROM invoices");

        std::cout << "\nInvoice List:\n";
        for (const auto& row : res) {
            std::cout << "ID: " << row["invoice_id"].as<int>()
                      << ", Customer ID: " << row["customer_id"].as<int>()
                      << ", Provider ID: " << row["provider_id"].as<int>()
                      << ", Due Date: " << row["due_date"].c_str()
                      << ", Amount Due: $" << row["amount_due"].as<std::string>()
                      << ", Status: " << row["status"].c_str() << "\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "Error viewing invoices: " << e.what() << "\n";
    }
}

// mark invoice as paid
void Invoice::markInvoiceAsPaid(pqxx::connection& c, int invoiceId) {
    try {
        pqxx::work txn(c);

        pqxx::result res = txn.exec_params(
            "SELECT customer_id, amount_due FROM invoices WHERE invoice_id = $1 AND status != 'PAID'",
            invoiceId
        );

        if (res.empty()) {
            std::cout << "Invoice not found or already marked as PAID.\n";
            return;
        }

        int customerId = res[0]["customer_id"].as<int>();
        double amountDue = res[0]["amount_due"].as<double>();

        // Update to PAID
        txn.exec_params(
            "UPDATE invoices SET status = 'PAID' WHERE invoice_id = $1",
            invoiceId
        );

        // Subtract paid amt from the customer's total_due
        txn.exec_params(
            "UPDATE customers SET total_due = total_due - $1 WHERE customer_id = $2",
            amountDue, customerId
        );

        txn.commit();
        std::cout << "Invoice marked as paid and customer's total due updated.\n";
    } catch (const std::exception& e) {
        std::cerr << "Error updating invoice: " << e.what() << "\n";
    }
}


// delete invoice
void Invoice::deleteInvoice(pqxx::connection& c, int invoiceId) {
    try {
        pqxx::work txn(c);
        txn.exec_params("DELETE FROM invoices WHERE invoice_id = $1", invoiceId);
        txn.commit();
        std::cout << "Invoice deleted successfully.\n";
    } catch (const std::exception& e) {
        std::cerr << "Error deleting invoice: " << e.what() << "\n";
    }
}
