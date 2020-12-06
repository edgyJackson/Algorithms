#include <iostream>
#include "readFileToArray.h"
#include "flgIsSorted.h"
#include "radixSort.h"
#include "bucketSort.h"
#include "topTenElements.h"
#include <algorithm>
#include<chrono>

using namespace std;

int main()
{

 bool userInput = 1;
 while(userInput) {
////------------------------while loop for exiting program---------------------
//loops for timing
int const LOOPS = 1;




//***************Set up array using data file***************
int size;
//loops for timing
//int const LOOPS = 1;

//read in number of records to pull
cout << "***********************Reading in array from data file************************\n";
cout << "enter number of records to examine ";
cin >> size;
cout << "creating 2 arrays \n";

int* labDataArray = new int[size];

//call my file reader function to set up array
readFileToArray(labDataArray,"lab3_data.txt",size);


//copied another array so that I can run bucket and radix sort, should not count towards time.
int* labDataArray2 = new int[size];
readFileToArray(labDataArray2,"lab3_data.txt",size);


//check if sorted I know it's not
cout << "\nis array 1 sorted : " << flgIsSorted(labDataArray, size) << "\n";
cout << "\nis array 2 sorted : " << flgIsSorted(labDataArray2, size) << "\n";




//-----------------------------print array before sort--------------------
cout << "\n\narray 1 before sort : \n";
printArray(labDataArray, 10);
cout << "\n\narray 2 before sort : \n";
printArray(labDataArray2, 10);





//--------------timing code---------------------
auto started = std::chrono::high_resolution_clock::now();


//loops for timing purposes
for(int i=0;i<LOOPS;i++){


//--------------Radix Sort Function---------------------


//radix sort array
cout << "\n\nusing Radix Sort to sort array 1: \n";
radixSort(labDataArray, size);



}
auto done = std::chrono::high_resolution_clock::now();
//output algorithm timing results in milliseconds
cout << "\nRUNTIME for Radix algorithm is " << chrono::duration_cast<chrono::milliseconds>(done-started).count() << " ms\n";

 //---------------------------------------------------------



//check if sorted I know it's not
cout << "\nis array 1 sorted : " << flgIsSorted(labDataArray, size) << "\n";
cout << "\nis array 2 sorted : " << flgIsSorted(labDataArray2, size) << "\n";




//--------------timing code---------------------
started = std::chrono::high_resolution_clock::now();


//loops for timing purposes
for(int i=0;i<LOOPS;i++){


//-------------- Bucket Sort Function---------------------

//bucket sort array
cout << "\n\nusing Bucket Sort to sort array 2: \n";
bucketSort(labDataArray2, size);


}
done = std::chrono::high_resolution_clock::now();
//output algorithm timing results in milliseconds
cout << "\nRUNTIME for Bucket algorithm is " << chrono::duration_cast<chrono::milliseconds>(done-started).count() << " ms\n";

 //---------------------------------------------------------





//check if sorted I know it's not
cout << "\nis array 1 sorted : " << flgIsSorted(labDataArray, size) << "\n";
cout << "\nis array 2 sorted : " << flgIsSorted(labDataArray2, size) << "\n\n";





//-----------------------------print array after sort--------------------

cout << "array 1 after sort : \n";
printArray(labDataArray, 10);
cout << "array 2 after sort : \n";
printArray(labDataArray2, 10);








//--------------timing code---------------------
started = std::chrono::high_resolution_clock::now();


//loops for timing purposes
for(int i=0;i<LOOPS;i++){

//-------------- topTen Function---------------------

//topTenFunction
cout << "\n\nusing topTen function to list top 10 valules in array 1: \n";
topTenElements(labDataArray, size);


}
done = std::chrono::high_resolution_clock::now();
//output algorithm timing results in milliseconds
cout << "\nRUNTIME for topTen algorithm is " << chrono::duration_cast<chrono::milliseconds>(done-started).count() << " ms\n";

//---------------------------------------------------------
















delete [] labDataArray;
delete [] labDataArray2;





////------------------------while loop for exiting program---------------------

cout << "continue? 1 or 0 \n";
cin >> userInput;
 }
}
