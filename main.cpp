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
	char** newCharArray = new char* [row];
	for (int i = 0; i < row; i++) {
		newCharArray[i] = new char[col];
		for (int j = 0; j < col; j++) {
			newCharArray[i][j] = '.';
		}
	}
	return newCharArray;
}

char** cloneArrayValues(int row_from ,int row_to , int col_from, int col_to, char** original_board, char** new_board) {
	/* Lemásolja a orignal array értékeit */
	char** newCharArray = new_board;
	for (int i = row_from; i < row_to; i++) {
		for (int j = col_from; j < col_to; j++) {
			newCharArray[i][j] << original_board[i][j];
		}

	}
	return newCharArray;
}

char** expandBoard(int row_orig, int col_orig, char** original_board,bool left,bool down) {
	/*
	* keypress col row bővítés ha a pálya szélén van
	*/
	int row;
	int col;
	
	row = row_orig;
	col = col_orig;
	//új tömb
	char** newCharArray = initBoard(row, col);
	//cloneArrayValues(row_from, row_to, col_from, col_to, original_board, newCharArray);

	return newCharArray;
}

int main(int argc, char** argv) {
	int row = 3;
	int col = 3;
	int vision=2;
	char** main_array = initBoard(row, col);


	while (true)
	{
		system("cls");
		cout << endl << cursor_X << ":" << cursor_Y<<endl;

		for (int i = cursor_X - vision; i < cursor_X + vision + 1; i++) {
			for (int j = cursor_X - vision; j < cursor_Y + vision + 1; j++) {
				if (i < 0 || j < 0 || row <= i || col <= j) {
					cout << "-";
				}
				else {
					cout << main_array[i][j];
				}
			}
			cout << "\n";
		}
		switch (_getch()) {
		case KEY_UP:
			cursor_X--;
			break;
		case KEY_DOWN:
			cursor_X++;
			break;
		case KEY_LEFT:
			cursor_Y--;
			break;
		case KEY_RIGHT:
			cursor_Y++;
			break;
		case KEY_ENTER:
			main_array[cursor_X][cursor_Y] = '+';
			break;
		}
		if (cursor_X < 0)
			cursor_X = 0;
		if (cursor_X >= col)
			cursor_X = col - 1;
		if (cursor_Y < 0)
			cursor_Y = 0;
		if (cursor_Y >= row)
			cursor_Y = row - 1;
	}
	cout << "Amoba jatek" << endl;
	cout << "Jatek kezdesehez nyomj Enter-t" << endl << "Kilepeshez ESC-t" << endl;
	
	return 0;
}

