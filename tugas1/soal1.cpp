// Program mencetak deret bilangan genap dan ganjil

#include <iostream>

using namespace std;

int main(int argc, char const *argv[])
{
    int n;
    cout << "Masukkan nilai digit deret: ";
    cin >> n;

    cout << "Ganjil: ";
    int temp = 0;
    for (int i = 0; temp < n; i++)
    {
        if (i % 2 == 1)
        {
            cout << i << " ";
            temp++;
        }
    }
    temp = 0;
    cout << "\nGenap: ";
    for (int i = 0; temp < n; i++)
    {
        if (i % 2 == 0)
        {
            cout << i << " ";
            temp++;
        }
    }

    return 0;
}
