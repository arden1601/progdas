#include "CarDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QMessageBox>

CarDialog::CarDialog(QWidget *parent, Car *existingCar)
    : QDialog(parent), isEditMode(existingCar != nullptr)
{
    setupUI();

    if (existingCar) {
        carIDEdit->setText(QString::fromStdString(existingCar->getCarID()));
        carIDEdit->setEnabled(false); // Can't change ID in edit mode
        brandEdit->setText(QString::fromStdString(existingCar->getBrand()));
        modelEdit->setText(QString::fromStdString(existingCar->getModel()));
        yearSpin->setValue(existingCar->getYear());
        licensePlateEdit->setText(QString::fromStdString(existingCar->getLicensePlate()));
        priceSpin->setValue(existingCar->getPricePerDay());
        statusCombo->setCurrentIndex(existingCar->getStatus());
    }
}

void CarDialog::setupUI()
{
    setWindowTitle("Car Details");
    setModal(true);
    setMinimumWidth(400);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Form group
    QGroupBox *formGroup = new QGroupBox("Car Information");
    QFormLayout *formLayout = new QFormLayout(formGroup);

    carIDEdit = new QLineEdit();
    carIDEdit->setPlaceholderText("e.g., C001");
    formLayout->addRow("Car ID:", carIDEdit);

    brandEdit = new QLineEdit();
    brandEdit->setPlaceholderText("e.g., Toyota");
    formLayout->addRow("Brand:", brandEdit);

    modelEdit = new QLineEdit();
    modelEdit->setPlaceholderText("e.g., Avanza");
    formLayout->addRow("Model:", modelEdit);

    yearSpin = new QSpinBox();
    yearSpin->setRange(1990, 2025);
    yearSpin->setValue(2023);
    formLayout->addRow("Year:", yearSpin);

    licensePlateEdit = new QLineEdit();
    licensePlateEdit->setPlaceholderText("e.g., B1234ABC");
    formLayout->addRow("License Plate:", licensePlateEdit);

    priceSpin = new QDoubleSpinBox();
    priceSpin->setRange(0, 10000000);
    priceSpin->setValue(350000);
    priceSpin->setPrefix("Rp ");
    priceSpin->setDecimals(0);
    priceSpin->setSingleStep(50000);
    formLayout->addRow("Price per Day:", priceSpin);

    statusCombo = new QComboBox();
    statusCombo->addItem("Ready", READY);
    statusCombo->addItem("Maintenance", MAINTENANCE);
    statusCombo->addItem("Rented", RENTED);
    formLayout->addRow("Status:", statusCombo);

    if (!isEditMode) {
        statusCombo->setEnabled(false); // New cars start as Ready
    }

    mainLayout->addWidget(formGroup);

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    okButton = new QPushButton("OK");
    cancelButton = new QPushButton("Cancel");

    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    mainLayout->addLayout(buttonLayout);

    connect(okButton, &QPushButton::clicked, this, &CarDialog::onAccept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

bool CarDialog::validateInput()
{
    if (carIDEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Car ID is required!");
        carIDEdit->setFocus();
        return false;
    }

    if (brandEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Brand is required!");
        brandEdit->setFocus();
        return false;
    }

    if (modelEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Model is required!");
        modelEdit->setFocus();
        return false;
    }

    if (licensePlateEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "License plate is required!");
        licensePlateEdit->setFocus();
        return false;
    }

    if (priceSpin->value() <= 0) {
        QMessageBox::warning(this, "Validation Error", "Price must be greater than 0!");
        priceSpin->setFocus();
        return false;
    }

    return true;
}

void CarDialog::onAccept()
{
    if (validateInput()) {
        accept();
    }
}

Car CarDialog::getCar() const
{
    Car car(
        carIDEdit->text().trimmed().toStdString(),
        brandEdit->text().trimmed().toStdString(),
        modelEdit->text().trimmed().toStdString(),
        yearSpin->value(),
        licensePlateEdit->text().trimmed().toStdString(),
        priceSpin->value()
    );

    car.setStatus(static_cast<CarStatus>(statusCombo->currentData().toInt()));

    return car;
}