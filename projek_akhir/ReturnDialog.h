#ifndef RETURNDIALOG_H
#define RETURNDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QDateEdit>
#include <QLabel>
#include <QPushButton>
#include "RentalSystem.h"

class ReturnDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReturnDialog(QWidget *parent, RentalSystem *system, const QString& preselectedTransID = "");

private slots:
    void onTransactionSelected(int index);
    void onReturnDateChanged();
    void onAccept();

private:
    void setupUI();
    void populateTransactionCombo();
    void updatePenaltyPreview();

    RentalSystem *rentalSystem;
    QString preselectedID;

    // Transaction selection
    QComboBox *transactionCombo;
    QLabel *transactionInfoLabel;

    // Return date
    QDateEdit *returnDateEdit;

    // Penalty info
    QLabel *expectedDateLabel;
    QLabel *lateDaysLabel;
    QLabel *penaltyLabel;
    QLabel *totalAmountLabel;

    QPushButton *okButton;
    QPushButton *cancelButton;
};

#endif // RETURNDIALOG_H