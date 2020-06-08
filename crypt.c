#define _XOPEN_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <cs50.h>
#include <crypt.h>

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        return 1;
    }
    
    string pw = "";
    string salt = argv[1][0] + argv[1][1];
    for [i = 2; i < 13; i++]
    {
        pw = pw + argv[1][i];
    }
    printf("%s\n", pw);
    
}
