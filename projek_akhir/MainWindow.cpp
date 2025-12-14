#include "MainWindow.h"
#include "CarDialog.h"
#include "RentalDialog.h"
#include "ReturnDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QMessageBox>
#include <QInputDialog>
#include <QAction>
#include <QApplication>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    rentalSystem = new RentalSystem();
    rentalSystem->initialize();

    setupUI();
    setupMenuBar();
    setupToolBar();
    setupConnections();

    refreshCarTable();
    refreshTransactionTable();
    updateStatusBar();

    setWindowTitle("Car Rental Management System");
    resize(1000, 600);
}

MainWindow::~MainWindow()
{
    rentalSystem->saveAllData();
    delete rentalSystem;
}

void MainWindow::setupUI()
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    tabWidget = new QTabWidget();
    mainLayout->addWidget(tabWidget);

    setupCarTab();
    setupTransactionTab();

    tabWidget->addTab(carTab, "Cars");
    tabWidget->addTab(transactionTab, "Transactions");

    statusBar()->showMessage("Ready");
}

void MainWindow::setupMenuBar()
{
    QMenu *fileMenu = menuBar()->addMenu("&File");

    QAction *refreshAction = new QAction("&Refresh", this);
    refreshAction->setShortcut(QKeySequence::Refresh);
    connect(refreshAction, &QAction::triggered, this, &MainWindow::onRefresh);
    fileMenu->addAction(refreshAction);

    fileMenu->addSeparator();

    QAction *exitAction = new QAction("E&xit", this);
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, &QAction::triggered, this, &QWidget::close);
    fileMenu->addAction(exitAction);

    QMenu *carMenu = menuBar()->addMenu("&Cars");

    QAction *addCarAction = new QAction("&Add Car", this);
    connect(addCarAction, &QAction::triggered, this, &MainWindow::onAddCar);
    carMenu->addAction(addCarAction);

    QAction *editCarAction = new QAction("&Edit Car", this);
    connect(editCarAction, &QAction::triggered, this, &MainWindow::onEditCar);
    carMenu->addAction(editCarAction);

    QAction *deleteCarAction = new QAction("&Delete Car", this);
    connect(deleteCarAction, &QAction::triggered, this, &MainWindow::onDeleteCar);
    carMenu->addAction(deleteCarAction);

    QMenu *transMenu = menuBar()->addMenu("&Transactions");

    QAction *newRentalAction = new QAction("&New Rental", this);
    connect(newRentalAction, &QAction::triggered, this, &MainWindow::onCreateRental);
    transMenu->addAction(newRentalAction);

    QAction *returnCarAction = new QAction("&Return Car", this);
    connect(returnCarAction, &QAction::triggered, this, &MainWindow::onReturnCar);
    transMenu->addAction(returnCarAction);

    QMenu *helpMenu = menuBar()->addMenu("&Help");

    QAction *aboutAction = new QAction("&About", this);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::onAbout);
    helpMenu->addAction(aboutAction);
}

void MainWindow::setupToolBar()
{
    QToolBar *toolBar = addToolBar("Main Toolbar");

    QAction *addCarAction = toolBar->addAction("Add Car");
    connect(addCarAction, &QAction::triggered, this, &MainWindow::onAddCar);

    QAction *newRentalAction = toolBar->addAction("New Rental");
    connect(newRentalAction, &QAction::triggered, this, &MainWindow::onCreateRental);

    QAction *returnAction = toolBar->addAction("Return Car");
    connect(returnAction, &QAction::triggered, this, &MainWindow::onReturnCar);

    toolBar->addSeparator();

    QAction *refreshAction = toolBar->addAction("Refresh");
    connect(refreshAction, &QAction::triggered, this, &MainWindow::onRefresh);
}

