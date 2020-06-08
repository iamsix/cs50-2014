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
    
    int quarters = 0;
    int dimes = 0;
    int nickels = 0;

    quarters = owingcents / 25;
    owingcents = owingcents % 25;
    dimes = owingcents / 10;
    owingcents = owingcents % 10;
    nickels = owingcents / 5;
    owingcents = owingcents % 5;
    
    printf("%d quarters, %d dimes, %d, nickels, %d pennies\n%d\n", 
            quarters, 
            dimes, 
            nickels, 
            owingcents, 
            (quarters + dimes + nickels + owingcents));
   
    
}
