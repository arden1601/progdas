#include <iostream>
using namespace std;

int main() {
    int start, end, res = 0;
    int choice;
    bool validChoice = false;
    
    cout << "Program Penjumlahan Deret Bilangan" << endl;
    cout << "==================================" << endl;
    
    while (!validChoice) {
        cout << "Pilih jenis bilangan 1. Genap atau 2. Ganjil: ";
        cin >> choice;
        
        if (choice == 1 || choice == 2) {
            validChoice = true;
        } else {
            cout << "Input tidak valid! Masukkan 1 untuk Genap atau 2 untuk Ganjil." << endl;
        }
    }
    
    while (true) {
        cout << "Masukkan angka awal: ";
        if (!(cin >> start)) {
            cout << "Input tidak valid! Masukkan angka." << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        
        cout << "Masukkan angka akhir: ";
        if (!(cin >> end)) {
            cout << "Input tidak valid! Masukkan angka." << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        
        if (start <= end) break;
        cout << "Error: Angka awal harus lebih kecil dari angka akhir!" << endl;
    }
    
    cout << "\nBilangan yang ditemukan: ";
    for (int i = start; i <= end; i++) {
        bool genap = (i % 2 == 0);
        if ((choice == 1) && genap) {
            cout << i << " ";
            res += i;
        } else if ((choice == 2) && !genap) {
            cout << i << " ";
            res += i;
        }
    }
    
    cout << "\nJumlah semua bilangan " 
         << ((choice == 1) ? "genap" : "ganjil") 
         << ": " << res << endl;
    
    return 0;
}