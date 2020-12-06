//----------------------------------------------------------
// CS361 Algorithm Lab 1 File Reader
// @author Reggie Johnson
// @winter '19 01/20/19
//
//----------------------------------------------------------
#ifndef READFILETOARRAY_H
#define READFILETOARRAY_H
#include <string>
#include <iostream>
#include <fstream>

/**
 * @brief reads file and store values into int array
 */
void readFileToArray(int* array[], std::string fileToRead, int size);

//------------------------IMPLEMENTATION-------------------------------


void readFileToArray(int* array, std::string fileToRead, int size) {



// open file
std::ifstream inputFile(fileToRead);

// test file open
if (inputFile) {
    int value, i = 1;

//read the elements in the file into an array
//get sum of elements in array
long long int sum = 0;
array[0] = size;
while ( inputFile >> value && i<=size) {
    array[i] = value;
    i++;
    sum +=value;
}



//check array length and sum
std::cout << "number of elements in array : " << size << std::endl;
std::cout << "sum of array : " << sum << std::endl;

// close the file
inputFile.close();


} else {
    std::cout << "error opening file!!!!\n";
    exit(0);
}

}
#endif // READFILETOARRAY_H
