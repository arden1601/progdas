#include <iostream>
using namespace std;

int main(int argc, char const *argv[])
{
    // ubah nilai n untuk nilai akhir
    // ubah nilai number untuk nilai awal
    int n = 10;
    int number = 1;
    for (int i = 1; i <= n; i++){
        for (int j = 1; j <= i; j++){
            cout << number << " ";
            number++;
        }
        cout << endl;
        if (number > n) break;
    }
    return 0;
}
