#include "Date.h"
#include <sstream>
#include <iomanip>
#include <cmath>

Date::Date() : day(1), month(1), year(2024) {}

Date::Date(int d, int m, int y) : day(d), month(m), year(y) {}

std::string Date::toString() const {
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << day << "/"
       << std::setfill('0') << std::setw(2) << month << "/"
       << year;
    return ss.str();
}

bool Date::isValid() const {
    if (year < 1900 || year > 2100) return false;
    if (month < 1 || month > 12) return false;

    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // Check for leap year
    if (month == 2 && ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))) {
        daysInMonth[1] = 29;
    }

    if (day < 1 || day > daysInMonth[month - 1]) return false;

    return true;
}

int Date::daysBetween(const Date& other) const {
    // Simple day calculation (approximation)
    int days1 = year * 365 + month * 30 + day;
    int days2 = other.year * 365 + other.month * 30 + other.day;

    return std::abs(days2 - days1);
}

bool Date::isAfter(const Date& other) const {
    return *this > other;
}

bool Date::isBefore(const Date& other) const {
    return *this < other;
}

bool Date::operator<(const Date& other) const {
    if (year != other.year) return year < other.year;
    if (month != other.month) return month < other.month;
    return day < other.day;
}

bool Date::operator>(const Date& other) const {
    if (year != other.year) return year > other.year;
    if (month != other.month) return month > other.month;
    return day > other.day;
}

bool Date::operator==(const Date& other) const {
    return year == other.year && month == other.month && day == other.day;
}

bool Date::operator<=(const Date& other) const {
    return *this < other || *this == other;
}

bool Date::operator>=(const Date& other) const {
    return *this > other || *this == other;
}