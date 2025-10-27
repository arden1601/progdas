#include <iostream>
using namespace std;

int main(int argc, char const *argv[])
{
    int deretArr[100];
    int position = 0;

    for (int i = 3; i < 24; i++){
        if(i % 2 == 1) deretArr[position++] = i;
    }
    for (int i = 0; i < position; i++){
        cout << deretArr[i] << " ";
    }
    return 0;
}
