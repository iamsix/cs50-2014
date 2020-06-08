#include <stdio.h>
#include <string.h>
#include <crypt.h>
#include <unistd.h>

#define _X_OPENSOURCE

// a dictionary of about 172,000 words
#define DICTIONARY "cain.txt"

// maximum word size in the dictionary

int main(int argc, char* argv[])
{  
    // check correct usage
    if (argc != 2)
    {
        printf("Usage: ./crack \"password\"\n");
        return 1;    
    }

    // make cla cyphertext and the first two chars the salt
    char* ctext = argv[1];
    char salt[2] = {ctext[0], ctext[1]};

    // open dictionary and error check
    FILE* dict = fopen(DICTIONARY, "r");
    if (dict == NULL)
    {
        printf("Could not open dictionary\n");
        return 2;
    }

    // so long as the file hasn't ended
    while (!feof(dict))
    {
        char key[256];
        // get each word
        fgets(key, sizeof(key), dict);

        // replace newline char with null terminator
        int i = 0;
        while (key[++i] != '\n');
        printf("%i\n65t", key[i-1]);
        key[i] = '\0';

        // compare this encrypted word with the ciphertext
        if (strcmp(crypt(key, salt), ctext) == 0)
        {
            // print decrypted password if found
            printf("%s\n", key);
            return 0;
        }      
    }
    // otherwise, no dice
    printf("Could not crack\n");

    return 0;
    
}
