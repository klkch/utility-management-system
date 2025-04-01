#ifndef SERVICEDIALOG_H
#define SERVICEDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QPushButton>
#include <pqxx/pqxx>

class ServiceDialog : public QDialog {
    Q_OBJECT

public:
    explicit ServiceDialog(QWidget *parent = nullptr, pqxx::connection* conn = nullptr);

private slots:
    void loadServices();

private:
    void setupTable();

    QTableWidget *serviceTable;
    QPushButton *refreshButton;
    pqxx::connection* dbConnection;
};

#endif // SERVICEDIALOG_H
