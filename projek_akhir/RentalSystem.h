#ifndef RENTAL_SYSTEM_H
#define RENTAL_SYSTEM_H

#include <vector>
#include <string>
#include "Car.h"
#include "RentalTransaction.h"
#include "FileManager.h"
#include "Customer.h"
#include "Date.h"

class RentalSystem {
private:
    std::vector<Car> cars;
    std::vector<RentalTransaction> transactions;
    FileManager fileManager;
    int transactionCounter;

    std::string generateTransactionID();

public:
    // Constructor & Destructor
    RentalSystem();
    ~RentalSystem();

    // Initialization
    void initialize();

    // Car management
    bool addCar(const Car& car);
    bool removeCar(const std::string& carID);
    bool updateCarStatus(const std::string& carID, CarStatus status);
    Car* getCar(const std::string& carID);
    std::vector<Car> getAvailableCars();
    std::vector<Car> getAllCars();
    std::vector<Car> getCarsByStatus(CarStatus status);

    // Transaction management
    bool createRental(const std::string& carID, const Customer& customer,
                     const Date& startDate, const Date& endDate);
    bool returnCar(const std::string& transactionID, const Date& returnDate);
    RentalTransaction* getTransaction(const std::string& transactionID);
    std::vector<RentalTransaction> getAllTransactions();
    std::vector<RentalTransaction> getActiveRentals();

    // Data persistence
    void saveAllData();
    void loadAllData();

    // Display methods
    void displayCars(const std::vector<Car>& carsToDisplay);
    void displayTransactions(const std::vector<RentalTransaction>& transToDisplay);

    // Validation
    bool isCarIDUnique(const std::string& carID);
    bool isLicensePlateUnique(const std::string& plate);
};

#endif