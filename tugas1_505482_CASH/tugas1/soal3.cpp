#include <iostream> 

using namespace std;

int main(int argc, char const *argv[])
{
    int i = 1, j = 2, k = 3, m = 4;
    i += j + k;
    cout << "Nilai i: " << i << endl;

    j *= m +5;
    cout << "Nilai j: " << j << endl;

    k -= m/=j*2;
    cout << "Nilai k: " << k << endl;

    cout << "NILAI AKHIR: " << endl;
    cout << "i = " << i << ", j = " << j << ", k = " << k << ", m = " << m << endl;

    return 0;
}
