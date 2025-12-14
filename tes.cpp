#include <iostream>
using namespace std;
class SuperClass {
 public: virtual void out() { cout << "p"; }
};
class SubClass : public SuperClass {
 public: virtual void out() { cout << "b"; }
};
int main() {
 int a = 1;
 cout << a/2;
}