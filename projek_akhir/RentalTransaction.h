#ifndef RENTAL_TRANSACTION_H
#define RENTAL_TRANSACTION_H

#include <string>
#include "Customer.h"
#include "Date.h"

enum TransactionStatus {
    ACTIVE,
    COMPLETED,
    CANCELLED
};

class RentalTransaction {
private:
    std::string transactionID;
    std::string carID;
    Customer customer;
    Date startDate;
    Date endDate;
    Date actualReturnDate;
    double totalCost;
    double penalty;
    TransactionStatus status;

public:
    // Constructors
    RentalTransaction();
    RentalTransaction(std::string tID, std::string cID, Customer cust, Date start, Date end);

    // Getters
    std::string getTransactionID() const { return transactionID; }
    std::string getCarID() const { return carID; }
    Customer getCustomer() const { return customer; }
    Date getStartDate() const { return startDate; }
    Date getEndDate() const { return endDate; }
    Date getActualReturnDate() const { return actualReturnDate; }
    double getTotalCost() const { return totalCost; }
    double getPenalty() const { return penalty; }
    TransactionStatus getStatus() const { return status; }
    std::string getStatusString() const;

    // Setters
    void setTransactionID(const std::string& id) { transactionID = id; }
    void setCarID(const std::string& id) { carID = id; }
    void setCustomer(const Customer& cust) { customer = cust; }
    void setStartDate(const Date& date) { startDate = date; }
    void setEndDate(const Date& date) { endDate = date; }
    void setActualReturnDate(const Date& date) { actualReturnDate = date; }
    void setTotalCost(double cost) { totalCost = cost; }
    void setPenalty(double pen) { penalty = pen; }
    void setStatus(TransactionStatus st) { status = st; }

    // Business methods
    void calculateTotalCost(double pricePerDay);
    void calculatePenalty(const Date& returnDate, double pricePerDay);
    void completeTransaction(const Date& returnDate);

    // Utility methods
    std::string toString() const;
    std::string toFileString() const;
    void fromFileString(const std::string& data);
};

#endif