//----------------------------------------------------------
// CS361 Algorithm Lab 1 flgIsSorted
// @author Reggie Johnson
// @winter '19 01/20/19
//
//----------------------------------------------------------
#ifndef FLGISSORTED_H
#define FLGISSORTED_H

/**
 * @brief recursive check if a given array (provided as a parameter) is sorted in increasing order
 * @param array
 * @return bool
 */
bool flgIsSorted(int* array);

/**
 * @brief arraySortedOrNah helper function to check larger arrays piece by piece.
 * @param array
 * @param startIndex
 * @param endIndex
 * @return bool
 */
bool arraySortedOrNah(int* array, int startIndex, int endIndex);

/**
 * @brief isSortedArray helper function to check if array is sorted.
 * @param ar
 * @param size
 * @return bool
 */
bool isSortedArray (int* ar, int size);
//------------------------IMPLEMENTATION-------------------------------

bool isSortedArray (int* array, int size)
{
    //this is a limit set so that large arrays don't cause a stackOverFlow
    if( size > 1 && size < 10000)
    {
        return array[size - 1] <= array[size] && isSortedArray(array, size - 1);

    }


//If the array is bigger than 10000 than we will sort the array
//10,000 elements at a time until we get to a lower number

int start =1, stackOverFlowLimit = 10000;
    while( size - start > 10000 ) {

        if(arraySortedOrNah(array, start, stackOverFlowLimit)){

        //increase start and stackOverFLow relative to eachother
        start+=10000;
        stackOverFlowLimit+=10000;
        } else {


        return false;
        }
    }

return true;

}

bool flgIsSorted(int* array) {

    //only going to check half the array
   return isSortedArray(array, array[0]/2);

}

bool arraySortedOrNah(int* array, int startIndex, int stackOverFlowLimit) {

  //Keep increasing start until it reaches the over flow limit
  //then we send the info back to the caller function so it can
  //search a new part of the array after releasing the used
  //memory
  if(startIndex>stackOverFlowLimit-1){
  return true;

  }
    //return false if unsorted pair found
    if(array[startIndex] > array[startIndex+1])
        return false;

    //oh, the last pair was actually sorted
    //lets check the next pair
    return arraySortedOrNah(array, startIndex +1, stackOverFlowLimit);
}
#endif // FLGISSORTED_H
