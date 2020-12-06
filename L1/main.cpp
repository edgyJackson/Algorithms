#include <iostream>
#include <fstream>
#include<chrono>
#include "readFileToArray.h"
#include "flgIsSorted.h"
#include "auxMergeSort.h"
#include "auxQuickSort.h"


using namespace std;




int main()
{

//***************Set up array using data file***************
int size;
//loops for timing
int const LOOPS = 1;

//read in number of records to pull
cout << "***********************Reading in array from data file************************\n";
cout << "enter number of records to examine ";
cin >> size;



//--------------timing code for File Reader---------------------
//found on stackOVerFlow answer by @ForceBru
//How to calculate execution time in milliseconds
//auto started = std::chrono::high_resolution_clock::now();



int* labDataArray = new int[size + 1];

//call my file reader function to set up array
readFileToArray(labDataArray,"lab1_Data.txt",size);








//copied another array so that I can run merge and quick sort, should not count towards time.
int* labDataArray2 = new int[size + 1];
readFileToArray(labDataArray2,"lab1_Data.txt",size);


//store the size of the array in the first index
labDataArray[0]=size;
labDataArray2[0]=size;




//check if sorted I know it's not
cout << "\nis array 1 sorted : " << flgIsSorted(labDataArray) << "\n";
cout << "\nis array 2 sorted : " << flgIsSorted(labDataArray2);



//--------------auxMergeSort Function---------------------


cout << "\n\nusing auxMergeSort to sort array : ";
//mergesort array
auxMergeSort(labDataArray, 1, size);
cout << "\n\narray sorted : \n\n\n";












//-------------- auxQuickSort---------------------

cout << "\n\nusing auxQuickSort to sort array : \n\n\n";
//quicksort array
auxQuickSort(labDataArray2, 1, size);
cout << "\n\narray sorted : \n\n\n";



 //--------------timing code for flgIsSorted---------------------
auto started = std::chrono::high_resolution_clock::now();




        //check if array is sorted

        cout << "array is sorted? : " << flgIsSorted(labDataArray);


auto  done = std::chrono::high_resolution_clock::now();
//output algorithm timing results in milliseconds
cout << "\nRUNTIME for flgIsSorted algorithm is " << chrono::duration_cast<chrono::milliseconds>(done-started).count() << " ms\n";

//---------------------------------------------------------


 cout << "array to prove is sorted \n";
 for (int i =1;i<20;i++) {

     cout << "\n" << labDataArray[i];
 }

cout << "\n\n";



//--------------timing code for flgIsSorted---------------------
started = std::chrono::high_resolution_clock::now();


//loops for timing purposes
for(int i=0;i<LOOPS;i++){
//check if array is sorted
bool m = flgIsSorted(labDataArray2);
}
 done = std::chrono::high_resolution_clock::now();
//output algorithm timing results in milliseconds
cout << "\nRUNTIME for flgIsSorted algorithm is " << chrono::duration_cast<chrono::milliseconds>(done-started).count() << " ms\n";

//---------------------------------------------------------

//check if sorted I know it's not
cout << "\nis array 1 sorted : " << flgIsSorted(labDataArray) << "\n";
cout << "\nis array 2 sorted : " << flgIsSorted(labDataArray2) << "\n\n";






delete [] labDataArray;
delete [] labDataArray2;


}



