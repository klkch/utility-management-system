#include "MainWindow.h"
#include "CustomerDialog.h"
#include "ProviderDialog.h"
#include "InvoiceDialog.h"
#include "ServiceDialog.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPalette>
#include <QColor>
#include <QMessageBox>
#include <QLabel>
#include <QSpacerItem>
#include <QPushButton>
#include <QPixmap>
#include <QFile>

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
    QLabel *welcomeLabel = new QLabel("Utility Management System");
    welcomeLabel->setStyleSheet("font-size: 32px; font-weight: bold; color: white; padding: 20px;");
    welcomeLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(welcomeLabel);

    QLabel *names = new QLabel("By Ali Kalakech, Chris Bolton and Daniel Yun");
    names->setStyleSheet("font-size: 16px; color: white; padding: 10px;");
    names->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(names);



    // Create button containers
    QWidget *topButtonContainer = new QWidget;
    QHBoxLayout *topButtonLayout = new QHBoxLayout(topButtonContainer);

    QWidget *bottomButtonContainer = new QWidget;
    QHBoxLayout *bottomButtonLayout = new QHBoxLayout(bottomButtonContainer);

    // Create and style buttons
    customerButton = new QPushButton("Manage Customers", this);
    providerButton = new QPushButton("Manage Providers", this);
    invoiceButton = new QPushButton("Manage Invoices", this);
    serviceButton = new QPushButton("View Services", this);

    QString buttonStyle = "QPushButton {"
                         "    background-color:rgb(62, 126, 123);"
                         "    color: white;"
                         "    border: none;"
                         "    padding: 15px 30px;"
                         "    font-size: 18px;"
                         "    border-radius: 8px;"
                         "    min-width: 200px;"
                         "}"
                         "QPushButton:hover {"
                         "    background-color:rgb(76, 135, 132);"
                         "}"
                         "QPushButton:pressed {"
                         "    background-color:rgb(46, 94, 90);"
                         "}";

    customerButton->setStyleSheet(buttonStyle);
    providerButton->setStyleSheet(buttonStyle);
    invoiceButton->setStyleSheet(buttonStyle);
    serviceButton->setStyleSheet(buttonStyle);

    // Icon and button layout setup
    auto makeIconButtonGroup = [](const QString &iconPath, QPushButton *button) -> QWidget* {
        QLabel *iconLabel = new QLabel;
        iconLabel->setPixmap(QPixmap(iconPath).scaled(128, 128, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        iconLabel->setAlignment(Qt::AlignCenter);

        QVBoxLayout *vLayout = new QVBoxLayout;
        vLayout->addWidget(iconLabel);
        vLayout->addWidget(button);
        vLayout->setAlignment(Qt::AlignHCenter);
        vLayout->setContentsMargins(0, 10, 0, 0);

        QWidget *group = new QWidget;
        group->setLayout(vLayout);
        return group;
    };

    // Create icon-button groups
    QWidget *customerWidget = makeIconButtonGroup("icons/customer.png", customerButton);
    QWidget *providerWidget = makeIconButtonGroup("icons/provider.png", providerButton);
    QWidget *invoiceWidget = makeIconButtonGroup("icons/invoice.png", invoiceButton);
    QWidget *serviceWidget = makeIconButtonGroup("icons/service.png", serviceButton); 

    // Add widgets to layouts
    topButtonLayout->addStretch();
    topButtonLayout->addWidget(customerWidget);
    topButtonLayout->addSpacing(20);
    topButtonLayout->addWidget(providerWidget);
    topButtonLayout->addSpacing(20);
    topButtonLayout->addWidget(invoiceWidget);
    topButtonLayout->addStretch();

    bottomButtonLayout->addStretch();
    bottomButtonLayout->addWidget(serviceWidget);
    bottomButtonLayout->addStretch();

    // Add button containers to main layout
    mainLayout->addSpacing(50);
    mainLayout->addWidget(topButtonContainer);
    mainLayout->addSpacing(30);
    mainLayout->addWidget(bottomButtonContainer);
    mainLayout->addStretch();

    setCentralWidget(centralWidget);

    // Connect buttons to slots
    connect(customerButton, &QPushButton::clicked, this, &MainWindow::handleCustomerClick);
    connect(providerButton, &QPushButton::clicked, this, &MainWindow::handleProviderClick);
    connect(invoiceButton, &QPushButton::clicked, this, &MainWindow::handleInvoiceClick);
    connect(serviceButton, &QPushButton::clicked, this, &MainWindow::handleServiceClick); 
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

void MainWindow::handleServiceClick()
{
    if (!dbConnection) {
        QMessageBox::critical(this, "Error", "Database connection not available!");
        return;
    }

    ServiceDialog dialog(this, dbConnection);
    dialog.exec();
}

