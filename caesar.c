#import <cs50.h>
#import <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

string caesar(string plaintext, int k);

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("You must specify a rotation value\n");
        return 1;
    }
    
    int k = atoi(argv[1]);
    //printf("String to perform caesar rotation on:\n");
    string plaintext = GetString();
    string crypted = caesar(plaintext, k);  
    printf("%s\n", crypted);
}

string caesar(string text, int k)
{
    for (int i = 0, len = strlen(text); i < len; i++)
    {
        if (islower(text[i]))
        {
            text[i] = 'a' + (((text[i] - 'a') + k) % 26);
        }
        else if (isupper(text[i]))
        {
           text[i] = 'A' + (((text[i] - 'A') + k) % 26);
        }
    }
    return text;
}

