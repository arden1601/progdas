#include "RentalDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QDate>

RentalDialog::RentalDialog(QWidget *parent, RentalSystem *system)
    : QDialog(parent), rentalSystem(system)
{
    setupUI();
    populateCarCombo();
    updateCostPreview();
}

void RentalDialog::setupUI()
{
    setWindowTitle("Create New Rental");
    setModal(true);
    setMinimumWidth(500);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Car selection group
    QGroupBox *carGroup = new QGroupBox("Select Car");
    QVBoxLayout *carLayout = new QVBoxLayout(carGroup);

    carCombo = new QComboBox();
    carLayout->addWidget(carCombo);

    carInfoLabel = new QLabel("Select a car to see details");
    carInfoLabel->setStyleSheet("color: gray; font-style: italic;");
    carLayout->addWidget(carInfoLabel);

    mainLayout->addWidget(carGroup);

    // Customer info group
    QGroupBox *customerGroup = new QGroupBox("Customer Information");
    QFormLayout *customerLayout = new QFormLayout(customerGroup);

    nameEdit = new QLineEdit();
    nameEdit->setPlaceholderText("Full name");
    customerLayout->addRow("Name:", nameEdit);

    nikEdit = new QLineEdit();
    nikEdit->setPlaceholderText("16-digit NIK");
    nikEdit->setMaxLength(16);
    customerLayout->addRow("NIK:", nikEdit);

    phoneEdit = new QLineEdit();
    phoneEdit->setPlaceholderText("e.g., 081234567890");
    customerLayout->addRow("Phone:", phoneEdit);

    mainLayout->addWidget(customerGroup);

    // Rental period group
    QGroupBox *periodGroup = new QGroupBox("Rental Period");
    QFormLayout *periodLayout = new QFormLayout(periodGroup);

    startDateEdit = new QDateEdit();
    startDateEdit->setDate(QDate::currentDate());
    startDateEdit->setCalendarPopup(true);
    startDateEdit->setMinimumDate(QDate::currentDate());
    periodLayout->addRow("Start Date:", startDateEdit);

    endDateEdit = new QDateEdit();
    endDateEdit->setDate(QDate::currentDate().addDays(1));
    endDateEdit->setCalendarPopup(true);
    endDateEdit->setMinimumDate(QDate::currentDate());
    periodLayout->addRow("End Date:", endDateEdit);

    mainLayout->addWidget(periodGroup);

    // Cost preview group
    QGroupBox *costGroup = new QGroupBox("Cost Preview");
    QFormLayout *costLayout = new QFormLayout(costGroup);

    daysLabel = new QLabel("0 days");
    costLayout->addRow("Duration:", daysLabel);

    pricePerDayLabel = new QLabel("Rp 0");
    costLayout->addRow("Price per Day:", pricePerDayLabel);

    totalCostLabel = new QLabel("Rp 0");
    totalCostLabel->setStyleSheet("font-weight: bold; font-size: 14px; color: #2e7d32;");
    costLayout->addRow("Total Cost:", totalCostLabel);

    mainLayout->addWidget(costGroup);

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    okButton = new QPushButton("Create Rental");
    cancelButton = new QPushButton("Cancel");

    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    mainLayout->addLayout(buttonLayout);

    // Connections
    connect(carCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &RentalDialog::onCarSelected);
    connect(startDateEdit, &QDateEdit::dateChanged, this, &RentalDialog::onDateChanged);
    connect(endDateEdit, &QDateEdit::dateChanged, this, &RentalDialog::onDateChanged);
    connect(okButton, &QPushButton::clicked, this, &RentalDialog::onAccept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

void RentalDialog::populateCarCombo()
{
    carCombo->clear();

    auto availableCars = rentalSystem->getAvailableCars();

    for (const auto& car : availableCars) {
        QString displayText = QString("%1 - %2 %3 (%4)")
            .arg(QString::fromStdString(car.getCarID()))
            .arg(QString::fromStdString(car.getBrand()))
            .arg(QString::fromStdString(car.getModel()))
            .arg(QString::fromStdString(car.getLicensePlate()));

        carCombo->addItem(displayText, QString::fromStdString(car.getCarID()));
    }

    if (carCombo->count() > 0) {
        onCarSelected(0);
    }
}

void RentalDialog::onCarSelected(int index)
{
    if (index < 0) return;

    QString carID = carCombo->currentData().toString();
    Car* car = rentalSystem->getCar(carID.toStdString());

    if (car) {
        QString info = QString("Brand: %1\nModel: %2\nYear: %3\nLicense Plate: %4\nPrice/Day: Rp %5")
            .arg(QString::fromStdString(car->getBrand()))
            .arg(QString::fromStdString(car->getModel()))
            .arg(car->getYear())
            .arg(QString::fromStdString(car->getLicensePlate()))
            .arg(car->getPricePerDay(), 0, 'f', 0);

        carInfoLabel->setText(info);
        carInfoLabel->setStyleSheet("color: black; font-style: normal;");
    }

    updateCostPreview();
}

void RentalDialog::onDateChanged()
{
    // Ensure end date is not before start date
    if (endDateEdit->date() < startDateEdit->date()) {
        endDateEdit->setDate(startDateEdit->date().addDays(1));
    }
    endDateEdit->setMinimumDate(startDateEdit->date());

    updateCostPreview();
}

void RentalDialog::updateCostPreview()
{
    int days = startDateEdit->date().daysTo(endDateEdit->date());
    if (days <= 0) days = 1;

    daysLabel->setText(QString("%1 day(s)").arg(days));

    QString carID = carCombo->currentData().toString();
    Car* car = rentalSystem->getCar(carID.toStdString());

    if (car) {
        double pricePerDay = car->getPricePerDay();
        double totalCost = days * pricePerDay;

        pricePerDayLabel->setText(QString("Rp %1").arg(pricePerDay, 0, 'f', 0));
        totalCostLabel->setText(QString("Rp %1").arg(totalCost, 0, 'f', 0));
    } else {
        pricePerDayLabel->setText("Rp 0");
        totalCostLabel->setText("Rp 0");
    }
}

bool RentalDialog::validateInput()
{
    if (carCombo->count() == 0) {
        QMessageBox::warning(this, "Validation Error", "No car selected!");
        return false;
    }

    if (nameEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Customer name is required!");
        nameEdit->setFocus();
        return false;
    }

    if (nikEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "NIK is required!");
        nikEdit->setFocus();
        return false;
    }

    if (nikEdit->text().trimmed().length() != 16) {
        QMessageBox::warning(this, "Validation Error", "NIK must be 16 digits!");
        nikEdit->setFocus();
        return false;
    }

    if (phoneEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Phone number is required!");
        phoneEdit->setFocus();
        return false;
    }

    if (endDateEdit->date() < startDateEdit->date()) {
        QMessageBox::warning(this, "Validation Error", "End date must be after start date!");
        endDateEdit->setFocus();
        return false;
    }

    return true;
}

void RentalDialog::onAccept()
{
    if (!validateInput()) return;

    QString carID = carCombo->currentData().toString();

    Customer customer(
        nameEdit->text().trimmed().toStdString(),
        nikEdit->text().trimmed().toStdString(),
        phoneEdit->text().trimmed().toStdString()
    );

    QDate qStartDate = startDateEdit->date();
    QDate qEndDate = endDateEdit->date();

    Date startDate(qStartDate.day(), qStartDate.month(), qStartDate.year());
    Date endDate(qEndDate.day(), qEndDate.month(), qEndDate.year());

    if (rentalSystem->createRental(carID.toStdString(), customer, startDate, endDate)) {
        accept();
    } else {
        QMessageBox::critical(this, "Error", "Failed to create rental!");
    }
}