#ifndef RADIXSORT_H
#define RADIXSORT_H


//https://www.youtube.com/watch?v=vhxpMcDwRPM
//How to implement Radix Sort in C++

//get max value from array
int getMaxValue(int* array, int n) {

    int max = array[0];
    for(int i = 1; i <n;i++)
        if(array[i]>max)
            max=array[i];
    return max;

}


//count sort an array
void countingSort(int* array, int n, int ith ){


//output array for
int * output = new int[n];

//count[] for counting number of elements with the same digit in the "ith" place
int i , count[10] = {0};

//count how many times each digit occured in count[]
for (i=0;i<n;i++)
    count[(array[i]/ith) % 10]++;

//calculate the overall count
for (i=1;i<10;i++)
    count[i] += count[i-1];

//inserting values according to digit '(array[i]/ith)%10' in count[]
for (i=n-1;i>=0;i--) {
    output[count[(array[i]/ith)%10] - 1] = array[i];
    count[(array[i]/ith)%10]--;
}

//Assign result to the main array
for (i=0;i<n;i++)
    array[i] = output[i];

delete [] output;
}

//sort array of size n using dun dun... Radix sort!
void radixSort(int* array, int n ){

int ith, m;
m = getMaxValue(array, n);

//call countingSort() for digit at ith place
for (ith=1;m/ith>0;ith*=10) {
    countingSort(array, n, ith);
}

}





#endif // RADIXSORT_H
