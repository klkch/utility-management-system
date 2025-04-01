#include "MainWindow.h"
#include "CustomerDialog.h"
#include "ProviderDialog.h"
#include "InvoiceDialog.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPalette>
#include <QColor>
#include <QMessageBox>
#include <QLabel>
#include <QSpacerItem>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), dbConnection(nullptr)
{
    setWindowTitle("Utility Management System");
    resize(1000, 700);

    // Set background color
    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor("#122B29"));
    setAutoFillBackground(true);
    setPalette(pal);

    initDatabase();

    // Create central widget and main layout
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    
    // Welcome label
    QLabel *welcomeLabel = new QLabel("Welcome to Utility Management System");
    welcomeLabel->setStyleSheet("font-size: 32px; font-weight: bold; color: white; padding: 20px;");
    welcomeLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(welcomeLabel);

    // Create buttons container
    QWidget *buttonContainer = new QWidget;
    QHBoxLayout *buttonLayout = new QHBoxLayout(buttonContainer);
    
    // Create and style buttons
    customerButton = new QPushButton("Manage Customers", this);
    providerButton = new QPushButton("Manage Providers", this);
    invoiceButton = new QPushButton("Manage Invoices", this);

    QString buttonStyle = "QPushButton {"
                         "    background-color: #2E5D5B;"
                         "    color: white;"
                         "    border: none;"
                         "    padding: 15px 30px;"
                         "    font-size: 18px;"
                         "    border-radius: 8px;"
                         "    min-width: 200px;"
                         "}"
                         "QPushButton:hover {"
                         "    background-color: #3E6D6B;"
                         "}"
                         "QPushButton:pressed {"
                         "    background-color: #1E3D3B;"
                         "}";

    customerButton->setStyleSheet(buttonStyle);
    providerButton->setStyleSheet(buttonStyle);
    invoiceButton->setStyleSheet(buttonStyle);

    // Add buttons to layout with spacing
    buttonLayout->addStretch();
    buttonLayout->addWidget(customerButton);
    buttonLayout->addSpacing(20);
    buttonLayout->addWidget(providerButton);
    buttonLayout->addSpacing(20);
    buttonLayout->addWidget(invoiceButton);
    buttonLayout->addStretch();

    // Add button container to main layout with some vertical spacing
    mainLayout->addSpacing(50);
    mainLayout->addWidget(buttonContainer);
    mainLayout->addStretch();

    setCentralWidget(centralWidget);

    // Connect buttons to slots
    connect(customerButton, &QPushButton::clicked, this, &MainWindow::handleCustomerClick);
    connect(providerButton, &QPushButton::clicked, this, &MainWindow::handleProviderClick);
    connect(invoiceButton, &QPushButton::clicked, this, &MainWindow::handleInvoiceClick);
}

MainWindow::~MainWindow()
{
    if (dbConnection) {
        delete dbConnection;
    }
}

void MainWindow::initDatabase()
{
    try {
        dbConnection = new pqxx::connection(
            "host=aws-0-ca-central-1.pooler.supabase.com "
            "port=5432 "
            "dbname=postgres "
            "user=postgres.nbjzlzreuwryagkoqvgi "
            "password=COMP3400Project "
            "sslmode=require"
        );
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Database Error", 
            QString("Failed to connect to database: %1").arg(e.what()));
    }
}

void MainWindow::handleCustomerClick()
{
    if (!dbConnection) {
        QMessageBox::critical(this, "Error", "Database connection not available!");
        return;
    }

    CustomerDialog dialog(this, dbConnection);
    dialog.exec();
}

void MainWindow::handleProviderClick()
{
    if (!dbConnection) {
        QMessageBox::critical(this, "Error", "Database connection not available!");
        return;
    }

    ProviderDialog dialog(this, dbConnection);
    dialog.exec();
}

void MainWindow::handleInvoiceClick()
{
    if (!dbConnection) {
        QMessageBox::critical(this, "Error", "Database connection not available!");
        return;
    }

    InvoiceDialog dialog(this, dbConnection);
    dialog.exec();
}
