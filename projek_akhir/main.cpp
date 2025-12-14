#include <iostream>
#include "RentalSystem.h"
#include "MenuSystem.h"

int main() {
    std::cout << "Starting Car Rental Management System...\n";
    std::cout << "Loading data...\n";

    // Create rental system
    RentalSystem rentalSystem;
    rentalSystem.initialize();

    // Create menu system
    MenuSystem menu(&rentalSystem);

    // Run the application
    menu.run();

    return 0;
}