#ifndef PROVIDERDIALOG_H
#define PROVIDERDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QTableWidget>
#include <QComboBox>
#include <pqxx/pqxx>

class ProviderDialog : public QDialog {
    Q_OBJECT

public:
    ProviderDialog(QWidget *parent = nullptr, pqxx::connection* conn = nullptr);

private slots:
    void handleAddProvider();
    void handleViewProviders();
    void handleRemoveProvider();
    void handleEditProvider();
    void handleSelectionChanged();

private:
    void createForm();
    void clearForm();
    void setupTable();
    void refreshTable();
    void populateFormForEdit(int row);

    QLineEdit *nameEdit;
    QLineEdit *phoneEdit;
    QLineEdit *emailEdit;
    QComboBox *serviceTypeCombo;
    QPushButton *addButton;
    QPushButton *updateButton;
    QPushButton *viewButton;
    QPushButton *removeButton;
    QTableWidget *providerTable;
    pqxx::connection* dbConnection;
    bool isEditing;
    int editingId;
};

#endif // PROVIDERDIALOG_H 