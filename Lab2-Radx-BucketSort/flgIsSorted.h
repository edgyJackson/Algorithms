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
bool flgIsSorted(int* array, int size);

/**
 * @brief arraySortedOrNah helper function to check if sub array is sorted.
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

int start =size - 10000;
int stackOverFlowLimit = size-1;



//------------------------------------------------------
//It all starts here
//while the array is bigger than 10000 than we will sort the array
//10,000 elements at a time until we get to a number lower than 10000
//this is a limit set so that large arrays don't cause a stackOverFlow
//------------------------------------------------------
    while( stackOverFlowLimit > 10000 ) {

        if(arraySortedOrNah(array, start, stackOverFlowLimit)){

        //decrease start and stackOverFLow relative to eachother
        start-=10000;

        //if stackOverFlow goes below the threshhold then we can run isSortedArray on the rest
        //using stackOverFlow as the size
        if(stackOverFlowLimit-10000 > 0) {
        stackOverFlowLimit-=10000;
        }


        } else {

        return false;
        }


    }



//-----------------------------------------------------------
//Now that the array up until now is sorted, and there
//are guaranteed less than 10,000 elements left, we can
//run the isSortedArray function on the remaining elements
//------------------------------------------------------------
       start = 1;

       if(size > 1) {
           return array[stackOverFlowLimit - 1] <= array[stackOverFlowLimit] && isSortedArray(array, stackOverFlowLimit);

       }

       //if we get to one item left return true
       return true;
}







bool flgIsSorted(int* array, int size) {

    //good thing we stored size in array[0]
   return isSortedArray(array, size);

}










bool arraySortedOrNah(int* array, int startIndex, int stackOverFlowLimit) {

  //Keep increasing start until it reaches the over flow limit
  //then we send the info back to the caller function so it can
  //search a new part of the array after releasing the used
  //memory
  if(startIndex==stackOverFlowLimit){
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
