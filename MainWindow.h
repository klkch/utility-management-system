// MainWindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <pqxx/pqxx>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void handleCustomerClick();
    void handleProviderClick();
    void handleInvoiceClick();

private:
    void createButtons();
    void initDatabase();
    
    QPushButton *customerButton;
    QPushButton *providerButton;
    QPushButton *invoiceButton;

    // Database connection
    pqxx::connection* dbConnection;
};

#endif // MAINWINDOW_H
