#import <cs50.h>
#import <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

string vigenere(string text, string key);

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("You must specify a key\n");
        return 1;
    }
    string key = argv[1];
    for (int i = 0, len = strlen(key); i < len; i++)
    {
        if (!(isalpha(key[i])))
        {
            printf("Accepts only alphabetical keys\n");
            return 1;
        }
    }
    string plaintext = GetString();
    string crypted = vigenere(plaintext, key);  
    printf("%s\n", crypted);
}

string vigenere(string text, string key)
{
    int j = 0;
    for (int i = 0, len = strlen(text); i < len; i++)
    {
        if (isalpha(text[i]))
        {
            int k = tolower(key[j]) - 'a';
            if (islower(text[i]))
            {
                text[i] = 'a' + (((text[i] - 'a') + k) % 26);
            }
            else if (isupper(text[i]))
            {
               text[i] = 'A' + (((text[i] - 'A') + k) % 26);
            }
            
            j++;
            j = j % strlen(key);
        }
    }
    return text;
}

