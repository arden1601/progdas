#ifndef MENU_SYSTEM_H
#define MENU_SYSTEM_H

#include <string>
#include "RentalSystem.h"
#include "Date.h"

class MenuSystem {
private:
    RentalSystem* rentalSystem;

    // Menu display methods
    void displayMainMenu();
    void displayCarMenu();
    void displayTransactionMenu();
    void displayReportMenu();

    // Car management handlers
    void handleAddCar();
    void handleRemoveCar();
    void handleUpdateStatus();
    void handleViewCars();

    // Transaction handlers
    void handleCreateRental();
    void handleReturnCar();
    void handleViewTransactions();

    // Report handlers
    void handleAllCars();
    void handleAvailableCars();
    void handleRentedCars();
    void handleMaintenanceCars();
    void handleAllTransactions();
    void handleActiveRentals();

    // Utility methods
    void clearScreen();
    void pauseScreen();
    int getIntInput(const std::string& prompt);
    double getDoubleInput(const std::string& prompt);
    std::string getStringInput(const std::string& prompt);
    Date getDateInput(const std::string& prompt);
    bool confirmAction(const std::string& message);

public:
    // Constructor
    MenuSystem(RentalSystem* system);

    // Main run method
    void run();
};

#endif