void MainWindow::setupCarTab()
{
    carTab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(carTab);

    // Filter section
    QHBoxLayout *filterLayout = new QHBoxLayout();
    filterLayout->addWidget(new QLabel("Filter by Status:"));
    carFilterCombo = new QComboBox();
    carFilterCombo->addItem("All Cars", -1);
    carFilterCombo->addItem("Ready", READY);
    carFilterCombo->addItem("Maintenance", MAINTENANCE);
    carFilterCombo->addItem("Rented", RENTED);
    filterLayout->addWidget(carFilterCombo);
    filterLayout->addStretch();
    carCountLabel = new QLabel("Total: 0 cars");
    filterLayout->addWidget(carCountLabel);
    layout->addLayout(filterLayout);

    // Car table
    carTable = new QTableWidget();
    carTable->setColumnCount(7);
    carTable->setHorizontalHeaderLabels({"ID", "Brand", "Model", "Year", "License Plate", "Price/Day", "Status"});
    carTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    carTable->setSelectionMode(QAbstractItemView::SingleSelection);
    carTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    carTable->horizontalHeader()->setStretchLastSection(true);
    carTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    layout->addWidget(carTable);

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    btnAddCar = new QPushButton("Add Car");
    btnEditCar = new QPushButton("Edit Car");
    btnDeleteCar = new QPushButton("Delete Car");
    btnUpdateStatus = new QPushButton("Update Status");

    btnEditCar->setEnabled(false);
    btnDeleteCar->setEnabled(false);
    btnUpdateStatus->setEnabled(false);

    buttonLayout->addWidget(btnAddCar);
    buttonLayout->addWidget(btnEditCar);
    buttonLayout->addWidget(btnDeleteCar);
    buttonLayout->addWidget(btnUpdateStatus);
    buttonLayout->addStretch();
    layout->addLayout(buttonLayout);
}

void MainWindow::setupTransactionTab()
{
    transactionTab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(transactionTab);

    // Filter section
    QHBoxLayout *filterLayout = new QHBoxLayout();
    filterLayout->addWidget(new QLabel("Filter by Status:"));
    transactionFilterCombo = new QComboBox();
    transactionFilterCombo->addItem("All Transactions", -1);
    transactionFilterCombo->addItem("Active", ACTIVE);
    transactionFilterCombo->addItem("Completed", COMPLETED);
    transactionFilterCombo->addItem("Cancelled", CANCELLED);
    filterLayout->addWidget(transactionFilterCombo);
    filterLayout->addStretch();
    transactionCountLabel = new QLabel("Total: 0 transactions");
    filterLayout->addWidget(transactionCountLabel);
    layout->addLayout(filterLayout);

    // Transaction table
    transactionTable = new QTableWidget();
    transactionTable->setColumnCount(9);
    transactionTable->setHorizontalHeaderLabels({"Trans ID", "Car ID", "Customer", "Phone", "Start Date", "End Date", "Total Cost", "Penalty", "Status"});
    transactionTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    transactionTable->setSelectionMode(QAbstractItemView::SingleSelection);
    transactionTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    transactionTable->horizontalHeader()->setStretchLastSection(true);
    transactionTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    layout->addWidget(transactionTable);

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    btnCreateRental = new QPushButton("New Rental");
    btnReturnCar = new QPushButton("Return Car");

    btnReturnCar->setEnabled(false);

    buttonLayout->addWidget(btnCreateRental);
    buttonLayout->addWidget(btnReturnCar);
    buttonLayout->addStretch();
    layout->addLayout(buttonLayout);
}

