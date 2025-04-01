#include "ServiceDialog.h"
#include <QVBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QLabel>

ServiceDialog::ServiceDialog(QWidget *parent, pqxx::connection* conn)
    : QDialog(parent), dbConnection(conn)
{
    setWindowTitle("Service Catalog");
    setMinimumSize(900, 500);

    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *titleLabel = new QLabel("Services by Provider");
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);

    serviceTable = new QTableWidget(this);
    setupTable();
    layout->addWidget(serviceTable);

    refreshButton = new QPushButton("Refresh", this);
    layout->addWidget(refreshButton, 0, Qt::AlignRight);

    connect(refreshButton, &QPushButton::clicked, this, &ServiceDialog::loadServices);

    loadServices();
}

void ServiceDialog::setupTable()
{
    serviceTable->setColumnCount(5);
    serviceTable->setHorizontalHeaderLabels({
        "Provider", "Service Type", "Meter Rate", "Phone Number", "Email"
    });

    // Set initial column widths manually
    serviceTable->setColumnWidth(0, 150); // Provider
    serviceTable->setColumnWidth(1, 150); // Service Type
    serviceTable->setColumnWidth(2, 100); // Meter Rate
    serviceTable->setColumnWidth(3, 180); // Phone Number
    serviceTable->setColumnWidth(4, 250); // Email (wider)

    serviceTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    serviceTable->setSelectionMode(QAbstractItemView::SingleSelection);
    serviceTable->horizontalHeader()->setStretchLastSection(true);
}

void ServiceDialog::loadServices()
{
    try {
        pqxx::work txn(*dbConnection);
        pqxx::result res = txn.exec(R"(
            SELECT p.provider_name, s.service_type, s.meter_rate, p.phone_number, p.provider_email
            FROM services s
            JOIN providers p ON s.provider_id = p.provider_id
            ORDER BY p.provider_name
        )");

        serviceTable->setRowCount(res.size());

        for (size_t i = 0; i < res.size(); ++i) {
            serviceTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(res[i][0].c_str())));
            serviceTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(res[i][1].c_str())));
            serviceTable->setItem(i, 2, new QTableWidgetItem(QString::number(res[i][2].as<double>(), 'f', 2)));
            serviceTable->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(res[i][3].is_null() ? "" : res[i][3].as<std::string>())));
            serviceTable->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(res[i][4].is_null() ? "" : res[i][4].as<std::string>())));
        }

    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("Failed to load services: %1").arg(e.what()));
    }
}