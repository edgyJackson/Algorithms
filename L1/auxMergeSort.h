//----------------------------------------------------------
// CS361 Algorithm Lab 1 MergeSort
// @author Reggie Johnson
// @winter '19 01/20/19
// code recycled from CS260 data structures @CCC
//----------------------------------------------------------
#ifndef AUXMERGESORT_H
#define AUXMERGESORT_H


/**
 * @brief Performs merge sort on an array by splitting it into 2 subarrays
 * and then will merge the to sub arrays into one.
 */
void auxMergeSort(int array, int startIndex, int endIndex);


//------------------------IMPLEMENTATION-------------------------------
/**
 *Precondition: array contains two sorted sequences
 *Postcondition: sequences are merged into one
 *@param array : array being sorted
 *
*/
void Merge(int *a, int low, int high, int mid)
{
    // We have low to mid and mid+1 to high already sorted.
    int i, j, k;
    int * temp = new int[high-low+1];
    i = low;
    k = 0;
    j = mid + 1;

    // Merge the two parts into temp[].
    while (i <= mid && j <= high)
    {
        if (a[i] < a[j])
        {
            temp[k] = a[i];
            k++;
            i++;
        }
        else
        {
            temp[k] = a[j];
            k++;
            j++;
        }
    }

    // Insert all the remaining values from i to mid into temp[].
    while (i <= mid)
    {
        temp[k] = a[i];
        k++;
        i++;
    }

    // Insert all the remaining values from j to high into temp[].
    while (j <= high)
    {
        temp[k] = a[j];
        k++;
        j++;
    }


    // Assign sorted data stored in temp[] to a[].
    for (i = low; i <= high; i++)
    {
        a[i] = temp[i-low];
    }
 delete [] temp;

}

// A function to split array into two parts.
void auxMergeSort(int* array, int startIndex, int endIndex) {

    //Sorts an array of Persons of given size
    //allocates n extra storage and calls helper

    int mid;
    if (startIndex < endIndex)
    {
        mid=(startIndex+endIndex)/2;
        // Split the data into two half.
        auxMergeSort(array, startIndex, mid);
        auxMergeSort(array, mid+1, endIndex);

        // Merge them to get sorted output.
        Merge(array, startIndex, endIndex, mid);

    }
}


#endif // AUXMERGESORT_H
