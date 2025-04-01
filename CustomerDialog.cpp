#include "CustomerDialog.h"
#include <QFormLayout>
#include <QMessageBox>
#include <QHeaderView>
#include <QGroupBox>
#include <QHBoxLayout>

CustomerDialog::CustomerDialog(QWidget *parent, pqxx::connection* conn)
    : QDialog(parent), dbConnection(conn)
{
    setWindowTitle("Customer Management");
    setMinimumWidth(600);
    setMinimumHeight(400);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Create form group
    QGroupBox *formGroup = new QGroupBox("Add Customer");
    QFormLayout *formLayout = new QFormLayout;

    nameEdit = new QLineEdit(this);
    addressEdit = new QLineEdit(this);
    phoneEdit = new QLineEdit(this);
    emailEdit = new QLineEdit(this);

    formLayout->addRow("Name:", nameEdit);
    formLayout->addRow("Address:", addressEdit);
    formLayout->addRow("Phone:", phoneEdit);
    formLayout->addRow("Email:", emailEdit);

    addButton = new QPushButton("Add Customer", this);
    formLayout->addRow(addButton);

    formGroup->setLayout(formLayout);
    mainLayout->addWidget(formGroup);

    // Create table
    customerTable = new QTableWidget(this);
    setupTable();
    mainLayout->addWidget(customerTable);

    // Create button layout
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    viewButton = new QPushButton("Refresh List", this);
    removeButton = new QPushButton("Remove Selected", this);
    buttonLayout->addWidget(viewButton);
    buttonLayout->addWidget(removeButton);
    mainLayout->addLayout(buttonLayout);

    // Connect signals
    connect(addButton, &QPushButton::clicked, this, &CustomerDialog::handleAddCustomer);
    connect(viewButton, &QPushButton::clicked, this, &CustomerDialog::handleViewCustomers);
    connect(removeButton, &QPushButton::clicked, this, &CustomerDialog::handleRemoveCustomer);

    // Initial load
    handleViewCustomers();
}

void CustomerDialog::setupTable()
{
    customerTable->setColumnCount(5);
    customerTable->setHorizontalHeaderLabels({"ID", "Name", "Address", "Phone", "Email"});
    customerTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    customerTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    customerTable->setSelectionMode(QAbstractItemView::SingleSelection);
}

void CustomerDialog::handleAddCustomer()
{
    QString name = nameEdit->text().trimmed();
    QString address = addressEdit->text().trimmed();
    QString phone = phoneEdit->text().trimmed();
    QString email = emailEdit->text().trimmed();

    if (name.isEmpty() || address.isEmpty() || phone.isEmpty() || email.isEmpty()) {
        QMessageBox::warning(this, "Error", "All fields are required!");
        return;
    }

    try {
        pqxx::work txn(*dbConnection);
        txn.exec_params(
            "INSERT INTO customers (customer_name, customer_address, phone_number, customer_email, total_due) "
            "VALUES ($1, $2, $3, $4, 0.00)",
            name.toStdString(),
            address.toStdString(),
            phone.toStdString(),
            email.toStdString()
        );
        txn.commit();

        QMessageBox::information(this, "Success", "Customer added successfully!");
        clearForm();
        handleViewCustomers();
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("Failed to add customer: %1").arg(e.what()));
    }
}

void CustomerDialog::handleViewCustomers()
{
    try {
        pqxx::work txn(*dbConnection);
        pqxx::result res = txn.exec(
            "SELECT customer_id, customer_name, customer_address, phone_number, customer_email "
            "FROM customers ORDER BY customer_id"
        );

        customerTable->setRowCount(res.size());
        
        for (size_t i = 0; i < res.size(); ++i) {
            customerTable->setItem(i, 0, new QTableWidgetItem(QString::number(res[i][0].as<int>())));
            customerTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(res[i][1].as<std::string>())));
            customerTable->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(res[i][2].as<std::string>())));
            customerTable->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(res[i][3].as<std::string>())));
            customerTable->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(res[i][4].as<std::string>())));
        }
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("Failed to load customers: %1").arg(e.what()));
    }
}

void CustomerDialog::handleRemoveCustomer()
{
    QModelIndexList selection = customerTable->selectionModel()->selectedRows();
    if (selection.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please select a customer to remove!");
        return;
    }

    int row = selection.first().row();
    int customerId = customerTable->item(row, 0)->text().toInt();

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Confirm Deletion",
        "Are you sure you want to remove this customer?",
        QMessageBox::Yes | QMessageBox::No
    );

    if (reply == QMessageBox::Yes) {
        try {
            pqxx::work txn(*dbConnection);
            txn.exec_params("DELETE FROM customers WHERE customer_id = $1", customerId);
            txn.commit();

            QMessageBox::information(this, "Success", "Customer removed successfully!");
            handleViewCustomers();
        } catch (const std::exception& e) {
            QMessageBox::critical(this, "Error", QString("Failed to remove customer: %1").arg(e.what()));
        }
    }
}

void CustomerDialog::clearForm()
{
    nameEdit->clear();
    addressEdit->clear();
    phoneEdit->clear();
    emailEdit->clear();
} 