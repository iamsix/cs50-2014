#import <cs50.h>
#import <stdio.h>

int main(void)
{
    printf("card number: ");
    long long int ccnum = GetLongLong();
    string cardtype = "";
    int i = 1, evens = 0, odds = 0, prevdigit = 0;
    do
    {
        int digit = ccnum % 10;
        
        // if first digit of the card number
        if ((ccnum / 10) == 0)
        {  
            switch (digit)
            {
                case 4:
                    cardtype = "VISA";
                    break;
                case 5:
                    if (prevdigit >= 1 && prevdigit <= 5)
                    {
                        cardtype = "MASTERCARD";
                        break;
                    }
                case 3:
                    if (prevdigit == 4 || prevdigit == 7)
                    {
                        cardtype = "AMEX";
                        break;
                    }
                default:
                    cardtype = "INVALID";
                    break;
            }
                    
                    
        }
        prevdigit = digit;
 
        if ((i % 2) == 0)
        {
            digit = digit * 2;
            do
            {
                int tmpdigit = digit % 10;
                evens = evens + tmpdigit;
                digit = digit / 10;
            } while (digit != 0);
            
        } 
        else 
        {
            odds = odds + digit;
        }
 
        ccnum = ccnum / 10;
        
        i++;
        
    } while (ccnum != 0);
    
    if ((odds + evens) % 10 != 0)
    {
        cardtype = "INVALID";
    }
    
    printf("%s\n", cardtype);
    
    return 0;

}

