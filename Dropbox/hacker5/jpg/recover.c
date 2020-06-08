/**
 * recover.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Recovers JPEGs from a forensic image.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char* argv[])
{
    // TODO
    
    FILE* card = fopen("card.raw", "r");
    if (card == NULL)
    {
        printf("Could not open card.raw");
        return 2;
    }
    
    uint8_t* block;
    block = (uint8_t*) malloc(512);
    
    int filename = 0;
    FILE* outpic = NULL;
    while(fread(block, 512, 1, card) != 0)
    {
              
        if(block[0] == 0xff && 
          block[1] == 0xd8 &&
          block[2] == 0xff && 
          (block[3] == 0xe0 || block[3] == 0xe1))
        {
            if(outpic != NULL)
            {
                fclose(outpic);
            }
            char fname[7];
            sprintf(fname, "%03i.jpg", filename);
            outpic = fopen(fname, "w");
            filename++;
        }
        if(outpic != NULL)
        {
            fwrite(block, 512, 1, outpic);
        }
        
    }
    
    free(block);
    fclose(card);
    fclose(outpic);
    
}
