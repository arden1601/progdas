#include "Car.h"
#include <iomanip>
#include <sstream>

Car::Car() : carID(""), brand(""), model(""), year(0), licensePlate(""), pricePerDay(0.0), status(READY) {}

Car::Car(std::string id, std::string br, std::string md, int yr, std::string plate, double price)
    : carID(id), brand(br), model(md), year(yr), licensePlate(plate), pricePerDay(price), status(READY) {}

std::string Car::getStatusString() const {
    switch(status) {
        case READY: return "Ready";
        case MAINTENANCE: return "Maintenance";
        case RENTED: return "Rented";
        default: return "Unknown";
    }
}

std::string Car::toString() const {
    std::stringstream ss;
    ss << "ID: " << carID << "\n"
       << "Brand: " << brand << "\n"
       << "Model: " << model << "\n"
       << "Year: " << year << "\n"
       << "License Plate: " << licensePlate << "\n"
       << "Price/Day: Rp " << std::fixed << std::setprecision(0) << pricePerDay << "\n"
       << "Status: " << getStatusString();
    return ss.str();
}

std::string Car::toFileString() const {
    std::stringstream ss;
    ss << carID << "|" << brand << "|" << model << "|" << year << "|"
       << licensePlate << "|" << pricePerDay << "|" << status;
    return ss.str();
}

void Car::fromFileString(const std::string& data) {
    std::stringstream ss(data);
    std::string item;
    int fieldCount = 0;

    while (std::getline(ss, item, '|')) {
        switch(fieldCount) {
            case 0: carID = item; break;
            case 1: brand = item; break;
            case 2: model = item; break;
            case 3: year = std::stoi(item); break;
            case 4: licensePlate = item; break;
            case 5: pricePerDay = std::stod(item); break;
            case 6: status = static_cast<CarStatus>(std::stoi(item)); break;
        }
        fieldCount++;
    }
}