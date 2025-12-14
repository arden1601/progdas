#ifndef RENTALDIALOG_H
#define RENTALDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QDateEdit>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include "RentalSystem.h"

class RentalDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RentalDialog(QWidget *parent, RentalSystem *system);

private slots:
    void onCarSelected(int index);
    void onDateChanged();
    void onAccept();

private:
    void setupUI();
    void populateCarCombo();
    void updateCostPreview();
    bool validateInput();

    RentalSystem *rentalSystem;

    // Car selection
    QComboBox *carCombo;
    QLabel *carInfoLabel;

    // Customer info
    QLineEdit *nameEdit;
    QLineEdit *nikEdit;
    QLineEdit *phoneEdit;

    // Dates
    QDateEdit *startDateEdit;
    QDateEdit *endDateEdit;

    // Cost preview
    QLabel *daysLabel;
    QLabel *pricePerDayLabel;
    QLabel *totalCostLabel;

    QPushButton *okButton;
    QPushButton *cancelButton;
};

#endif // RENTALDIALOG_H