#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>

class Customer {
private:
    std::string name;
    std::string nik;
    std::string phone;

public:
    // Constructors
    Customer();
    Customer(std::string n, std::string id, std::string ph);

    // Getters
    std::string getName() const { return name; }
    std::string getNIK() const { return nik; }
    std::string getPhone() const { return phone; }

    // Setters
    void setName(const std::string& n) { name = n; }
    void setNIK(const std::string& id) { nik = id; }
    void setPhone(const std::string& ph) { phone = ph; }

    // Utility methods
    std::string toString() const;
};

#endif