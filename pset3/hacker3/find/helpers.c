/**
 * helpers.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Helper functions for Problem Set 3.
 */
       
#include <cs50.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
 
int binSearch(int array[], int value, int start, int end)
{
   if(end < start)
   {
        // we've crossed our search boundries
        // which means it doesn't exist
        return -1;
   }
   else
   {
        int middle = (start + end) / 2;
        if(array[middle] > value)
        {
            // throw away the right half
            return binSearch(array, value, start, middle - 1);
            // middle-1 too throw away that item that was just checked
        }
        else if(array[middle] < value)
        {
            // throw away the left half
            return binSearch(array, value, middle + 1, end);
            // middle+1 to throw away the checked item
        }
        else
        {
            // only way it's not greater or less
            // is if it's equal so:
            return middle;
        }
   }
}
 
 
bool search(int value, int array[], int n)
{

/*    // TODO: implement a searching algorithm*/
    if(value < 0)
    {
        return false;
    }
    if(binSearch(array, value, 0, n) >= 0)
    {
        return true;
    }

    return false;
}


void mergeSort(int left[], int right[], int start, int len)
{

    if ((len - start) < 2)
    {
        return;
    }
    
    // 'half' is a funny concept because of int truncation.
    // but we account for that with the 'len'gth because
    // if 'half' of 5 is 2 then lefthalf = [0-2](len 2) righthalf = [2-5](len 3)
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

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // TODO: implement an O(n^2) sorting algorithm
    // attempt at a mergesort.....
    // in order to merge sort we need both arrays to be identical copies
    int temp[n];
    for (int i = 0; i < n; i++)
    {
        temp[i] = values[i];
    }
    
    mergeSort(values, temp, 0, n);

    
    return;
}
