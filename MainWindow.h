// MainWindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void handleCustomerClick();
    void handleProviderClick();
    void handleInvoiceClick();

private:
    QWidget *centralWidget;
    QVBoxLayout *layout;
    QPushButton *customerButton;
    QPushButton *providerButton;
    QPushButton *invoiceButton;
};

#endif // MAINWINDOW_H
