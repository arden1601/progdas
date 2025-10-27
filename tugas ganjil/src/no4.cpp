#include <iostream>
using namespace std;

int main() {
    float g = 9.8;  // percepatan gravitasi (m/s^2)
    
    // Input dari pengguna
    double v0;      // kecepatan awal (m/s)
    double h0;      // tinggi awal (m)
    double dt;      // interval waktu (detik)
    
    cout << "Masukkan kecepatan awal (m/s): ";
    cin >> v0;
    cout << "Masukkan tinggi awal (m): ";
    cin >> h0;
    cout << "Masukkan interval waktu (detik): ";
    cin >> dt;
    cout << endl;
    
    // Header tabel
    cout << "\t" << "Iterasi" 
         << "\t\t" << "Waktu(s)" 
         << "\t" << "Posisi (m)" << endl;
    cout << string(50, '-') << endl;
    
    // Variabel untuk simulasi
    int iterasi = 0;
    float t = 0.0;

    // Loop simulasi sampai benda menyentuh tanah
    for(float posisi = h0; posisi >= 0; iterasi++, t += dt) {
        // Hitung posisi menggunakan rumus: h(t) = h0 + v0*t - 0.5*g*t^2
        posisi = h0 + v0 * t - 0.5 * g * t * t;
        
        // Pastikan posisi tidak negatif (benda sudah di tanah)
        if (posisi < 0) {
            posisi = 0;
        }
        
        // Cetak hasil
        cout << "\t" << iterasi 
             << "\t\t" << t 
             << "\t\t"  << posisi << endl;
        
        
        // Break jika sudah menyentuh tanah
        if (posisi == 0) {
            break;
        }
    }
    
    return 0;
}