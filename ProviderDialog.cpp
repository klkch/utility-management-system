#include "ProviderDialog.h"
#include <QFormLayout>
#include <QMessageBox>
#include <QHeaderView>
#include <QGroupBox>
#include <QHBoxLayout>

ProviderDialog::ProviderDialog(QWidget *parent, pqxx::connection* conn)
    : QDialog(parent), dbConnection(conn), isEditing(false), editingId(-1)
{
    setWindowTitle("Provider Management");
    setMinimumWidth(600);
    setMinimumHeight(400);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Create form group
    QGroupBox *formGroup = new QGroupBox("Add/Edit Provider");
    QFormLayout *formLayout = new QFormLayout;

    nameEdit = new QLineEdit(this);
    phoneEdit = new QLineEdit(this);
    emailEdit = new QLineEdit(this);
    serviceTypeCombo = new QComboBox(this);

    // Add service types
    serviceTypeCombo->addItem("Natural Gas");
    serviceTypeCombo->addItem("Internet - TV");
    serviceTypeCombo->addItem("Internet - Mobile Phone");
    serviceTypeCombo->addItem("Internet - Home Phone");
    serviceTypeCombo->addItem("Hydro - Electric");
    serviceTypeCombo->addItem("Hydro - Water");
    serviceTypeCombo->addItem("Hydro - Sewerage");

    formLayout->addRow("Name:", nameEdit);
    formLayout->addRow("Phone:", phoneEdit);
    formLayout->addRow("Email:", emailEdit);
    formLayout->addRow("Service Type:", serviceTypeCombo);

    // Create button layout for form
    QHBoxLayout *formButtonLayout = new QHBoxLayout;
    addButton = new QPushButton("Add Provider", this);
    updateButton = new QPushButton("Update Provider", this);
    updateButton->setVisible(false);  // Initially hidden
    formButtonLayout->addWidget(addButton);
    formButtonLayout->addWidget(updateButton);
    formLayout->addRow(formButtonLayout);

    formGroup->setLayout(formLayout);
    mainLayout->addWidget(formGroup);

    // Create table
    providerTable = new QTableWidget(this);
    setupTable();
    mainLayout->addWidget(providerTable);

    // Create button layout
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    viewButton = new QPushButton("Refresh List", this);
    removeButton = new QPushButton("Remove Selected", this);
    buttonLayout->addWidget(viewButton);
    buttonLayout->addWidget(removeButton);
    mainLayout->addLayout(buttonLayout);

    // Connect signals
    connect(addButton, &QPushButton::clicked, this, &ProviderDialog::handleAddProvider);
    connect(updateButton, &QPushButton::clicked, this, &ProviderDialog::handleEditProvider);
    connect(viewButton, &QPushButton::clicked, this, &ProviderDialog::handleViewProviders);
    connect(removeButton, &QPushButton::clicked, this, &ProviderDialog::handleRemoveProvider);
    connect(providerTable, &QTableWidget::itemSelectionChanged, this, &ProviderDialog::handleSelectionChanged);

    // Initial load
    handleViewProviders();
}

void ProviderDialog::setupTable()
{
    providerTable->setColumnCount(5);
    providerTable->setHorizontalHeaderLabels({"ID", "Name", "Phone", "Email", "Service Type"});
    providerTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    providerTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    providerTable->setSelectionMode(QAbstractItemView::SingleSelection);
}

void ProviderDialog::handleAddProvider()
{
    QString name = nameEdit->text().trimmed();
    QString phone = phoneEdit->text().trimmed();
    QString email = emailEdit->text().trimmed();
    QString serviceType = serviceTypeCombo->currentText();

    if (name.isEmpty() || phone.isEmpty() || email.isEmpty()) {
        QMessageBox::warning(this, "Error", "All fields are required!");
        return;
    }

    try {
        pqxx::work txn(*dbConnection);
        txn.exec_params(
            "INSERT INTO providers (provider_name, phone_number, provider_email, service_type) "
            "VALUES ($1, $2, $3, $4)",
            name.toStdString(),
            phone.toStdString(),
            email.toStdString(),
            serviceType.toStdString()
        );
        txn.commit();

        QMessageBox::information(this, "Success", "Provider added successfully!");
        clearForm();
        handleViewProviders();
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("Failed to add provider: %1").arg(e.what()));
    }
}

