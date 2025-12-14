#include "RentalTransaction.h"
#include <sstream>
#include <iomanip>

RentalTransaction::RentalTransaction()
    : transactionID(""), carID(""), totalCost(0.0), penalty(0.0), status(ACTIVE) {}

RentalTransaction::RentalTransaction(std::string tID, std::string cID, Customer cust, Date start, Date end)
    : transactionID(tID), carID(cID), customer(cust), startDate(start), endDate(end),
      totalCost(0.0), penalty(0.0), status(ACTIVE) {}

std::string RentalTransaction::getStatusString() const {
    switch(status) {
        case ACTIVE: return "Active";
        case COMPLETED: return "Completed";
        case CANCELLED: return "Cancelled";
        default: return "Unknown";
    }
}

void RentalTransaction::calculateTotalCost(double pricePerDay) {
    int days = startDate.daysBetween(endDate);
    if (days == 0) days = 1; // Minimum 1 day rental
    totalCost = days * pricePerDay;
}

void RentalTransaction::calculatePenalty(const Date& returnDate, double pricePerDay) {
    if (returnDate > endDate) {
        int lateDays = endDate.daysBetween(returnDate);
        penalty = lateDays * (pricePerDay * 1.5); // 150% of daily rate for penalty
    } else {
        penalty = 0.0;
    }
    actualReturnDate = returnDate;
}

void RentalTransaction::completeTransaction(const Date& returnDate) {
    actualReturnDate = returnDate;
    status = COMPLETED;
}

std::string RentalTransaction::toString() const {
    std::stringstream ss;
    ss << "Transaction ID: " << transactionID << "\n"
       << "Car ID: " << carID << "\n"
       << "Customer:\n" << customer.toString() << "\n"
       << "Start Date: " << startDate.toString() << "\n"
       << "End Date: " << endDate.toString() << "\n";

    if (status == COMPLETED) {
        ss << "Return Date: " << actualReturnDate.toString() << "\n";
    }

    ss << "Total Cost: Rp " << std::fixed << std::setprecision(0) << totalCost << "\n";

    if (penalty > 0) {
        ss << "Penalty: Rp " << std::fixed << std::setprecision(0) << penalty << "\n";
    }

    ss << "Status: " << getStatusString();
    return ss.str();
}

std::string RentalTransaction::toFileString() const {
    std::stringstream ss;
    ss << transactionID << "|" << carID << "|"
       << customer.getName() << "|" << customer.getNIK() << "|" << customer.getPhone() << "|"
       << startDate.getDay() << "-" << startDate.getMonth() << "-" << startDate.getYear() << "|"
       << endDate.getDay() << "-" << endDate.getMonth() << "-" << endDate.getYear() << "|";

    if (status == COMPLETED) {
        ss << actualReturnDate.getDay() << "-" << actualReturnDate.getMonth() << "-" << actualReturnDate.getYear();
    } else {
        ss << "0-0-0";
    }

    ss << "|" << totalCost << "|" << penalty << "|" << status;
    return ss.str();
}

void RentalTransaction::fromFileString(const std::string& data) {
    std::stringstream ss(data);
    std::string item;
    int fieldCount = 0;

    while (std::getline(ss, item, '|')) {
        switch(fieldCount) {
            case 0: transactionID = item; break;
            case 1: carID = item; break;
            case 2: customer.setName(item); break;
            case 3: customer.setNIK(item); break;
            case 4: customer.setPhone(item); break;
            case 5: {
                // Parse start date
                int d, m, y;
                sscanf(item.c_str(), "%d-%d-%d", &d, &m, &y);
                startDate = Date(d, m, y);
                break;
            }
            case 6: {
                // Parse end date
                int d, m, y;
                sscanf(item.c_str(), "%d-%d-%d", &d, &m, &y);
                endDate = Date(d, m, y);
                break;
            }
            case 7: {
                // Parse return date
                int d, m, y;
                sscanf(item.c_str(), "%d-%d-%d", &d, &m, &y);
                if (d != 0 && m != 0 && y != 0) {
                    actualReturnDate = Date(d, m, y);
                }
                break;
            }
            case 8: totalCost = std::stod(item); break;
            case 9: penalty = std::stod(item); break;
            case 10: status = static_cast<TransactionStatus>(std::stoi(item)); break;
        }
        fieldCount++;
    }
}