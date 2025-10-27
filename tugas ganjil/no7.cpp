#include <iostream>
#include <string>
#include <sstream>
using namespace std;

bool isUpperCase(char c)
{
    return isupper(static_cast<unsigned char>(c));
}

string buatAkronim(string kalimat, bool upperCase = true)
{
    string akronim = "";
    stringstream ss(kalimat);
    string kata;

    while (ss >> kata)
    {
        if (!kata.empty())
        {
            char hurufPertama = kata[0];
            akronim += toupper(hurufPertama);
            for (size_t i = 1; i < kata.length(); i++)
            {
                if (isUpperCase(kata[i]))
                {
                    akronim += kata[i];
                }
            }
        }
        
    }

    return akronim;
}

// Fungsi untuk menghitung jumlah kata
int hitungKata(string kalimat)
{
    stringstream ss(kalimat);
    string kata;
    int jumlah = 0;

    while (ss >> kata)
    {
        jumlah++;
    }

    return jumlah;
}

int main()
{
    string input;
    int pilihan;

    cout << "\n--- AKRONIM ---" << endl;
    cout << "Masukkan kalimat: ";
    getline(cin, input);

    if (!input.empty())
    {
        string hasil = buatAkronim(input, true);
        int jumlahKata = hitungKata(input);

        cout << "\nKalimat asli\t: " << input << endl;
        cout << "Jumlah kata\t: " << jumlahKata << endl;
        cout << "Akronim\t\t: " << hasil << endl;
        cout << "Panjang Akronim\t: " << hasil.length() << " karakter" << endl;
    }
    else
    {
        cout << "Kalimat tidak boleh kosong!" << endl;
    }

    return 0;
}