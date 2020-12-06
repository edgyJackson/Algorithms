//----------------------------------------------------------
// CS361 Algorithm Lab 1 QuickSort
// @author Reggie Johnson
// @winter '19 01/20/19
// code recycled from CS260 data structures @CCC
//----------------------------------------------------------
#ifndef AUXQUICKSORT_H
#define AUXQUICKSORT_H
#include <algorithm>

/**
 * @brief Recursively performs quick sort on an array
 */
void auxQuickSort(int array, int startIndex, int endIndex);

/**
 * @brief partitions the array into 2 subarrays and returns the pivot
 */
int partition (int* arr, int low, int high);





//------------------------IMPLEMENTATION-------------------------------

/** This function takes last element as pivot, places
 * the pivot element at its correct position in sortedarray,
 * and places all elements (smaller than pivot)to left of pivot
 * and all greater elements to right of pivot and returns the
 * partition index to the calling function
*/
int partition (int arr[], int low, int high)
{
    int pivot = arr[high];    //took the last element as pivot
    int i = (low - 1);
    for (int j = low; j <= high- 1; j++)
    {
        // If current element is smaller than or
        // equal to pivot
        if (arr[j] <= pivot)
        {
            i++;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return (i + 1);
}


/**
 * @brief auxQuickSort takes an array and recursively sorts each half of each sub array
 * until all smaller items are on the left and bigger items are on the right
 * @param array
 * @param startIndex
 * @param endIndex
 */
void auxQuickSort(int* array, int startIndex, int endIndex)
{
    if (startIndex < endIndex)
    {
        int pi = partition(array, startIndex, endIndex);
        auxQuickSort(array, startIndex, pi - 1);
        auxQuickSort(array, pi + 1, endIndex);
    }
}
#endif // AUXQUICKSORT_H
