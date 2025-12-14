#include "ReturnDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QDate>

ReturnDialog::ReturnDialog(QWidget *parent, RentalSystem *system, const QString& preselectedTransID)
    : QDialog(parent), rentalSystem(system), preselectedID(preselectedTransID)
{
    setupUI();
    populateTransactionCombo();
    updatePenaltyPreview();
}

void ReturnDialog::setupUI()
{
    setWindowTitle("Return Car");
    setModal(true);
    setMinimumWidth(500);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Transaction selection group
    QGroupBox *transGroup = new QGroupBox("Select Transaction");
    QVBoxLayout *transLayout = new QVBoxLayout(transGroup);

    transactionCombo = new QComboBox();
    transLayout->addWidget(transactionCombo);

    transactionInfoLabel = new QLabel("Select a transaction to see details");
    transactionInfoLabel->setStyleSheet("color: gray; font-style: italic;");
    transactionInfoLabel->setWordWrap(true);
    transLayout->addWidget(transactionInfoLabel);

    mainLayout->addWidget(transGroup);

    // Return date group
    QGroupBox *dateGroup = new QGroupBox("Return Information");
    QFormLayout *dateLayout = new QFormLayout(dateGroup);

    returnDateEdit = new QDateEdit();
    returnDateEdit->setDate(QDate::currentDate());
    returnDateEdit->setCalendarPopup(true);
    dateLayout->addRow("Return Date:", returnDateEdit);

    mainLayout->addWidget(dateGroup);

    // Penalty calculation group
    QGroupBox *penaltyGroup = new QGroupBox("Payment Summary");
    QFormLayout *penaltyLayout = new QFormLayout(penaltyGroup);

    expectedDateLabel = new QLabel("-");
    penaltyLayout->addRow("Expected Return:", expectedDateLabel);

    lateDaysLabel = new QLabel("0 days");
    penaltyLayout->addRow("Late Days:", lateDaysLabel);

    penaltyLabel = new QLabel("Rp 0");
    penaltyLabel->setStyleSheet("color: #c62828;");
    penaltyLayout->addRow("Penalty (150%):", penaltyLabel);

    totalAmountLabel = new QLabel("Rp 0");
    totalAmountLabel->setStyleSheet("font-weight: bold; font-size: 14px; color: #2e7d32;");
    penaltyLayout->addRow("Total Amount Due:", totalAmountLabel);

    mainLayout->addWidget(penaltyGroup);

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    okButton = new QPushButton("Confirm Return");
    cancelButton = new QPushButton("Cancel");

    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    mainLayout->addLayout(buttonLayout);

    // Connections
    connect(transactionCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ReturnDialog::onTransactionSelected);
    connect(returnDateEdit, &QDateEdit::dateChanged, this, &ReturnDialog::onReturnDateChanged);
    connect(okButton, &QPushButton::clicked, this, &ReturnDialog::onAccept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

void ReturnDialog::populateTransactionCombo()
{
    transactionCombo->clear();

    auto activeRentals = rentalSystem->getActiveRentals();
    int preselectedIndex = -1;

    for (size_t i = 0; i < activeRentals.size(); ++i) {
        const auto& trans = activeRentals[i];
        Car* car = rentalSystem->getCar(trans.getCarID());

        QString displayText = QString("%1 - %2 (%3)")
            .arg(QString::fromStdString(trans.getTransactionID()))
            .arg(QString::fromStdString(trans.getCustomer().getName()))
            .arg(car ? QString::fromStdString(car->getBrand() + " " + car->getModel()) : "Unknown Car");

        transactionCombo->addItem(displayText, QString::fromStdString(trans.getTransactionID()));

        if (!preselectedID.isEmpty() && QString::fromStdString(trans.getTransactionID()) == preselectedID) {
            preselectedIndex = static_cast<int>(i);
        }
    }

    if (preselectedIndex >= 0) {
        transactionCombo->setCurrentIndex(preselectedIndex);
    } else if (transactionCombo->count() > 0) {
        onTransactionSelected(0);
    }
}

void ReturnDialog::onTransactionSelected(int index)
{
    if (index < 0) return;

    QString transID = transactionCombo->currentData().toString();
    RentalTransaction* trans = rentalSystem->getTransaction(transID.toStdString());

    if (trans) {
        Car* car = rentalSystem->getCar(trans->getCarID());

        QString carInfo = car ?
            QString("%1 %2 (%3)").arg(QString::fromStdString(car->getBrand()))
                .arg(QString::fromStdString(car->getModel()))
                .arg(QString::fromStdString(car->getLicensePlate()))
            : "Unknown Car";

        QString info = QString(
            "Customer: %1\n"
            "NIK: %2\n"
            "Phone: %3\n"
            "Car: %4\n"
            "Period: %5 to %6\n"
            "Rental Cost: Rp %7")
            .arg(QString::fromStdString(trans->getCustomer().getName()))
            .arg(QString::fromStdString(trans->getCustomer().getNIK()))
            .arg(QString::fromStdString(trans->getCustomer().getPhone()))
            .arg(carInfo)
            .arg(QString::fromStdString(trans->getStartDate().toString()))
            .arg(QString::fromStdString(trans->getEndDate().toString()))
            .arg(trans->getTotalCost(), 0, 'f', 0);

        transactionInfoLabel->setText(info);
        transactionInfoLabel->setStyleSheet("color: black; font-style: normal;");

        // Set minimum return date to start date
        Date startDate = trans->getStartDate();
        returnDateEdit->setMinimumDate(QDate(startDate.getYear(), startDate.getMonth(), startDate.getDay()));
    }

    updatePenaltyPreview();
}

void ReturnDialog::onReturnDateChanged()
{
    updatePenaltyPreview();
}

void ReturnDialog::updatePenaltyPreview()
{
    QString transID = transactionCombo->currentData().toString();
    RentalTransaction* trans = rentalSystem->getTransaction(transID.toStdString());

    if (!trans) {
        expectedDateLabel->setText("-");
        lateDaysLabel->setText("0 days");
        penaltyLabel->setText("Rp 0");
        totalAmountLabel->setText("Rp 0");
        return;
    }

    Date endDate = trans->getEndDate();
    expectedDateLabel->setText(QString::fromStdString(endDate.toString()));

    QDate qEndDate(endDate.getYear(), endDate.getMonth(), endDate.getDay());
    QDate qReturnDate = returnDateEdit->date();

    int lateDays = qEndDate.daysTo(qReturnDate);
    if (lateDays < 0) lateDays = 0;

    lateDaysLabel->setText(QString("%1 day(s)").arg(lateDays));

    Car* car = rentalSystem->getCar(trans->getCarID());
    double penalty = 0;
    double pricePerDay = car ? car->getPricePerDay() : 0;

    if (lateDays > 0) {
        penalty = lateDays * (pricePerDay * 1.5);
        lateDaysLabel->setStyleSheet("color: #c62828; font-weight: bold;");
    } else {
        lateDaysLabel->setStyleSheet("color: #2e7d32;");
    }

    penaltyLabel->setText(QString("Rp %1").arg(penalty, 0, 'f', 0));

    double totalAmount = trans->getTotalCost() + penalty;
    totalAmountLabel->setText(QString("Rp %1").arg(totalAmount, 0, 'f', 0));
}

void ReturnDialog::onAccept()
{
    if (transactionCombo->count() == 0) {
        QMessageBox::warning(this, "Error", "No transaction selected!");
        return;
    }

    QString transID = transactionCombo->currentData().toString();
    RentalTransaction* trans = rentalSystem->getTransaction(transID.toStdString());

    if (!trans) {
        QMessageBox::critical(this, "Error", "Transaction not found!");
        return;
    }

    // Show confirmation with penalty if applicable
    Date endDate = trans->getEndDate();
    QDate qEndDate(endDate.getYear(), endDate.getMonth(), endDate.getDay());
    QDate qReturnDate = returnDateEdit->date();

    int lateDays = qEndDate.daysTo(qReturnDate);
    if (lateDays < 0) lateDays = 0;

    QString confirmMsg;
    if (lateDays > 0) {
        Car* car = rentalSystem->getCar(trans->getCarID());
        double penalty = lateDays * (car ? car->getPricePerDay() * 1.5 : 0);
        double total = trans->getTotalCost() + penalty;

        confirmMsg = QString(
            "The return is %1 day(s) late.\n\n"
            "Rental Cost: Rp %2\n"
            "Penalty: Rp %3\n"
            "Total: Rp %4\n\n"
            "Confirm return?")
            .arg(lateDays)
            .arg(trans->getTotalCost(), 0, 'f', 0)
            .arg(penalty, 0, 'f', 0)
            .arg(total, 0, 'f', 0);
    } else {
        confirmMsg = QString(
            "Return on time!\n\n"
            "Total: Rp %1\n\n"
            "Confirm return?")
            .arg(trans->getTotalCost(), 0, 'f', 0);
    }

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirm Return",
        confirmMsg, QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        QDate returnDate = returnDateEdit->date();
        Date date(returnDate.day(), returnDate.month(), returnDate.year());

        if (rentalSystem->returnCar(transID.toStdString(), date)) {
            accept();
        } else {
            QMessageBox::critical(this, "Error", "Failed to process return!");
        }
    }
}