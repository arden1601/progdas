#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

// hapus spasi & lower string
void normalizeString(char *str, char *result)
{
    int j = 0;
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (!isspace(str[i]))
        {
            result[j] = tolower(str[i]);
            j++;
        }
    }
    result[j] = '\0';
}

// Fungsi untuk mengecek palindrome
bool isPalindrome(char *str)
{
    int length = strlen(str);
    int start = 0;
    int end = length - 1;

    while (start < end)
    {
        if (str[start] != str[end])
        {
            return false;
        }
        start++;
        end--;
    }
    return true;
}

int main()
{
    char input[100];
    char normalized[100];
    int pilihan;

    printf("\n--- CEK PALINDROME ---\n");
    printf("Masukkan string: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0'; // Hapus newline

    // Normalisasi string (hapus spasi, lowercase)
    normalizeString(input, normalized);

    printf("\nString asli    : \"%s\"\n", input);
    printf("String normal  : \"%s\"\n", normalized);
    printf("Panjang        : %lu karakter\n", strlen(normalized));

    if (isPalindrome(normalized))
    {
        printf("\nPALINDROME - String ini adalah palindrome!\n");
    }
    else
    {
        printf("\nBUKAN PALINDROME - String ini bukan palindrome.\n");
    }

    return 0;
}