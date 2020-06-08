#include <cs50.h>
#include <stdio.h>
#include <math.h>

int main(void)
{
    float owing;
    do
    {
        printf("How much change should be returned?: ");
        owing = GetFloat();
    } while (owing < 0);
    
    int owingcents = round(owing * 100.0);
    
    int coins = 0;
    
    coins = owingcents / 25;
    owingcents = owingcents % 25;
    coins = coins + (owingcents / 10);
    owingcents = owingcents % 10;
    coins = coins + owingcents / 5;
    owingcents = owingcents % 5;
    coins = coins + owingcents;
    
    printf("%d\n", coins);
   
    
}
