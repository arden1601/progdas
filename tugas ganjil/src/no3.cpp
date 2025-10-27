#include <iostream>
using namespace std;

int main(int argc, char const *argv[])
{
    // ganti angka kalo mau beda
    int n = 3;
    int deretArr[100];
    int position = 0;
    
    for (int i = -n; i <= n; i++){
        deretArr[position++] = i;
    } 
    for (int i = 0; i < position; i++){
        cout << deretArr[i] << " ";
    }
    return 0;
}
