#ifndef CUSTOMERDIALOG_H
#define CUSTOMERDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QTableWidget>
#include <pqxx/pqxx>

class CustomerDialog : public QDialog {
    Q_OBJECT

public:
    CustomerDialog(QWidget *parent = nullptr, pqxx::connection* conn = nullptr);

private slots:
    void handleAddCustomer();
    void handleViewCustomers();
    void handleRemoveCustomer();

private:
    void createForm();
    void clearForm();
    void setupTable();
    void refreshTable();

    QLineEdit *nameEdit;
    QLineEdit *addressEdit;
    QLineEdit *phoneEdit;
    QLineEdit *emailEdit;
    QPushButton *addButton;
    QPushButton *viewButton;
    QPushButton *removeButton;
    QTableWidget *customerTable;
    pqxx::connection* dbConnection;
};

#endif // CUSTOMERDIALOG_H 