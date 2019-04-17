/* 
 * File:   main.cpp
 * Author: remus
 *
 * Created on 2019. április 17., 18:30
 */

#include <cstdlib>
#include <iostream>
//#include <ncurses.h>
//#include <curses.h>
using namespace std;

/*
333333333
111111111    main tömb első szinjének alszintjei
864213579   main tömb első szintje 
222222222
555555555
 */

int** expandBoard(int** array) {
 
    int arraySize = array[0][0];
    int** newArray;
    newArray = new int*[arraySize + 1];
    newArray[0] = new int[1];
    newArray[0][0] = arraySize + 1;
    for (int i = 1; i < array[0][0]+1; i++) {
        //                      subarraySize
        newArray[i] = new int[array[i][0]];
        newArray[i][0] = array[i][0] ;
        for (int j = 1; j < array[i][0]; j++) {
            newArray[i][j] = array[i][j];
        }
    }
    array = newArray;
    delete newArray;
    return array;
}

int main(int argc, char** argv) {
    int** main_array;
    int arraySize = 5;
    int subarraySize = 5;
    main_array = new int*[arraySize];
    //    main_array[0] = arraySize;
    main_array[0] = new int[1];
    main_array[0][0] = arraySize;
    for (int i = 1; i < main_array[0][0]; ++i) {
        main_array[i] = new int[subarraySize];
        main_array[i][0] = subarraySize;
    }
    for (int i = 1; i < main_array[0][0]; i++) {
        for (int j = 1; j < main_array[i][0]; j++) {
            main_array[i][j] = i;
            cout << main_array[i][j];
        }
    }
    expandBoard(main_array);
    for (int i = 1; i < main_array[0][0]; i++) {
        for (int j = 1; j < main_array[i][0]; j++) {
            main_array[i][j] = i;
            cout << main_array[i][j];
        }
    }

    return 0;
}

