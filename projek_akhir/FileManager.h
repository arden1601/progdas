#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <string>
#include <vector>
#include "Car.h"
#include "RentalTransaction.h"

class FileManager {
private:
    std::string carsFile;
    std::string transactionsFile;

public:
    // Constructor
    FileManager();

    // File operations
    bool saveCars(const std::vector<Car>& cars);
    std::vector<Car> loadCars();
    bool saveTransactions(const std::vector<RentalTransaction>& transactions);
    std::vector<RentalTransaction> loadTransactions();
    bool appendTransaction(const RentalTransaction& transaction);

    // Utility methods
    bool fileExists(const std::string& filename);
    void createBackup();
};

#endif