#include <iostream>
using namespace std;

// Makro untuk menentukan mode input
// 1 = Input Eksplisit (tanpa input user)
// 0 = Input dari User
#define INPUT 1

struct Mahasiswa {
    int niu;
    char nama[50];
    float ipk;
    bool aktif;
};

int main() {
    Mahasiswa data[5];

#if INPUT
    data[0] = {101, "Arden Hermawan", 3.75, true};
    data[1] = {102, "Dhimas Nur", 3.92, true};
    data[2] = {103, "Budi Santoso", 3.45, false};
    data[3] = {104, "Dewi Lestari", 3.88, true};
    data[4] = {105, "Eko Prasetyo", 3.60, true};
#else
    cout << "===== INPUT DATA MAHASISWA =====" << endl;
    for (int i = 0; i < 5; i++) {
        cout << "\nData Mahasiswa ke-" << (i + 1) << ":" << endl;

        cout << "Masukkan NIU: ";
        cin >> data[i].niu;
        cin.ignore(); // Membersihkan newline buffer

        cout << "Masukkan Nama: ";
        cin.getline(data[i].nama, 50);

        cout << "Masukkan IPK: ";
        cin >> data[i].ipk;

        cout << "Apakah aktif? (1=Ya, 0=Tidak): ";
        cin >> data[i].aktif;
    }
#endif

    cout << "\n===== DATA MAHASISWA =====" << endl;
    cout << "-----------------------------------------------" << endl;
    for (int i = 0; i < 5; i++) {
        cout << "Data ke-" << (i + 1) << ":" << endl;
        cout << "  NIM    : " << data[i].niu << endl;
        cout << "  Nama   : " << data[i].nama << endl;
        cout << "  IPK    : " << data[i].ipk << endl;
        cout << "  Aktif  : " << (data[i].aktif ? "Ya" : "Tidak") << endl;
        cout << "-----------------------------------------------" << endl;
    }

    cout << "\n===== UKURAN STRUCT DAN MEMBER =====" << endl;
    cout << "Ukuran struct Mahasiswa : " << sizeof(Mahasiswa) << " bytes" << endl;
    cout << "Ukuran member niu       : " << sizeof(int) << " bytes" << endl;
    cout << "Ukuran member nama      : " << sizeof(char[50]) << " bytes" << endl;
    cout << "Ukuran member ipk       : " << sizeof(float) << " bytes" << endl;
    cout << "Ukuran member aktif     : " << sizeof(bool) << " bytes" << endl;

    return 0;
}
