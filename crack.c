#define _XOPEN_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>
#include <crypt.h>
#include <string.h>

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        return 1;
    }
    
    string pw = argv[1];
    char salt[3] = {argv[1][0], argv[1][1], 0};
 
    
    FILE* file = fopen("cain.txt", "r"); 
    
    if(file == NULL)
    {
        printf("nope.. file b0rk");
    }
    
    char line[256];
    string cleartext = "";
    while (fgets(line, sizeof(line), file)) {
        int len = strlen(line);
        line[len - 1] = '\0';
        
        string hash = crypt(line, salt);
        
        if(strncmp(hash, pw, 13) == 0)
        {
            cleartext = line;
            printf("PASSWORD IS: %s\n", cleartext);
            return 0;
        }
    }
    
    char letters[] = "0123456789AmakeBCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz./";
    int nbletters = sizeof(letters)-1;
    char trystring[9] = {0,0,0,0,0,0,0,0,0};
    for(int len = 1; len < 8; len++)
    {
        int i, entry[len];
        for(i=0 ; i<len ; i++)
        {
            entry[i] = 0;
        }
        do {
            for(i=0 ; i<len ; i++)
            {
                trystring[i] = letters[entry[i]];
            }
            
            string hash = crypt(trystring, salt);

            if(strncmp(hash, pw, 13) == 0)
            {
                cleartext = trystring;
                printf("%s", hash);
                printf("PASSWORD IS: %s\n", cleartext);
                return 0;
            }
                
            for(i=0 ; i<len && ++entry[i] == nbletters; i++)
            {
                entry[i] = 0;
            }
        } while(i<len);
    }
    
}
