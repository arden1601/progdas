#include <iostream>
#include <cmath>
using namespace std;

// Function prototypes (signatures)
double distance(double x1, double y1, double x2, double y2);
double radius(double centerX, double centerY, double pointX, double pointY);
double circumference(double r);
double area(double r);

int main() {
    double centerX, centerY, pointX, pointY;
    
    // User input for center coordinates
    cout << "Enter the center coordinates (x, y): ";
    cin >> centerX >> centerY;
    
    // User input for point on circle coordinates
    cout << "Enter a point on the circle (x, y): ";
    cin >> pointX >> pointY;
    
    // Calculate radius
    double r = radius(centerX, centerY, pointX, pointY);
    
    double diameter = 2 * r;
    double circ = circumference(r);
    double a = area(r);
    
    cout << "\nCircle Properties:" << endl;
    cout << "Radius: " << r << endl;
    cout << "Diameter: " << diameter << endl;
    cout << "Circumference: " << circ << endl;
    cout << "Area: " << a << endl;
    
    return 0;
}

// Function to calculate distance between two points
double distance(double x1, double y1, double x2, double y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

// Function to calculate radius of circle
double radius(double centerX, double centerY, double pointX, double pointY) {
    return distance(centerX, centerY, pointX, pointY);
}

// Function to calculate circumference of circle
double circumference(double r) {
    const double PI = 3.1416;
    return 2 * PI * r;
}

// Function to calculate area of circle
double area(double r) {
    const double PI = 3.1416;
    return PI * r * r;
}