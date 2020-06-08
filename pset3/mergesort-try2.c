#include <stdio.h>
#include <string.h>

    // attempt at a mergesort.....
    
int ops;
void mergeSort(int left[], int right[], int start, int len)
{

    if ((len - start) < 2)
    {
        return;
    }
    //'half' is a funny concept because of int truncation.
    int half = (start + len) / 2;
    
    // sort left half
    mergeSort(left, right, start, half);
    
    // sort right half
    mergeSort(left, right, half, len);
  
    // merge the halves  
    int i = start;
    int lefti = start;
    int righti = half;
    do
    {
        int leftVar = -1, rightVar = -1;
        if (lefti < half)
        {
            leftVar = left[lefti];
        }
        
        if (righti < len)
        {
            rightVar = left[righti];
        }
        
        if((leftVar < rightVar && leftVar >= 0) || rightVar < 0)
        {
            right[i] = leftVar;
            lefti++;
        }
        
        else
        {
            right[i] = rightVar;
            righti++;
        }
        i++;
    } while( lefti < half || righti < len);
    
    
    for(int i = 0; i < len; i++)
    {
        left[i] = right[i];
    }   
    
}


int main(void)
{  
    int values[9] = {2, 8, 6, 1, 0, 7, 4, 5, 3};
    int values2[9] = {2, 8, 6, 1, 0, 7, 4, 5, 3};
    int n = 9;
    ops = 0;
    mergeSort(values, values2, 0, n);
    
    for(int i = 0; i < n; i++)
    {
        printf("%i \n", values[i]);
    }   
    
    printf("%i Ops\n", ops);
}
