#include <iostream>
#include <cstring>
using namespace std;

// Makro untuk menentukan mode input
// 1 = Input Eksplisit (tanpa input user)
// 0 = Input dari User
#define INPUT 1

// Deklarasi union dengan 4 member tipe data berbeda
union Mahasiswa {
    int niu;
    char nama[50];
    float ipk;
    bool aktif;
};

int main() {
    Mahasiswa data[5];

#if INPUT
    data[0] = {101};  // Menyimpan niu dari data pertama
    strcpy(data[1].nama, "Dhimas Nur");  // Menyimpan nama dari data kedua
    data[2] = {.ipk = 3.45};  // Menyimpan ipk dari data ketiga
    data[3] = {.aktif = false};  // Menyimpan status aktif dari data keempat
    data[4] = {105};  // Menyimpan niu dari data kelima
#else
    cout << "===== INPUT DATA MAHASISWA =====" << endl;
    int pilihan;
    for (int i = 0; i < 5; i++) {
        cout << "\nData Mahasiswa ke-" << (i + 1) << ":" << endl;
        cout << "Pilih tipe data (1=NIU, 2=Nama, 3=IPK, 4=Aktif): ";
        cin >> pilihan;
        cin.ignore();

        switch(pilihan) {
            case 1:
                cout << "Masukkan NIU: ";
                cin >> data[i].niu;
                break;
            case 2:
                cout << "Masukkan Nama: ";
                cin.getline(data[i].nama, 50);
                break;
            case 3:
                cout << "Masukkan IPK: ";
                cin >> data[i].ipk;
                break;
            case 4:
                cout << "Apakah aktif? (1=Ya, 0=Tidak): ";
                cin >> data[i].aktif;
                break;
        }
    }
#endif

    cout << "\n===== DATA MAHASISWA =====" << endl;
    cout << "-----------------------------------------------" << endl;
    cout << "CATATAN: Union hanya dapat menyimpan satu member aktif" << endl;
    cout << "-----------------------------------------------" << endl;
    for (int i = 0; i < 5; i++) {
        cout << "Data ke-" << (i + 1) << ":" << endl;
        if (i == 0) {
            cout << "  NIU: " << data[i].niu << endl;
        } else if (i == 1) {
            cout << "  Nama: " << data[i].nama << endl;
        } else if (i == 2) {
            cout << "  IPK: " << data[i].ipk << endl;
        } else if (i == 3) {
            cout << "  Aktif: " << (data[i].aktif ? "Ya" : "Tidak") << endl;
        } else if (i == 4) {
            cout << "  NIU: " << data[i].niu << endl;
        }
        cout << "-----------------------------------------------" << endl;
    }

    cout << "\n===== UKURAN UNION DAN MEMBER =====" << endl;
    cout << "Ukuran union Mahasiswa  : " << sizeof(Mahasiswa) << " bytes" << endl;
    cout << "Ukuran member niu       : " << sizeof(int) << " bytes" << endl;
    cout << "Ukuran member nama      : " << sizeof(char[50]) << " bytes" << endl;
    cout << "Ukuran member ipk       : " << sizeof(float) << " bytes" << endl;
    cout << "Ukuran member aktif     : " << sizeof(bool) << " bytes" << endl;

    return 0;
}
