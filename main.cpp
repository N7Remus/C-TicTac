
#include "pch.h"
#include <iostream>
#include <iostream>
#include <windows.h>
#include<conio.h>
#include <tuple>
#include <string>


/*Billentyű definiciók*/
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_ENTER 13
#define KEY_ESC 27

/*Játékos változó*/
const char Players[] = { 'O','X' };

/*Színkezelő változó*/
const HANDLE  hConsole = GetStdHandle(STD_OUTPUT_HANDLE);


using namespace std;
int cursor_X = 1;
int cursor_Y = 1;

char** initBoard(int row, int col) {
	char** newCharArray = new char*[row];
	for (int i = 0; i < row; i++) {
		newCharArray[i] = new char[col];
		for (int j = 0; j < col; j++) {
			newCharArray[i][j] = '.';
		}
	}
	return newCharArray;
}

char** cloneArrayValues(int row, int col, int row_extra, int col_extra, char** original_board) {


	/* Lemásolja a orignal array értékeit */
	char** newCharArray = initBoard(row, col);
	if (row_extra < 0) {
		row--; 
		row_extra = 0;
	}
	else 
		row--;
	
	if (col_extra < 0) {
		col--;
		col_extra = 0;
	}
	else
		col--;
	for (int i = row_extra; i < row; i++) {
		for (int j = col_extra; j < col; j++) {
			newCharArray[i][j] = original_board[i][j];
		}
	}
	delete[] original_board;
	return newCharArray;
}

int main(int argc, char** argv) {
	int row = 3;
	int col = 3;
	int vision = 4;
	char** main_array = initBoard(row, col);


	while (true)
	{
		system("cls");
		cout << endl << cursor_X << ":" << cursor_Y << endl;
		cout << endl << "row : " << row << ":" << " col : " << col << endl;

		for (int i = cursor_Y + vision; i > cursor_Y - vision-1; i--) {
			for (int j = cursor_X - vision; j < cursor_X + vision + 1; j++) {
				if (i < 0 || j < 0 || row <= i || col <= j) {
					cout << ":";
				}
				else {
					cout << main_array[i][j];
				}
			}
			cout << "\n";
		}
		switch (_getch()) {
		case KEY_UP:
			cursor_Y++;
			break;
		case KEY_DOWN:
			cursor_Y--;
			break;
		case KEY_LEFT:
			cursor_X--;
			break;
		case KEY_RIGHT:
			cursor_X++;
			break;
		case KEY_ENTER:
			main_array[cursor_Y][cursor_X] = '+';
			break;
		}
		if (cursor_Y < 0){
			row++;
			main_array = cloneArrayValues(row, col, 1, 0, main_array);
			cursor_Y = 0;
		}
		if (cursor_Y >= row){
			row++;
			main_array = cloneArrayValues(row, col, -1, 0, main_array);
			
		}
		if (cursor_X < 0){
			col++;
			main_array = cloneArrayValues(row, col, 0, 1, main_array);
			cursor_X = 0;
		}
		if (cursor_X >= col) {
			col++;
			main_array = cloneArrayValues(row, col, 0, -1, main_array);
			
		}
	}
	cout << "Amoba jatek" << endl;
	cout << "Jatek kezdesehez nyomj Enter-t" << endl << "Kilepeshez ESC-t" << endl;

	return 0;
}
