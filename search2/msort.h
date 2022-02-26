#include <vector>

//DELETE
#include <iostream>
using namespace std;
//


/**
 * Performs mergesort on the provided vector using
 * the Compare object, comp, to peform ALL comparisons
 *
 * Must run in O(n*log(n))
 */
template <class T, class Compare >
void mergeSort(std::vector<T>& list, Compare comp );

//helper function used to do the actual recursion
template<class T, class Compare >
void msortHelp(std::vector<T>& list, int start, int end, Compare comp );

//merge used to compare and actually sort the smaller vectors after recursion
template<class T, class Compare >
void merge(std::vector<T>& input, Compare comp, int start, int mid, int end);


//FUNCTION DEFINITIONS 
template <class T, class Compare >
void mergeSort(std::vector<T>& list, Compare comp )
{
    msortHelp(list, 0, list.size()-1, comp);   
}



template<class T, class Compare >
void msortHelp(std::vector<T>& list, int start, int end, Compare comp )
{
    if(start < end)
    {
        int mid = (start + end) / 2; //find the mid point
        //recursive calls
        msortHelp(list, start, mid, comp);
        msortHelp(list, mid+1, end, comp);
        //merge the sorted parts
        merge(list, comp, start, mid, end);
    }
    return;
}

template<class T, class Compare >
void merge(std::vector<T>& input, Compare comp, int start, int mid, int end)
{
    
    //lefti: represents place holder for index in the input ('left') array array
    //righti: represents place holder for index in the input ('right') sorted array
    //outi: represents place holder for index in the output merged array
    int lefti = start, righti = mid + 1, outi = start;
    
    //make the output array
    std::vector<T> output(input.size());

    while (lefti <= mid && righti <= end)
    {
        if ( comp(input[lefti], input[righti]) )
        {
            output[outi++] = input[lefti++];
            // outi++;
            // lefti++;
        }
        else
        {
            output[outi++] = input[righti++];
            // outi++;
            // righti++;
        }
    }
    while (lefti <= mid)
    {
        output[outi++] = input[lefti++];
        // outi++;
        // lefti++;
    }
    while (righti <= end)
    {
        output[outi++] = input[righti++];
        // outi++;
        // righti++;
    }
    //copy sorted elements and their positions from output to input
    for (int i = start; i < outi; i++)
    {
        input[i] = output[i];
    }
}