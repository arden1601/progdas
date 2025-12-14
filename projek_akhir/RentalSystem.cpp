#include "RentalSystem.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <ctime>

RentalSystem::RentalSystem() : transactionCounter(1000) {}

RentalSystem::~RentalSystem() {
    saveAllData();
}

void RentalSystem::initialize() {
    loadAllData();
}

std::string RentalSystem::generateTransactionID() {
    std::stringstream ss;
    ss << "TRX" << std::setfill('0') << std::setw(5) << ++transactionCounter;
    return ss.str();
}

// Car management
bool RentalSystem::addCar(const Car& car) {
    if (!isCarIDUnique(car.getCarID())) {
        std::cout << "Error: Car ID already exists!" << std::endl;
        return false;
    }
    if (!isLicensePlateUnique(car.getLicensePlate())) {
        std::cout << "Error: License plate already exists!" << std::endl;
        return false;
    }

    cars.push_back(car);
    saveAllData();
    return true;
}

bool RentalSystem::removeCar(const std::string& carID) {
    auto it = std::find_if(cars.begin(), cars.end(),
        [&carID](const Car& c) { return c.getCarID() == carID; });

    if (it != cars.end()) {
        // Check if car is currently rented
        if (it->getStatus() == RENTED) {
            std::cout << "Error: Cannot remove a car that is currently rented!" << std::endl;
            return false;
        }
        cars.erase(it);
        saveAllData();
        return true;
    }
    return false;
}

bool RentalSystem::updateCarStatus(const std::string& carID, CarStatus status) {
    Car* car = getCar(carID);
    if (car != nullptr) {
        car->setStatus(status);
        saveAllData();
        return true;
    }
    return false;
}

Car* RentalSystem::getCar(const std::string& carID) {
    auto it = std::find_if(cars.begin(), cars.end(),
        [&carID](const Car& c) { return c.getCarID() == carID; });

    if (it != cars.end()) {
        return &(*it);
    }
    return nullptr;
}

std::vector<Car> RentalSystem::getAvailableCars() {
    std::vector<Car> available;
    for (const auto& car : cars) {
        if (car.isAvailable()) {
            available.push_back(car);
        }
    }
    return available;
}

std::vector<Car> RentalSystem::getAllCars() {
    return cars;
}

std::vector<Car> RentalSystem::getCarsByStatus(CarStatus status) {
    std::vector<Car> filtered;
    for (const auto& car : cars) {
        if (car.getStatus() == status) {
            filtered.push_back(car);
        }
    }
    return filtered;
}

// Transaction management
bool RentalSystem::createRental(const std::string& carID, const Customer& customer,
                                const Date& startDate, const Date& endDate) {
    Car* car = getCar(carID);
    if (car == nullptr) {
        std::cout << "Error: Car not found!" << std::endl;
        return false;
    }

    if (!car->isAvailable()) {
        std::cout << "Error: Car is not available!" << std::endl;
        return false;
    }

    if (endDate < startDate) {
        std::cout << "Error: End date must be after start date!" << std::endl;
        return false;
    }

    std::string transID = generateTransactionID();
    RentalTransaction transaction(transID, carID, customer, startDate, endDate);
    transaction.calculateTotalCost(car->getPricePerDay());

    transactions.push_back(transaction);
    car->setStatus(RENTED);
    saveAllData();

    std::cout << "\nRental created successfully!" << std::endl;
    std::cout << "Transaction ID: " << transID << std::endl;
    std::cout << "Total Cost: Rp " << std::fixed << std::setprecision(0)
              << transaction.getTotalCost() << std::endl;

    return true;
}

