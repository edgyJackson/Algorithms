//----------------------------------------------------------
// CS361 Algorithm Lab 1 BucketSort
// @author Reggie Johnson
// @winter '19 01/20/19
//
//----------------------------------------------------------
#ifndef BUCKETSORT_H
#define BUCKETSORT_H

#include <vector>
#include <algorithm>

//pseudocode from https://en.wikipedia.org/wiki/Bucket_sort
//implementation from sources https://www.geeksforgeeks.org/bucket-sort-2/
//and https://www.thecrazyprogrammer.com/2017/02/bucket-sort-in-c.html


/**
 * @brief bucketSort sort array into bins and then perform sorting algorithm on bins
 * @param arr
 * @param size
 */
void bucketSort(int * arr, int size);



//insertionSort
//code modelled after response from user @SuuperKoko
//http://forums.codeguru.com/showthread.php?359247-Sorting-with-Iterators
//and resource
//http://www2.lawrence.edu/fast/GREGGJ/CMSC270/iterators.html
//created a template typename iter for passing in iterator pointers
template< typename Iter >
void insertionSort( Iter first, Iter last )
{
    if (first==last) return; // returns successfully if the container is empty!


    Iter j=first;


    ++j; // <---- sort first 2 items
    // The current pointer will scan through the part of the array
        // that appears before the mover location.
    for(; j!=last; ++j ) // it is preferable to use prefix operators
    {
        typename std::iterator_traits<Iter>::value_type key = (*j);
        Iter i=j;
        --i;

        //  tested for i==first before decrementing i
        while( (*i)>key )
        {
                {
                Iter k=i;
                ++k;
                *k=*i;
                }
            // testing for equality
            if (i==first) break;
            --i;
        }
        if ((*i)>key)
            (*i)=key;
        else
            {
            ++i;
            (*i)=key;
            }
    }
}


//get max value from array
int getMax(int* array, int n) {

    int max = array[0];
    for(int i = 1; i <n;i++)
        if(array[i]>max)
            max=array[i];
    return max;

}



//to get an even distribution I had to felt like 700k buckets
//created the best time and space balance for size 10,000,000
void bucketSort(int * arr, int size) {
  //variables
  int max, bucket = 1000000, divider, i, j, k;

  //Set up an array of initially empty "buckets", 1000.
  std::vector<int> *B = new std::vector<int>[bucket];

  //find max and min
  max = getMax(arr, size);

  divider = ceil(float(max + 1) / bucket);

  //Go over the original array, putting each object in its bucket, inserting elements into buckets
  for(i = 0; i < size; i++) {
    j = floor( arr[i] / divider );
    B[j].push_back(arr[i]);
  }

  //Sort each non-empty bucket.
  for(i = 0; i < bucket; i++) {
    insertionSort(B[i].begin(), B[i].end());
  }

  //Visit the buckets in order and put all elements back into the original array.
  k = 0;
  for(i = 0; i < bucket; i++) {
    for(j = 0; j < B[i].size(); j++) {
      arr[k++] = B[i][j];
    }
  }


//delete dynamic vector
  delete [] B;
}


#endif // BUCKETSORT_H
