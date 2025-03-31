#include "MainWindow.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QPalette>
#include <QColor>
#include <QMessageBox>
#include <QLabel>

void MainWindow::handleCustomerClick() {
    QMessageBox::information(this, "Customers", "Navigating to Manage Customers...");
}

void MainWindow::handleProviderClick() {
    QMessageBox::information(this, "Providers", "Navigating to Manage Providers...");
}

void MainWindow::handleInvoiceClick() {
    QMessageBox::information(this, "Invoices", "Navigating to Manage Invoices...");
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Utility Management System");
    resize(1000, 700);

    // Set background color
    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor("#122B29"));
    setAutoFillBackground(true);
    setPalette(pal);

    // Main layout
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(50, 50, 50, 50);

    // Title
    QLabel *title = new QLabel("Utility Management System");
    title->setStyleSheet("font-size: 32px; font-weight: bold; color: white;");
    title->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(title);
    mainLayout->addSpacing(50);

    // Button group
    QVBoxLayout *buttonLayout = new QVBoxLayout;
    buttonLayout->setSpacing(30);
    buttonLayout->setAlignment(Qt::AlignCenter);

    QString buttonStyle = R"(
        QPushButton {
            font-size: 22px;
            padding: 25px;
            min-height: 80px;
            min-width: 400px;
            border-radius: 12px;
            background-color: #1A3C3A;
            color: white;
        }
        QPushButton:hover {
            background-color: #256963;
        }
    )";

    auto createButton = [&](const QString &text) -> QPushButton* {
        QPushButton *btn = new QPushButton(text);
        btn->setStyleSheet(buttonStyle);
        btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        return btn;
    };

    QPushButton *customerButton = createButton("Manage Customers");
    QPushButton *providerButton = createButton("Manage Providers");
    QPushButton *invoiceButton = createButton("Manage Invoices");

    connect(customerButton, &QPushButton::clicked, this, &MainWindow::handleCustomerClick);
    connect(providerButton, &QPushButton::clicked, this, &MainWindow::handleProviderClick);
    connect(invoiceButton, &QPushButton::clicked, this, &MainWindow::handleInvoiceClick);

    buttonLayout->addWidget(customerButton);
    buttonLayout->addWidget(providerButton);
    buttonLayout->addWidget(invoiceButton);

    QVBoxLayout *centerLayout = new QVBoxLayout;
    centerLayout->addStretch();
    centerLayout->addLayout(buttonLayout);
    centerLayout->addStretch();

    mainLayout->addLayout(centerLayout);
    setCentralWidget(centralWidget);
}
