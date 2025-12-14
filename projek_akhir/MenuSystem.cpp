#include "MenuSystem.h"
#include <iostream>
#include <limits>
#include <iomanip>
#include <cstdlib>

#ifdef _WIN32
    #define CLEAR_SCREEN "cls"
#else
    #define CLEAR_SCREEN "clear"
#endif

MenuSystem::MenuSystem(RentalSystem* system) : rentalSystem(system) {}

void MenuSystem::clearScreen() {
    system(CLEAR_SCREEN);
}

void MenuSystem::pauseScreen() {
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

int MenuSystem::getIntInput(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            std::cin.ignore();
            return value;
        } else {
            std::cout << "Invalid input! Please enter a number.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

double MenuSystem::getDoubleInput(const std::string& prompt) {
    double value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value && value > 0) {
            std::cin.ignore();
            return value;
        } else {
            std::cout << "Invalid input! Please enter a positive number.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

std::string MenuSystem::getStringInput(const std::string& prompt) {
    std::string value;
    std::cout << prompt;
    std::getline(std::cin, value);
    return value;
}

Date MenuSystem::getDateInput(const std::string& prompt) {
    while (true) {
        std::cout << prompt << " (DD MM YYYY): ";
        int day, month, year;
        if (std::cin >> day >> month >> year) {
            Date date(day, month, year);
            if (date.isValid()) {
                std::cin.ignore();
                return date;
            } else {
                std::cout << "Invalid date! Please try again.\n";
            }
        } else {
            std::cout << "Invalid input format!\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

bool MenuSystem::confirmAction(const std::string& message) {
    std::cout << message << " (Y/N): ";
    char choice;
    std::cin >> choice;
    std::cin.ignore();
    return (choice == 'Y' || choice == 'y');
}

void MenuSystem::displayMainMenu() {
    std::cout << "\n";
    std::cout << "╔═══════════════════════════════════════════╗\n";
    std::cout << "║     CAR RENTAL MANAGEMENT SYSTEM          ║\n";
    std::cout << "╚═══════════════════════════════════════════╝\n";
    std::cout << "\n";
    std::cout << "1. Car Management\n";
    std::cout << "2. Transaction Management\n";
    std::cout << "3. Reports\n";
    std::cout << "0. Exit\n";
    std::cout << "\n";
}

void MenuSystem::displayCarMenu() {
    std::cout << "\n";
    std::cout << "═══════════ CAR MANAGEMENT ═══════════\n";
    std::cout << "1. Add New Car\n";
    std::cout << "2. Remove Car\n";
    std::cout << "3. Update Car Status\n";
    std::cout << "4. View All Cars\n";
    std::cout << "0. Back to Main Menu\n";
    std::cout << "\n";
}

void MenuSystem::displayTransactionMenu() {
    std::cout << "\n";
    std::cout << "═══════ TRANSACTION MANAGEMENT ═══════\n";
    std::cout << "1. Create New Rental\n";
    std::cout << "2. Return Car\n";
    std::cout << "3. View All Transactions\n";
    std::cout << "0. Back to Main Menu\n";
    std::cout << "\n";
}

void MenuSystem::displayReportMenu() {
    std::cout << "\n";
    std::cout << "═══════════ REPORTS ═══════════\n";
    std::cout << "1. View All Cars\n";
    std::cout << "2. View Available Cars\n";
    std::cout << "3. View Rented Cars\n";
    std::cout << "4. View Maintenance Cars\n";
    std::cout << "5. View All Transactions\n";
    std::cout << "6. View Active Rentals\n";
    std::cout << "0. Back to Main Menu\n";
    std::cout << "\n";
}

void MenuSystem::handleAddCar() {
    clearScreen();
    std::cout << "\n═══════════ ADD NEW CAR ═══════════\n\n";

    std::string carID = getStringInput("Enter Car ID: ");

    // Check if ID already exists
    if (!rentalSystem->isCarIDUnique(carID)) {
        std::cout << "\nError: Car ID already exists!\n";
        pauseScreen();
        return;
    }

    std::string brand = getStringInput("Enter Brand: ");
    std::string model = getStringInput("Enter Model: ");
    int year = getIntInput("Enter Year: ");

    if (year < 1900 || year > 2025) {
        std::cout << "\nError: Invalid year!\n";
        pauseScreen();
        return;
    }

    std::string licensePlate = getStringInput("Enter License Plate: ");

    // Check if license plate already exists
    if (!rentalSystem->isLicensePlateUnique(licensePlate)) {
        std::cout << "\nError: License plate already exists!\n";
        pauseScreen();
        return;
    }

    double pricePerDay = getDoubleInput("Enter Price per Day (Rp): ");

    Car newCar(carID, brand, model, year, licensePlate, pricePerDay);

    if (rentalSystem->addCar(newCar)) {
        std::cout << "\nCar added successfully!\n";
    } else {
        std::cout << "\nFailed to add car!\n";
    }

    pauseScreen();
}

void MenuSystem::handleRemoveCar() {
    clearScreen();
    std::cout << "\n═══════════ REMOVE CAR ═══════════\n\n";

    // Display all cars
    rentalSystem->displayCars(rentalSystem->getAllCars());

    std::string carID = getStringInput("\nEnter Car ID to remove: ");

    Car* car = rentalSystem->getCar(carID);
    if (car == nullptr) {
        std::cout << "\nError: Car not found!\n";
        pauseScreen();
        return;
    }

    std::cout << "\nCar Details:\n";
    std::cout << car->toString() << "\n\n";

    if (confirmAction("Are you sure you want to remove this car?")) {
        if (rentalSystem->removeCar(carID)) {
            std::cout << "\nCar removed successfully!\n";
        } else {
            std::cout << "\nFailed to remove car!\n";
        }
    } else {
        std::cout << "\nOperation cancelled.\n";
    }

    pauseScreen();
}

void MenuSystem::handleUpdateStatus() {
    clearScreen();
    std::cout << "\n═══════════ UPDATE CAR STATUS ═══════════\n\n";

    // Display all cars
    rentalSystem->displayCars(rentalSystem->getAllCars());

    std::string carID = getStringInput("\nEnter Car ID: ");

    Car* car = rentalSystem->getCar(carID);
    if (car == nullptr) {
        std::cout << "\nError: Car not found!\n";
        pauseScreen();
        return;
    }

    std::cout << "\nCurrent Status: " << car->getStatusString() << "\n\n";
    std::cout << "Select New Status:\n";
    std::cout << "1. Ready\n";
    std::cout << "2. Maintenance\n";
    std::cout << "3. Rented\n";

    int choice = getIntInput("Choice: ");
    CarStatus newStatus;

    switch (choice) {
        case 1: newStatus = READY; break;
        case 2: newStatus = MAINTENANCE; break;
        case 3: newStatus = RENTED; break;
        default:
            std::cout << "\nInvalid choice!\n";
            pauseScreen();
            return;
    }

    if (rentalSystem->updateCarStatus(carID, newStatus)) {
        std::cout << "\nStatus updated successfully!\n";
    } else {
        std::cout << "\nFailed to update status!\n";
    }

    pauseScreen();
}

void MenuSystem::handleViewCars() {
    clearScreen();
    std::cout << "\n═══════════ ALL CARS ═══════════\n";
    rentalSystem->displayCars(rentalSystem->getAllCars());
    pauseScreen();
}

void MenuSystem::handleCreateRental() {
    clearScreen();
    std::cout << "\n═══════════ CREATE NEW RENTAL ═══════════\n\n";

    // Show available cars
    auto availableCars = rentalSystem->getAvailableCars();
    if (availableCars.empty()) {
        std::cout << "No cars available for rental!\n";
        pauseScreen();
        return;
    }

    std::cout << "Available Cars:\n";
    rentalSystem->displayCars(availableCars);

    // Get car selection
    std::string carID = getStringInput("\nEnter Car ID to rent: ");

    Car* car = rentalSystem->getCar(carID);
    if (car == nullptr || !car->isAvailable()) {
        std::cout << "\nError: Car not available!\n";
        pauseScreen();
        return;
    }

    // Get customer information
    std::cout << "\nCustomer Information:\n";
    std::string name = getStringInput("Name: ");
    std::string nik = getStringInput("NIK: ");
    std::string phone = getStringInput("Phone: ");

    Customer customer(name, nik, phone);

    // Get rental dates
    std::cout << "\nRental Period:\n";
    Date startDate = getDateInput("Start Date");
    Date endDate = getDateInput("End Date");

    // Calculate and display cost
    int days = startDate.daysBetween(endDate);
    if (days == 0) days = 1;
    double totalCost = days * car->getPricePerDay();

    std::cout << "\n--- Rental Summary ---\n";
    std::cout << "Car: " << car->getBrand() << " " << car->getModel() << "\n";
    std::cout << "Customer: " << name << "\n";
    std::cout << "Period: " << startDate.toString() << " to " << endDate.toString() << "\n";
    std::cout << "Days: " << days << "\n";
    std::cout << "Total Cost: Rp " << std::fixed << std::setprecision(0) << totalCost << "\n";

    if (confirmAction("\nConfirm rental?")) {
        if (rentalSystem->createRental(carID, customer, startDate, endDate)) {
            std::cout << "\nRental created successfully!\n";
        } else {
            std::cout << "\nFailed to create rental!\n";
        }
    } else {
        std::cout << "\nRental cancelled.\n";
    }

    pauseScreen();
}

void MenuSystem::handleReturnCar() {
    clearScreen();
    std::cout << "\n═══════════ RETURN CAR ═══════════\n\n";

    // Show active rentals
    auto activeRentals = rentalSystem->getActiveRentals();
    if (activeRentals.empty()) {
        std::cout << "No active rentals!\n";
        pauseScreen();
        return;
    }

    std::cout << "Active Rentals:\n";
    rentalSystem->displayTransactions(activeRentals);

    std::string transID = getStringInput("\nEnter Transaction ID: ");

    RentalTransaction* trans = rentalSystem->getTransaction(transID);
    if (trans == nullptr || trans->getStatus() != ACTIVE) {
        std::cout << "\nError: Active transaction not found!\n";
        pauseScreen();
        return;
    }

    Car* car = rentalSystem->getCar(trans->getCarID());
    if (car == nullptr) {
        std::cout << "\nError: Associated car not found!\n";
        pauseScreen();
        return;
    }

    std::cout << "\nTransaction Details:\n";
    std::cout << trans->toString() << "\n";

    Date returnDate = getDateInput("\nReturn Date");

    // Calculate penalty if late
    double penalty = 0;
    if (returnDate > trans->getEndDate()) {
        int lateDays = trans->getEndDate().daysBetween(returnDate);
        penalty = lateDays * (car->getPricePerDay() * 1.5);
        std::cout << "\nLate Return!\n";
        std::cout << "Days Late: " << lateDays << "\n";
        std::cout << "Penalty: Rp " << std::fixed << std::setprecision(0) << penalty << "\n";
        std::cout << "Total Amount: Rp " << (trans->getTotalCost() + penalty) << "\n";
    }

    if (confirmAction("\nConfirm return?")) {
        if (rentalSystem->returnCar(transID, returnDate)) {
            std::cout << "\nCar returned successfully!\n";
        } else {
            std::cout << "\nFailed to return car!\n";
        }
    } else {
        std::cout << "\nReturn cancelled.\n";
    }

    pauseScreen();
}

void MenuSystem::handleViewTransactions() {
    clearScreen();
    std::cout << "\n═══════════ ALL TRANSACTIONS ═══════════\n";
    rentalSystem->displayTransactions(rentalSystem->getAllTransactions());
    pauseScreen();
}

void MenuSystem::handleAllCars() {
    clearScreen();
    std::cout << "\n═══════════ ALL CARS ═══════════\n";
    rentalSystem->displayCars(rentalSystem->getAllCars());
    pauseScreen();
}

void MenuSystem::handleAvailableCars() {
    clearScreen();
    std::cout << "\n═══════════ AVAILABLE CARS ═══════════\n";
    rentalSystem->displayCars(rentalSystem->getAvailableCars());
    pauseScreen();
}

void MenuSystem::handleRentedCars() {
    clearScreen();
    std::cout << "\n═══════════ RENTED CARS ═══════════\n";
    rentalSystem->displayCars(rentalSystem->getCarsByStatus(RENTED));
    pauseScreen();
}

void MenuSystem::handleMaintenanceCars() {
    clearScreen();
    std::cout << "\n═══════════ MAINTENANCE CARS ═══════════\n";
    rentalSystem->displayCars(rentalSystem->getCarsByStatus(MAINTENANCE));
    pauseScreen();
}

void MenuSystem::handleAllTransactions() {
    clearScreen();
    std::cout << "\n═══════════ ALL TRANSACTIONS ═══════════\n";
    rentalSystem->displayTransactions(rentalSystem->getAllTransactions());
    pauseScreen();
}

void MenuSystem::handleActiveRentals() {
    clearScreen();
    std::cout << "\n═══════════ ACTIVE RENTALS ═══════════\n";
    rentalSystem->displayTransactions(rentalSystem->getActiveRentals());
    pauseScreen();
}

void MenuSystem::run() {
    bool running = true;

    while (running) {
        clearScreen();
        displayMainMenu();

        int choice = getIntInput("Enter your choice: ");

        switch (choice) {
            case 1: {
                // Car Management
                bool carMenuRunning = true;
                while (carMenuRunning) {
                    clearScreen();
                    displayCarMenu();
                    int carChoice = getIntInput("Enter your choice: ");

                    switch (carChoice) {
                        case 1: handleAddCar(); break;
                        case 2: handleRemoveCar(); break;
                        case 3: handleUpdateStatus(); break;
                        case 4: handleViewCars(); break;
                        case 0: carMenuRunning = false; break;
                        default: std::cout << "Invalid choice!\n"; pauseScreen();
                    }
                }
                break;
            }
            case 2: {
                // Transaction Management
                bool transMenuRunning = true;
                while (transMenuRunning) {
                    clearScreen();
                    displayTransactionMenu();
                    int transChoice = getIntInput("Enter your choice: ");

                    switch (transChoice) {
                        case 1: handleCreateRental(); break;
                        case 2: handleReturnCar(); break;
                        case 3: handleViewTransactions(); break;
                        case 0: transMenuRunning = false; break;
                        default: std::cout << "Invalid choice!\n"; pauseScreen();
                    }
                }
                break;
            }
            case 3: {
                // Reports
                bool reportMenuRunning = true;
                while (reportMenuRunning) {
                    clearScreen();
                    displayReportMenu();
                    int reportChoice = getIntInput("Enter your choice: ");

                    switch (reportChoice) {
                        case 1: handleAllCars(); break;
                        case 2: handleAvailableCars(); break;
                        case 3: handleRentedCars(); break;
                        case 4: handleMaintenanceCars(); break;
                        case 5: handleAllTransactions(); break;
                        case 6: handleActiveRentals(); break;
                        case 0: reportMenuRunning = false; break;
                        default: std::cout << "Invalid choice!\n"; pauseScreen();
                    }
                }
                break;
            }
            case 0: {
                if (confirmAction("Are you sure you want to exit?")) {
                    std::cout << "Saving data...\n";
                    rentalSystem->saveAllData();
                    std::cout << "Thank you for using Car Rental Management System!\n";
                    running = false;
                }
                break;
            }
            default:
                std::cout << "Invalid choice! Please try again.\n";
                pauseScreen();
        }
    }
}