//----------------------------------------------------------
// CS361 Algorithm Lab 1 topTenElements
// @author Reggie Johnson
// @winter '19 01/20/19
//
//----------------------------------------------------------
#ifndef TOPTENELEMENTS_H
#define TOPTENELEMENTS_H
#include <stack>
#include <iostream>
/**
 * @brief topTenElements prints a list of the the top ten elements
 * in an array, in descending order
 * @param input
 */
void topTenElements(int* array, int size);










//get max value from array
int getMaximum(int* array, int n) {

    int max = array[0];
    for(int i = 1; i <n;i++)
        if(array[i]>max)
            max=array[i];
    return max;

}




//helper function for recursive calls
//loops 10 times and returns the maximum value in array
//after each iteration and pushes it into the stack

void topTenElementsHelper(int* array, std::stack<int> * stack, int size){

if(stack->size()<11 && size >0){

int max = getMaximum(array, size);

stack->push(max);

    topTenElementsHelper(array, stack, size-1);
}


}




//topTen function sets up the dynamic stack
//runs the helper function on the array
//and then handles the printing of the stack
//and clean up of dynamic stack
void topTenElements(int* array, int size){
    std::stack<int> * mystack = new std::stack<int>;

    topTenElementsHelper(array, mystack, size);

 int stop = 10;
if(size < 10){
stop = size;
}
    for (int i=0;i<stop;i++) {
        std::cout << mystack->top() << "\n";
        mystack->pop();
    }

    delete mystack;


}

#endif // TOPTENELEMENTS_H