bool RentalSystem::returnCar(const std::string& transactionID, const Date& returnDate) {
    RentalTransaction* trans = getTransaction(transactionID);
    if (trans == nullptr) {
        std::cout << "Error: Transaction not found!" << std::endl;
        return false;
    }

    if (trans->getStatus() != ACTIVE) {
        std::cout << "Error: Transaction is not active!" << std::endl;
        return false;
    }

    Car* car = getCar(trans->getCarID());
    if (car == nullptr) {
        std::cout << "Error: Associated car not found!" << std::endl;
        return false;
    }

    // Calculate penalty if any
    trans->calculatePenalty(returnDate, car->getPricePerDay());
    trans->completeTransaction(returnDate);
    car->setStatus(READY);
    saveAllData();

    std::cout << "\nCar returned successfully!" << std::endl;
    if (trans->getPenalty() > 0) {
        std::cout << "Late return penalty: Rp " << std::fixed << std::setprecision(0)
                  << trans->getPenalty() << std::endl;
        std::cout << "Total amount due: Rp "
                  << (trans->getTotalCost() + trans->getPenalty()) << std::endl;
    }

    return true;
}

RentalTransaction* RentalSystem::getTransaction(const std::string& transactionID) {
    auto it = std::find_if(transactions.begin(), transactions.end(),
        [&transactionID](const RentalTransaction& t) {
            return t.getTransactionID() == transactionID;
        });

    if (it != transactions.end()) {
        return &(*it);
    }
    return nullptr;
}

std::vector<RentalTransaction> RentalSystem::getAllTransactions() {
    return transactions;
}

std::vector<RentalTransaction> RentalSystem::getActiveRentals() {
    std::vector<RentalTransaction> active;
    for (const auto& trans : transactions) {
        if (trans.getStatus() == ACTIVE) {
            active.push_back(trans);
        }
    }
    return active;
}

// Data persistence
void RentalSystem::saveAllData() {
    fileManager.saveCars(cars);
    fileManager.saveTransactions(transactions);
}

void RentalSystem::loadAllData() {
    cars = fileManager.loadCars();
    transactions = fileManager.loadTransactions();

    // Update transaction counter based on loaded transactions
    if (!transactions.empty()) {
        // Find the highest transaction number
        int maxNum = 0;
        for (const auto& trans : transactions) {
            std::string id = trans.getTransactionID();
            if (id.length() > 3) {
                int num = std::stoi(id.substr(3));
                if (num > maxNum) {
                    maxNum = num;
                }
            }
        }
        transactionCounter = maxNum;
    }
}

// Display methods
void RentalSystem::displayCars(const std::vector<Car>& carsToDisplay) {
    if (carsToDisplay.empty()) {
        std::cout << "No cars to display." << std::endl;
        return;
    }

    std::cout << "\n" << std::string(100, '=') << std::endl;
    std::cout << std::left << std::setw(10) << "ID"
              << std::setw(15) << "Brand"
              << std::setw(15) << "Model"
              << std::setw(8) << "Year"
              << std::setw(15) << "License"
              << std::setw(15) << "Price/Day"
              << std::setw(12) << "Status" << std::endl;
    std::cout << std::string(100, '-') << std::endl;

    for (const auto& car : carsToDisplay) {
        std::cout << std::left << std::setw(10) << car.getCarID()
                  << std::setw(15) << car.getBrand()
                  << std::setw(15) << car.getModel()
                  << std::setw(8) << car.getYear()
                  << std::setw(15) << car.getLicensePlate()
                  << "Rp " << std::setw(12) << std::fixed << std::setprecision(0)
                  << car.getPricePerDay()
                  << std::setw(12) << car.getStatusString() << std::endl;
    }
    std::cout << std::string(100, '=') << std::endl;
}

void RentalSystem::displayTransactions(const std::vector<RentalTransaction>& transToDisplay) {
    if (transToDisplay.empty()) {
        std::cout << "No transactions to display." << std::endl;
        return;
    }

    std::cout << "\n" << std::string(100, '=') << std::endl;
    for (const auto& trans : transToDisplay) {
        std::cout << trans.toString() << std::endl;
        std::cout << std::string(50, '-') << std::endl;
    }
    std::cout << std::string(100, '=') << std::endl;
}

// Validation methods
bool RentalSystem::isCarIDUnique(const std::string& carID) {
    for (const auto& car : cars) {
        if (car.getCarID() == carID) {
            return false;
        }
    }
    return true;
}

bool RentalSystem::isLicensePlateUnique(const std::string& plate) {
    for (const auto& car : cars) {
        if (car.getLicensePlate() == plate) {
            return false;
        }
    }
    return true;
}