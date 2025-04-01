#include "InvoiceDialog.h"
#include <QFormLayout>
#include <QMessageBox>
#include <QHeaderView>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QDoubleValidator>
#include <QDate>

InvoiceDialog::InvoiceDialog(QWidget *parent, pqxx::connection* conn)
    : QDialog(parent), dbConnection(conn)
{
    setWindowTitle("Invoice Management");
    setMinimumWidth(800);
    setMinimumHeight(600);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QLabel *titleLabel = new QLabel("Add Invoice");
    titleLabel->setAlignment(Qt::AlignHCenter);
    mainLayout->addWidget(titleLabel);

    // GroupBox without title
    QGroupBox *formGroup = new QGroupBox;
    QFormLayout *formLayout = new QFormLayout;
    formLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);

    customerCombo = new QComboBox(this);
    providerCombo = new QComboBox(this);
    dueDateEdit = new QDateEdit(this);
    amountEdit = new QLineEdit(this);

    // Setup date edit
    dueDateEdit->setCalendarPopup(true);
    dueDateEdit->setDate(QDate::currentDate());
    dueDateEdit->setMinimumDate(QDate::currentDate());

    // Setup amount edit with validator
    QDoubleValidator *validator = new QDoubleValidator(0.00, 999999.99, 2, this);
    validator->setNotation(QDoubleValidator::StandardNotation);
    amountEdit->setValidator(validator);
    amountEdit->setPlaceholderText("0.00");

    formLayout->addRow("Customer:", customerCombo);
    formLayout->addRow("Provider:", providerCombo);
    formLayout->addRow("Due Date:", dueDateEdit);
    formLayout->addRow("Amount:", amountEdit);

    addButton = new QPushButton("Add Invoice", this);
    formLayout->addRow(addButton);

    formGroup->setLayout(formLayout);
    mainLayout->addWidget(formGroup);

    // Create table
    invoiceTable = new QTableWidget(this);
    setupTable();
    mainLayout->addWidget(invoiceTable);

    // Create button layout
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    viewButton = new QPushButton("Refresh List", this);
    markPaidButton = new QPushButton("Mark as Paid", this);
    deleteButton = new QPushButton("Delete Invoice", this);
    buttonLayout->addWidget(viewButton);
    buttonLayout->addWidget(markPaidButton);
    buttonLayout->addWidget(deleteButton);
    mainLayout->addLayout(buttonLayout);

    // Connect signals
    connect(addButton, &QPushButton::clicked, this, &InvoiceDialog::handleAddInvoice);
    connect(viewButton, &QPushButton::clicked, this, &InvoiceDialog::handleViewInvoices);
    connect(markPaidButton, &QPushButton::clicked, this, &InvoiceDialog::handleMarkAsPaid);
    connect(deleteButton, &QPushButton::clicked, this, &InvoiceDialog::handleDeleteInvoice);
    connect(customerCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &InvoiceDialog::handleCustomerChanged);
    connect(providerCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &InvoiceDialog::handleProviderChanged);

    // Load initial data
    loadCustomers();
    loadProviders();
    handleViewInvoices();
}

void InvoiceDialog::setupTable()
{
    invoiceTable->setColumnCount(7);
    invoiceTable->setHorizontalHeaderLabels({
        "ID", "Customer", "Provider", "Due Date", "Amount Due", "Status", "Total Due"
    });
    invoiceTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    invoiceTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    invoiceTable->setSelectionMode(QAbstractItemView::SingleSelection);
}

void InvoiceDialog::loadCustomers()
{
    try {
        customerCombo->clear();
        customerCombo->addItem("Select Customer", QVariant());

        pqxx::work txn(*dbConnection);
        pqxx::result res = txn.exec("SELECT customer_id, customer_name FROM customers ORDER BY customer_name");

        for (const auto& row : res) {
            int id = row["customer_id"].as<int>();
            QString name = QString::fromStdString(row["customer_name"].as<std::string>());
            customerCombo->addItem(name, id);
        }
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("Failed to load customers: %1").arg(e.what()));
    }
}

void InvoiceDialog::loadProviders()
{
    try {
        providerCombo->clear();
        providerCombo->addItem("Select Provider", QVariant());

        pqxx::work txn(*dbConnection);
        pqxx::result res = txn.exec("SELECT provider_id, provider_name FROM providers ORDER BY provider_name");

        for (const auto& row : res) {
            int id = row["provider_id"].as<int>();
            QString name = QString::fromStdString(row["provider_name"].as<std::string>());
            providerCombo->addItem(name, id);
        }
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("Failed to load providers: %1").arg(e.what()));
    }
}

void InvoiceDialog::handleAddInvoice()
{
    if (!customerCombo->currentData().isValid() || !providerCombo->currentData().isValid()) {
        QMessageBox::warning(this, "Error", "Please select both a customer and a provider!");
        return;
    }

    QString amountText = amountEdit->text().trimmed();
    if (amountText.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter an amount!");
        return;
    }

    int customerId = customerCombo->currentData().toInt();
    int providerId = providerCombo->currentData().toInt();
    QString dueDate = dueDateEdit->date().toString("yyyy-MM-dd");
    double amount = amountText.toDouble();

    try {
        pqxx::work txn(*dbConnection);
        txn.exec_params(
            "INSERT INTO invoices (customer_id, provider_id, due_date, amount_due, status) "
            "VALUES ($1, $2, $3, $4, 'PENDING')",
            customerId,
            providerId,
            dueDate.toStdString(),
            amount
        );

        // Update customer's total due
        txn.exec_params(
            "UPDATE customers SET total_due = total_due + $1 WHERE customer_id = $2",
            amount,
            customerId
        );

        txn.commit();

        QMessageBox::information(this, "Success", "Invoice added successfully!");
        clearForm();
        handleViewInvoices();
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("Failed to add invoice: %1").arg(e.what()));
    }
}

