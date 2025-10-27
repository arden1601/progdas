#include <iostream>
using namespace std;

int main() {
    int tinggi;
    
    cout << "Masukkan tinggi segitiga: ";
    cin >> tinggi;
    cout << endl;
    int lebar = 2 * tinggi - 1;
    char segitiga[tinggi][lebar];
    
    for (int i = 0; i < tinggi; i++) {
        for (int j = 0; j < lebar; j++) {
            segitiga[i][j] = ' ';
        }
    }
    
    for (int i = 0; i < tinggi; i++) {
        int kiri = tinggi - 1 - i;
        int kanan = tinggi - 1 + i;
        
        for (int j = kiri; j <= kanan; j++) {
            segitiga[i][j] = '*';
        }
    }
    
    for (int i = 0; i < tinggi; i++) {
        for (int j = 0; j < lebar; j++) {
            cout << segitiga[i][j];
        }
        cout << endl;
    }
    
    return 0;
}