void MainWindow::setupConnections()
{
    // Car tab connections
    connect(btnAddCar, &QPushButton::clicked, this, &MainWindow::onAddCar);
    connect(btnEditCar, &QPushButton::clicked, this, &MainWindow::onEditCar);
    connect(btnDeleteCar, &QPushButton::clicked, this, &MainWindow::onDeleteCar);
    connect(btnUpdateStatus, &QPushButton::clicked, this, &MainWindow::onUpdateCarStatus);
    connect(carFilterCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onCarFilterChanged);
    connect(carTable, &QTableWidget::itemSelectionChanged, this, &MainWindow::onCarTableSelectionChanged);

    // Transaction tab connections
    connect(btnCreateRental, &QPushButton::clicked, this, &MainWindow::onCreateRental);
    connect(btnReturnCar, &QPushButton::clicked, this, &MainWindow::onReturnCar);
    connect(transactionFilterCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int) { refreshTransactionTable(); });
    connect(transactionTable, &QTableWidget::itemSelectionChanged, this, &MainWindow::onTransactionTableSelectionChanged);
}

void MainWindow::refreshCarTable()
{
    carTable->setRowCount(0);

    std::vector<Car> cars;
    int filterIndex = carFilterCombo->currentData().toInt();

    if (filterIndex == -1) {
        cars = rentalSystem->getAllCars();
    } else {
        cars = rentalSystem->getCarsByStatus(static_cast<CarStatus>(filterIndex));
    }

    for (const auto& car : cars) {
        int row = carTable->rowCount();
        carTable->insertRow(row);

        carTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(car.getCarID())));
        carTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(car.getBrand())));
        carTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(car.getModel())));
        carTable->setItem(row, 3, new QTableWidgetItem(QString::number(car.getYear())));
        carTable->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(car.getLicensePlate())));
        carTable->setItem(row, 5, new QTableWidgetItem(QString("Rp %1").arg(car.getPricePerDay(), 0, 'f', 0)));
        carTable->setItem(row, 6, new QTableWidgetItem(getStatusString(car.getStatus())));

        // Color code the status
        QTableWidgetItem *statusItem = carTable->item(row, 6);
        switch (car.getStatus()) {
            case READY:
                statusItem->setBackground(QColor(200, 255, 200));
                break;
            case MAINTENANCE:
                statusItem->setBackground(QColor(255, 255, 200));
                break;
            case RENTED:
                statusItem->setBackground(QColor(255, 200, 200));
                break;
        }
    }

    carCountLabel->setText(QString("Total: %1 cars").arg(cars.size()));
}

void MainWindow::refreshTransactionTable()
{
    transactionTable->setRowCount(0);

    std::vector<RentalTransaction> transactions;
    int filterIndex = transactionFilterCombo->currentData().toInt();

    if (filterIndex == -1) {
        transactions = rentalSystem->getAllTransactions();
    } else if (filterIndex == ACTIVE) {
        transactions = rentalSystem->getActiveRentals();
    } else {
        transactions = rentalSystem->getAllTransactions();
        transactions.erase(
            std::remove_if(transactions.begin(), transactions.end(),
                [filterIndex](const RentalTransaction& t) {
                    return t.getStatus() != static_cast<TransactionStatus>(filterIndex);
                }),
            transactions.end()
        );
    }

    for (const auto& trans : transactions) {
        int row = transactionTable->rowCount();
        transactionTable->insertRow(row);

        transactionTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(trans.getTransactionID())));
        transactionTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(trans.getCarID())));
        transactionTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(trans.getCustomer().getName())));
        transactionTable->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(trans.getCustomer().getPhone())));
        transactionTable->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(trans.getStartDate().toString())));
        transactionTable->setItem(row, 5, new QTableWidgetItem(QString::fromStdString(trans.getEndDate().toString())));
        transactionTable->setItem(row, 6, new QTableWidgetItem(QString("Rp %1").arg(trans.getTotalCost(), 0, 'f', 0)));
        transactionTable->setItem(row, 7, new QTableWidgetItem(QString("Rp %1").arg(trans.getPenalty(), 0, 'f', 0)));
        transactionTable->setItem(row, 8, new QTableWidgetItem(getTransactionStatusString(trans.getStatus())));

        // Color code the status
        QTableWidgetItem *statusItem = transactionTable->item(row, 8);
        switch (trans.getStatus()) {
            case ACTIVE:
                statusItem->setBackground(QColor(255, 255, 200));
                break;
            case COMPLETED:
                statusItem->setBackground(QColor(200, 255, 200));
                break;
            case CANCELLED:
                statusItem->setBackground(QColor(255, 200, 200));
                break;
        }
    }

    transactionCountLabel->setText(QString("Total: %1 transactions").arg(transactions.size()));
}