void ProviderDialog::handleViewProviders()
{
    try {
        pqxx::work txn(*dbConnection);
        pqxx::result res = txn.exec(
            "SELECT provider_id, provider_name, phone_number, provider_email, service_type "
            "FROM providers ORDER BY provider_id"
        );

        providerTable->setRowCount(res.size());
        
        for (size_t i = 0; i < res.size(); ++i) {
            providerTable->setItem(i, 0, new QTableWidgetItem(QString::number(res[i][0].as<int>())));
            providerTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(res[i][1].as<std::string>())));
            providerTable->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(res[i][2].as<std::string>())));
            providerTable->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(res[i][3].as<std::string>())));
            providerTable->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(res[i][4].as<std::string>())));
        }
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("Failed to load providers: %1").arg(e.what()));
    }
}

void ProviderDialog::handleRemoveProvider()
{
    QModelIndexList selection = providerTable->selectionModel()->selectedRows();
    if (selection.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please select a provider to remove!");
        return;
    }

    int row = selection.first().row();
    int providerId = providerTable->item(row, 0)->text().toInt();

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Confirm Deletion",
        "Are you sure you want to remove this provider?",
        QMessageBox::Yes | QMessageBox::No
    );

    if (reply == QMessageBox::Yes) {
        try {
            pqxx::work txn(*dbConnection);
            txn.exec_params("DELETE FROM providers WHERE provider_id = $1", providerId);
            txn.commit();

            QMessageBox::information(this, "Success", "Provider removed successfully!");
            handleViewProviders();
            clearForm();
            isEditing = false;
            addButton->setVisible(true);
            updateButton->setVisible(false);
        } catch (const std::exception& e) {
            QMessageBox::critical(this, "Error", QString("Failed to remove provider: %1").arg(e.what()));
        }
    }
}

void ProviderDialog::handleEditProvider()
{
    if (!isEditing) return;

    QString name = nameEdit->text().trimmed();
    QString phone = phoneEdit->text().trimmed();
    QString email = emailEdit->text().trimmed();
    QString serviceType = serviceTypeCombo->currentText();

    if (name.isEmpty() || phone.isEmpty() || email.isEmpty()) {
        QMessageBox::warning(this, "Error", "All fields are required!");
        return;
    }

    try {
        pqxx::work txn(*dbConnection);
        txn.exec_params(
            "UPDATE providers SET provider_name = $1, phone_number = $2, provider_email = $3, service_type = $4 "
            "WHERE provider_id = $5",
            name.toStdString(),
            phone.toStdString(),
            email.toStdString(),
            serviceType.toStdString(),
            editingId
        );
        txn.commit();

        QMessageBox::information(this, "Success", "Provider updated successfully!");
        clearForm();
        handleViewProviders();
        isEditing = false;
        addButton->setVisible(true);
        updateButton->setVisible(false);
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("Failed to update provider: %1").arg(e.what()));
    }
}

void ProviderDialog::handleSelectionChanged()
{
    QModelIndexList selection = providerTable->selectionModel()->selectedRows();
    if (!selection.isEmpty()) {
        int row = selection.first().row();
        populateFormForEdit(row);
    }
}

void ProviderDialog::populateFormForEdit(int row)
{
    editingId = providerTable->item(row, 0)->text().toInt();
    nameEdit->setText(providerTable->item(row, 1)->text());
    phoneEdit->setText(providerTable->item(row, 2)->text());
    emailEdit->setText(providerTable->item(row, 3)->text());
    serviceTypeCombo->setCurrentText(providerTable->item(row, 4)->text());
    
    isEditing = true;
    addButton->setVisible(false);
    updateButton->setVisible(true);
}

void ProviderDialog::clearForm()
{
    nameEdit->clear();
    phoneEdit->clear();
    emailEdit->clear();
    serviceTypeCombo->setCurrentIndex(0);
    isEditing = false;
    editingId = -1;
    addButton->setVisible(true);
    updateButton->setVisible(false);
} 