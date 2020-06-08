#include <cs50.h>
#include <stdio.h>


char letters[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
int nbletters = sizeof(letters)-1;

int main() {
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
//                printf("%c\n", letters[entry[i]]);
            }
            printf("%s\n", trystring);
            
            for(i=0 ; i<len && ++entry[i] == nbletters; i++)
            {
                entry[i] = 0;
            }
        } while(i<len);
    }
}
