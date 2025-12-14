#include "Customer.h"
#include <sstream>

Customer::Customer() : name(""), nik(""), phone("") {}

Customer::Customer(std::string n, std::string id, std::string ph)
    : name(n), nik(id), phone(ph) {}

std::string Customer::toString() const {
    std::stringstream ss;
    ss << "Name: " << name << "\n"
       << "NIK: " << nik << "\n"
       << "Phone: " << phone;
    return ss.str();
}