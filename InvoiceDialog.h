#ifndef INVOICEDIALOG_H
#define INVOICEDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QTableWidget>
#include <QComboBox>
#include <QDateEdit>
#include <pqxx/pqxx>

class InvoiceDialog : public QDialog {
    Q_OBJECT

public:
    InvoiceDialog(QWidget *parent = nullptr, pqxx::connection* conn = nullptr);

private slots:
    void handleAddInvoice();
    void handleViewInvoices();
    void handleMarkAsPaid();
    void handleDeleteInvoice();
    void handleCustomerChanged(int index);
    void handleProviderChanged(int index);

private:
    void createForm();
    void clearForm();
    void setupTable();
    void refreshTable();
    void loadCustomers();
    void loadProviders();
    void handleViewOverdueInvoices();

    QPushButton *overdueButton;  
    QComboBox *customerCombo;
    QComboBox *providerCombo;
    QDateEdit *dueDateEdit;
    QLineEdit *amountEdit;
    QPushButton *addButton;
    QPushButton *viewButton;
    QPushButton *markPaidButton;
    QPushButton *deleteButton;
    QTableWidget *invoiceTable;
    pqxx::connection* dbConnection;
};

#endif // INVOICEDIALOG_H 