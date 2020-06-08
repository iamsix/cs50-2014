#include <stdio.h>
#include <string.h>

    // attempt at a mergesort.....
    // each item in value is a single sorted item
    // pick 2, sort, throw in arrayX
    // pick 2 from arrayX sort, throw in arrayY?
    // pick 2 from arrayY and thow[x,y] in newly emptied arrayX 
    // (can I make an array hold an array like I can in python?)
    // are there lists....
    // ...multi dimensional array?
    // ArrayX = [n/2][itemsperarray]
    // itempsperarray = (starting at 1) itemsperarray * 2
    // marge arrayx[0][x] with arrayx[1][x] in to a single array, store in arrayY[0][single array goes here]
    
    //this probably only works if n are even numbered... I'm not sure what to do about that right now
    
    
    //I need to start with arrayY filled with [0][0] = values[0]
    
 //this probably only works if n are even numbered... I'm not sure what to do about that right now
    


// works perfectly.... but only on lists that are powers of 2.


int main(void)
{   
    int values[16] = {8, 7, 6, 5};
    int n = 4;
    int originalN = n;
    int ops = 0;
    int arrayY[n][n];
    for (int i = 0; i < n; i++)
    {
        arrayY[i][0] = values[i];
    }
    
 //   int leftlen = 0, rightlen = 0;
    
    int nPerArray = 1;
    do
    {
        int arrayX[n / 2][nPerArray * 2];
        int xPos = 0;
        for(int i = 0; i < (n); i++)
        {
            // tempArray actually has the final string in it at the end.
            // but I don't make use of that.
            int tempArray[nPerArray * 2 + 1];

            int tAPos = 0;
            int lefti = 0, righti = 0;
            // how do I account for uneven left/right sides....
            // the nPerArray method doesn't quite work...
            do
            {
                int Left = -1, Right = -1;
                if (lefti < nPerArray)
                {
                    Left = arrayY[i][lefti];
                }
                
                if (righti < nPerArray)
                {
                    Right = arrayY[i + 1][righti];
                }
                
//                if (Left == -1 && Right == -1)
//                {
//                    lefti++;
//                    righti++;
//                }
                
                if((Left < Right && Left >= 0) || Right < 0)
                {
                    tempArray[tAPos] = Left;
                    lefti++;
                }
                
                else
                {
                    tempArray[tAPos] = Right;
                    righti++;
                }
                tAPos++;
                ops++;
            } while( lefti < nPerArray || righti < nPerArray);
            // copy the temporary aray in to arrayX
            for(int j = 0; j < nPerArray*2 + 1; j++)
            {
                arrayX[xPos][j] = tempArray[j];
                printf("%i ", tempArray[j]);
            }
            printf("\nfinal%i\n\n", i);
            xPos++;
            //I think I need to step by 2 here
            i++;
            //if I step differently here can I adjust this to merge differently?
        }
        nPerArray = nPerArray * 2;
        n = n / 2;
        //array X is now half the number of 1st arrays, with each 2nd array being twice as big as arrayY's 2nd array
        // we now want to use arrayY as our starting point
        // so we have to turn arrayY in to array X.
        // arrayY = arrayX; - oh how I wish.
        for(int i = 0; i < n; i++)
        {
            for (int j = 0; j < nPerArray; j++)
            {
                arrayY[i][j] = arrayX[i][j];
            }
        }

    
    } while (n / 2 > 0);
    
    
    for (int i = 0; i < originalN; i++)
    {
        printf("%i\n", arrayY[0][i]);
    }
    
    printf("%i ops", ops);
    
}
