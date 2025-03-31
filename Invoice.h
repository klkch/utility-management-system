#ifndef INVOICE_H
#define INVOICE_H

#include <string>
#include <pqxx/pqxx>

class Invoice {
public:
    static void addInvoice(pqxx::connection& c, int customerId, int providerId, const std::string& dueDate, double amountDue);
    static void viewAllInvoices(pqxx::connection& c);
    static void markInvoiceAsPaid(pqxx::connection& c, int invoiceId);
    static void deleteInvoice(pqxx::connection& c, int invoiceId);
};

#endif 
