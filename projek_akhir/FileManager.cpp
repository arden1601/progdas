#include "FileManager.h"
#include <fstream>
#include <iostream>
#include <ctime>
#include <sstream>

FileManager::FileManager() {
    carsFile = "cars.txt";
    transactionsFile = "transactions.txt";
}

bool FileManager::fileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}

void FileManager::createBackup() {
    if (fileExists(carsFile)) {
        std::ifstream src(carsFile, std::ios::binary);
        std::ofstream dst(carsFile + ".backup", std::ios::binary);
        dst << src.rdbuf();
        src.close();
        dst.close();
    }

    if (fileExists(transactionsFile)) {
        std::ifstream src(transactionsFile, std::ios::binary);
        std::ofstream dst(transactionsFile + ".backup", std::ios::binary);
        dst << src.rdbuf();
        src.close();
        dst.close();
    }
}

bool FileManager::saveCars(const std::vector<Car>& cars) {
    std::ofstream file(carsFile);
    if (!file.is_open()) {
        return false;
    }

    // Write header
    file << "# CarID|Brand|Model|Year|LicensePlate|PricePerDay|Status" << std::endl;

    // Write car data
    for (const auto& car : cars) {
        file << car.toFileString() << std::endl;
    }

    file.close();
    return true;
}

std::vector<Car> FileManager::loadCars() {
    std::vector<Car> cars;
    std::ifstream file(carsFile);

    if (!file.is_open()) {
        return cars;
    }

    std::string line;
    while (std::getline(file, line)) {
        // Skip comments and empty lines
        if (line.empty() || line[0] == '#') {
            continue;
        }

        Car car;
        try {
            car.fromFileString(line);
            cars.push_back(car);
        } catch (...) {
            std::cerr << "Error parsing car data: " << line << std::endl;
        }
    }

    file.close();
    return cars;
}

bool FileManager::saveTransactions(const std::vector<RentalTransaction>& transactions) {
    std::ofstream file(transactionsFile);
    if (!file.is_open()) {
        return false;
    }

    // Write header
    file << "# TransID|CarID|Name|NIK|Phone|StartDate|EndDate|ReturnDate|TotalCost|Penalty|Status" << std::endl;

    // Write transaction data
    for (const auto& trans : transactions) {
        file << trans.toFileString() << std::endl;
    }

    file.close();
    return true;
}

std::vector<RentalTransaction> FileManager::loadTransactions() {
    std::vector<RentalTransaction> transactions;
    std::ifstream file(transactionsFile);

    if (!file.is_open()) {
        return transactions;
    }

    std::string line;
    while (std::getline(file, line)) {
        // Skip comments and empty lines
        if (line.empty() || line[0] == '#') {
            continue;
        }

        RentalTransaction trans;
        try {
            trans.fromFileString(line);
            transactions.push_back(trans);
        } catch (...) {
            std::cerr << "Error parsing transaction data: " << line << std::endl;
        }
    }

    file.close();
    return transactions;
}

bool FileManager::appendTransaction(const RentalTransaction& transaction) {
    std::ofstream file(transactionsFile, std::ios::app);
    if (!file.is_open()) {
        return false;
    }

    file << transaction.toFileString() << std::endl;
    file.close();
    return true;
}