void MainWindow::updateStatusBar()
{
    auto allCars = rentalSystem->getAllCars();
    auto availableCars = rentalSystem->getAvailableCars();
    auto activeRentals = rentalSystem->getActiveRentals();

    statusBar()->showMessage(QString("Total Cars: %1 | Available: %2 | Active Rentals: %3")
        .arg(allCars.size())
        .arg(availableCars.size())
        .arg(activeRentals.size()));
}

QString MainWindow::getStatusString(CarStatus status)
{
    switch (status) {
        case READY: return "Ready";
        case MAINTENANCE: return "Maintenance";
        case RENTED: return "Rented";
        default: return "Unknown";
    }
}

QString MainWindow::getTransactionStatusString(TransactionStatus status)
{
    switch (status) {
        case ACTIVE: return "Active";
        case COMPLETED: return "Completed";
        case CANCELLED: return "Cancelled";
        default: return "Unknown";
    }
}

void MainWindow::showMessage(const QString& title, const QString& message)
{
    QMessageBox::information(this, title, message);
}

void MainWindow::showError(const QString& title, const QString& message)
{
    QMessageBox::critical(this, title, message);
}

void MainWindow::onAddCar()
{
    CarDialog dialog(this);
    dialog.setWindowTitle("Add New Car");

    if (dialog.exec() == QDialog::Accepted) {
        Car car = dialog.getCar();

        if (!rentalSystem->isCarIDUnique(car.getCarID())) {
            showError("Error", "Car ID already exists!");
            return;
        }

        if (!rentalSystem->isLicensePlateUnique(car.getLicensePlate())) {
            showError("Error", "License plate already exists!");
            return;
        }

        if (rentalSystem->addCar(car)) {
            showMessage("Success", "Car added successfully!");
            refreshCarTable();
            updateStatusBar();
        } else {
            showError("Error", "Failed to add car!");
        }
    }
}

void MainWindow::onEditCar()
{
    int row = carTable->currentRow();
    if (row < 0) return;

    QString carID = carTable->item(row, 0)->text();
    Car* car = rentalSystem->getCar(carID.toStdString());

    if (!car) {
        showError("Error", "Car not found!");
        return;
    }

    CarDialog dialog(this, car);
    dialog.setWindowTitle("Edit Car");

    if (dialog.exec() == QDialog::Accepted) {
        Car updatedCar = dialog.getCar();

        // Update car properties
        car->setBrand(updatedCar.getBrand());
        car->setModel(updatedCar.getModel());
        car->setYear(updatedCar.getYear());
        car->setLicensePlate(updatedCar.getLicensePlate());
        car->setPricePerDay(updatedCar.getPricePerDay());

        rentalSystem->saveAllData();
        showMessage("Success", "Car updated successfully!");
        refreshCarTable();
    }
}

void MainWindow::onDeleteCar()
{
    int row = carTable->currentRow();
    if (row < 0) return;

    QString carID = carTable->item(row, 0)->text();
    Car* car = rentalSystem->getCar(carID.toStdString());

    if (!car) {
        showError("Error", "Car not found!");
        return;
    }

    if (car->getStatus() == RENTED) {
        showError("Error", "Cannot delete a car that is currently rented!");
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirm Delete",
        QString("Are you sure you want to delete car %1?").arg(carID),
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (rentalSystem->removeCar(carID.toStdString())) {
            showMessage("Success", "Car deleted successfully!");
            refreshCarTable();
            updateStatusBar();
        } else {
            showError("Error", "Failed to delete car!");
        }
    }
}