void InvoiceDialog::handleViewInvoices()
{
    try {
        pqxx::work txn(*dbConnection);
        pqxx::result res = txn.exec(
            "SELECT i.invoice_id, c.customer_name, p.provider_name, i.due_date, "
            "i.amount_due, i.status, c.total_due "
            "FROM invoices i "
            "JOIN customers c ON i.customer_id = c.customer_id "
            "JOIN providers p ON i.provider_id = p.provider_id "
            "ORDER BY i.due_date DESC"
        );

        invoiceTable->setRowCount(res.size());
        
        for (size_t i = 0; i < res.size(); ++i) {
            invoiceTable->setItem(i, 0, new QTableWidgetItem(QString::number(res[i][0].as<int>())));
            invoiceTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(res[i][1].as<std::string>())));
            invoiceTable->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(res[i][2].as<std::string>())));
            invoiceTable->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(res[i][3].as<std::string>())));
            invoiceTable->setItem(i, 4, new QTableWidgetItem(QString::number(res[i][4].as<double>(), 'f', 2)));
            invoiceTable->setItem(i, 5, new QTableWidgetItem(QString::fromStdString(res[i][5].as<std::string>())));
            invoiceTable->setItem(i, 6, new QTableWidgetItem(QString::number(res[i][6].as<double>(), 'f', 2)));
        }
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("Failed to load invoices: %1").arg(e.what()));
    }
}

void InvoiceDialog::handleMarkAsPaid()
{
    QModelIndexList selection = invoiceTable->selectionModel()->selectedRows();
    if (selection.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please select an invoice to mark as paid!");
        return;
    }

    int row = selection.first().row();
    int invoiceId = invoiceTable->item(row, 0)->text().toInt();
    QString status = invoiceTable->item(row, 5)->text();

    if (status == "PAID") {
        QMessageBox::warning(this, "Error", "This invoice is already marked as paid!");
        return;
    }

    try {
        pqxx::work txn(*dbConnection);
        
        // Get invoice details
        pqxx::result res = txn.exec_params(
            "SELECT customer_id, amount_due FROM invoices WHERE invoice_id = $1",
            invoiceId
        );

        if (res.empty()) {
            QMessageBox::warning(this, "Error", "Invoice not found!");
            return;
        }

        int customerId = res[0]["customer_id"].as<int>();
        double amount = res[0]["amount_due"].as<double>();

        // Update invoice status
        txn.exec_params(
            "UPDATE invoices SET status = 'PAID' WHERE invoice_id = $1",
            invoiceId
        );

        // Update customer's total due
        txn.exec_params(
            "UPDATE customers SET total_due = total_due - $1 WHERE customer_id = $2",
            amount,
            customerId
        );

        txn.commit();

        QMessageBox::information(this, "Success", "Invoice marked as paid successfully!");
        handleViewInvoices();
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("Failed to mark invoice as paid: %1").arg(e.what()));
    }
}

void InvoiceDialog::handleDeleteInvoice()
{
    QModelIndexList selection = invoiceTable->selectionModel()->selectedRows();
    if (selection.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please select an invoice to delete!");
        return;
    }

    int row = selection.first().row();
    int invoiceId = invoiceTable->item(row, 0)->text().toInt();
    QString status = invoiceTable->item(row, 5)->text();

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Confirm Deletion",
        "Are you sure you want to delete this invoice?\nThis action cannot be undone.",
        QMessageBox::Yes | QMessageBox::No
    );

    if (reply == QMessageBox::Yes) {
        try {
            pqxx::work txn(*dbConnection);
            
            // Get invoice details if not paid (to adjust customer's total due)
            if (status != "PAID") {
                pqxx::result res = txn.exec_params(
                    "SELECT customer_id, amount_due FROM invoices WHERE invoice_id = $1",
                    invoiceId
                );

                if (!res.empty()) {
                    int customerId = res[0]["customer_id"].as<int>();
                    double amount = res[0]["amount_due"].as<double>();

                    // Update customer's total due
                    txn.exec_params(
                        "UPDATE customers SET total_due = total_due - $1 WHERE customer_id = $2",
                        amount,
                        customerId
                    );
                }
            }

            // Delete the invoice
            txn.exec_params("DELETE FROM invoices WHERE invoice_id = $1", invoiceId);
            txn.commit();

            QMessageBox::information(this, "Success", "Invoice deleted successfully!");
            handleViewInvoices();
        } catch (const std::exception& e) {
            QMessageBox::critical(this, "Error", QString("Failed to delete invoice: %1").arg(e.what()));
        }
    }
}

void InvoiceDialog::clearForm()
{
    customerCombo->setCurrentIndex(0);
    providerCombo->setCurrentIndex(0);
    dueDateEdit->setDate(QDate::currentDate());
    amountEdit->clear();
} 

void InvoiceDialog::handleCustomerChanged(int index)
{
    Q_UNUSED(index);
}

void InvoiceDialog::handleProviderChanged(int index)
{
    Q_UNUSED(index);
}
