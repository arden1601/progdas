#ifndef CARDIALOG_H
#define CARDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QPushButton>
#include "Car.h"

class CarDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CarDialog(QWidget *parent = nullptr, Car *existingCar = nullptr);
    Car getCar() const;

private slots:
    void onAccept();

private:
    void setupUI();
    bool validateInput();

    QLineEdit *carIDEdit;
    QLineEdit *brandEdit;
    QLineEdit *modelEdit;
    QSpinBox *yearSpin;
    QLineEdit *licensePlateEdit;
    QDoubleSpinBox *priceSpin;
    QComboBox *statusCombo;

    QPushButton *okButton;
    QPushButton *cancelButton;

    bool isEditMode;
};

#endif // CARDIALOG_H