void MainWindow::onUpdateCarStatus()
{
    int row = carTable->currentRow();
    if (row < 0) return;

    QString carID = carTable->item(row, 0)->text();
    Car* car = rentalSystem->getCar(carID.toStdString());

    if (!car) {
        showError("Error", "Car not found!");
        return;
    }

    QStringList statuses;
    statuses << "Ready" << "Maintenance" << "Rented";

    bool ok;
    QString newStatus = QInputDialog::getItem(this, "Update Status",
        QString("Current status: %1\nSelect new status:").arg(getStatusString(car->getStatus())),
        statuses, car->getStatus(), false, &ok);

    if (ok && !newStatus.isEmpty()) {
        CarStatus status;
        if (newStatus == "Ready") status = READY;
        else if (newStatus == "Maintenance") status = MAINTENANCE;
        else status = RENTED;

        if (rentalSystem->updateCarStatus(carID.toStdString(), status)) {
            showMessage("Success", "Status updated successfully!");
            refreshCarTable();
            updateStatusBar();
        } else {
            showError("Error", "Failed to update status!");
        }
    }
}

void MainWindow::onCarFilterChanged(int)
{
    refreshCarTable();
}

void MainWindow::onCreateRental()
{
    auto availableCars = rentalSystem->getAvailableCars();
    if (availableCars.empty()) {
        showError("Error", "No cars available for rental!");
        return;
    }

    RentalDialog dialog(this, rentalSystem);
    dialog.setWindowTitle("Create New Rental");

    if (dialog.exec() == QDialog::Accepted) {
        refreshCarTable();
        refreshTransactionTable();
        updateStatusBar();
        showMessage("Success", "Rental created successfully!");
    }
}

void MainWindow::onReturnCar()
{
    auto activeRentals = rentalSystem->getActiveRentals();
    if (activeRentals.empty()) {
        showError("Error", "No active rentals to return!");
        return;
    }

    // Check if a transaction is selected
    int row = transactionTable->currentRow();
    QString selectedTransID = "";

    if (row >= 0) {
        QString status = transactionTable->item(row, 8)->text();
        if (status == "Active") {
            selectedTransID = transactionTable->item(row, 0)->text();
        }
    }

    ReturnDialog dialog(this, rentalSystem, selectedTransID);
    dialog.setWindowTitle("Return Car");

    if (dialog.exec() == QDialog::Accepted) {
        refreshCarTable();
        refreshTransactionTable();
        updateStatusBar();
        showMessage("Success", "Car returned successfully!");
    }
}

void MainWindow::onCarTableSelectionChanged()
{
    bool hasSelection = carTable->currentRow() >= 0;
    btnEditCar->setEnabled(hasSelection);
    btnDeleteCar->setEnabled(hasSelection);
    btnUpdateStatus->setEnabled(hasSelection);
}

void MainWindow::onTransactionTableSelectionChanged()
{
    int row = transactionTable->currentRow();
    if (row >= 0) {
        QString status = transactionTable->item(row, 8)->text();
        btnReturnCar->setEnabled(status == "Active");
    } else {
        btnReturnCar->setEnabled(false);
    }
}

void MainWindow::onAbout()
{
    QMessageBox::about(this, "About Car Rental Management System",
        "Car Rental Management System v1.0\n\n"
        "A simple car rental management application.\n\n"
        "Features:\n"
        "- Car management (add, edit, delete)\n"
        "- Rental transactions\n"
        "- Car return with penalty calculation\n"
        "- Data persistence to file\n\n"
        "Built with Qt and C++");
}

void MainWindow::onRefresh()
{
    rentalSystem->loadAllData();
    refreshCarTable();
    refreshTransactionTable();
    updateStatusBar();
    statusBar()->showMessage("Data refreshed", 2000);
}