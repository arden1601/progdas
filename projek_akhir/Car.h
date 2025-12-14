#ifndef CAR_H
#define CAR_H

#include <string>
#include <sstream>

enum CarStatus {
    READY,
    MAINTENANCE,
    RENTED
};

class Car {
private:
    std::string carID;
    std::string brand;
    std::string model;
    int year;
    std::string licensePlate;
    double pricePerDay;
    CarStatus status;

public:
    // Constructors
    Car();
    Car(std::string id, std::string br, std::string md, int yr, std::string plate, double price);

    // Getters
    std::string getCarID() const { return carID; }
    std::string getBrand() const { return brand; }
    std::string getModel() const { return model; }
    int getYear() const { return year; }
    std::string getLicensePlate() const { return licensePlate; }
    double getPricePerDay() const { return pricePerDay; }
    CarStatus getStatus() const { return status; }
    std::string getStatusString() const;

    // Setters
    void setStatus(CarStatus st) { status = st; }
    void setCarID(const std::string& id) { carID = id; }
    void setBrand(const std::string& br) { brand = br; }
    void setModel(const std::string& md) { model = md; }
    void setYear(int yr) { year = yr; }
    void setLicensePlate(const std::string& plate) { licensePlate = plate; }
    void setPricePerDay(double price) { pricePerDay = price; }

    // Utility methods
    bool isAvailable() const { return status == READY; }
    std::string toString() const;
    std::string toFileString() const;
    void fromFileString(const std::string& data);
};

#endif