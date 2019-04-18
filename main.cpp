/* 
 * File:   main.cpp
 * Author: remus
 *
 * Created on 2019. április 17., 18:30
 */

#include <cstdlib>
#include <iostream>
#include <ncurses.h>
#include <curses.h>
using namespace std;
int cursor_X = 0;
int cursor_y = 0;

char** initBoard(int row, int col) {
    char** newChar = new char*[row];
    for (int i = 0; i < row; i++) {
        newChar[i] = new char[col];
        for (int j = 0; j < col; j++) {
            newChar[i][j] = '.';
        }
    }
    return newChar;
}

char** expandBoard(int row, int col, char key_press,
        char ** original_board) {
    /* 
     keypress col row bővítés ha fájlszélén
     * 
     */
    char ** newChar = initBoard(row, col);

    return newChar;
}

int main(int argc, char** argv) {
    int row = 5;
    int col = 4;
    char** main_array = initBoard(row, col);
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            cout << main_array[i][j];
        }
        cout << "\n";
    }

    return 0;
}

