
#include "pch.h"
#include <iostream>
#include <windows.h>
#include <conio.h>
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
	else if (row_extra == 0)
		row_extra = 0;
	else
		row--;

	if (col_extra < 0) {
		col--;
		col_extra = 0;
	}
	else if (col_extra == 0)
		col_extra = 0;
	else
		col--;
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			newCharArray[i+row_extra][j+col_extra] = original_board[i][j];
		}
	}
	delete[] original_board;
	return newCharArray;
}

int main(int argc, char** argv) {

	int player = 0;
	int row = 3;
	int col = 3;
	int vision = 5;
	int limit_for_win = 3;
	int limit = 0;
	bool win = false;
	char** main_array = initBoard(row, col);
	int i;
	int j;
	while (true)
	{
		system("cls");
		SetConsoleTextAttribute(hConsole, 7);

		cout << endl << cursor_X << ":" << cursor_Y << endl;
		cout << endl << "row : " << row << ":" << " col : " << col << endl;

		for (int i = cursor_Y + vision; i > cursor_Y - vision - 1; i--) {
			for (int j = cursor_X - vision; j < cursor_X + vision + 1; j++) {
				if (i < 0 || j < 0 || row <= i || col <= j) {
					cout << ":";
				}
				else {
					if (i == cursor_Y && j == cursor_X)
					{
						if (main_array[i][j] == Players[0] || main_array[i][j] == Players[0])
							SetConsoleTextAttribute(hConsole, BACKGROUND_RED);
						else
							SetConsoleTextAttribute(hConsole, BACKGROUND_GREEN);
					}
					else
						SetConsoleTextAttribute(hConsole, 7);
					cout << main_array[i][j];
				}
				SetConsoleTextAttribute(hConsole, 7);
			}
			cout << "\n";
		}
		if (win) {

			int winner;
			if (player % 2 == 0)
				winner = 1;
			else
				winner = 0;
			cout << "Amoba jatek" << endl;
			cout << Players[winner] << " jatekos - nyert" << endl;
			cout << "Jatek kezdesehez nyomj Enter-t" << endl << "Kilepeshez ESC-t" << endl;
			break;
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
			if (main_array[cursor_Y][cursor_X] == Players[0] || main_array[cursor_Y][cursor_X] == Players[1]) {
				cout << "Ervenytelen" << endl << "Folytatashoz uss le egy billentyut..." << endl;
				//system("pause");
				cin.ignore();
				continue;
			}
			else
				main_array[cursor_Y][cursor_X] = Players[player];
			break;
		}
		if (cursor_Y < 0) {
			row++;
			main_array = cloneArrayValues(row, col, 1, 0, main_array);
			cursor_Y = 0;
		}
		if (cursor_Y >= row) {
			row++;
			main_array = cloneArrayValues(row, col, -1, 0, main_array);

		}
		if (cursor_X < 0) {
			col++;
			main_array = cloneArrayValues(row, col, 0, 1, main_array);
			cursor_X = 0;
		}
		if (cursor_X >= col) {
			col++;
			main_array = cloneArrayValues(row, col, 0, -1, main_array);
		}
		// ellenörzés soronként

		i = cursor_Y;

		//for (i = cursor_Y + limit_for_win; i > cursor_Y - limit_for_win - 1; i--) {
		limit = 0;
		for (j = cursor_X - limit_for_win; j < cursor_X + limit_for_win + 1; j++) {
			//j = cursor_X;
			if (i < 0 || j < 0 || row <= i || col <= j) {
				limit = 0;
			}
			else {
				if (main_array[i][j] != Players[player])
					limit = 0;
				else {
					limit++;
					if (limit == limit_for_win)
						win = true;
				}
			}

		}
		//}
		//ellenörzés oszloponként 
		j = cursor_X;

		//for ( j = cursor_X - limit_for_win; j < cursor_X + limit_for_win + 1; j++) {
		limit = 0;
		for (i = cursor_Y + limit_for_win; i > cursor_Y - limit_for_win - 1; i--) {
	//		cout << i << "_" << j;

			if (i < 0 || j < 0 || row <= i || col <= j) {
				limit = 0;
			}
			else {
				if (main_array[i][j] != Players[player])
					limit = 0;
				else {
					limit++;
					if (limit == limit_for_win)
						win = true;
				}
			}
			//}
		}
		//ellenörzés átlóban 
		j = cursor_X - limit_for_win;
		limit = 0;
		for (i = cursor_Y + limit_for_win; i > cursor_Y - limit_for_win - 1; i--) {
			if (i < 0 || j < 0 || row <= i || col <= j) {
				limit = 0;
			}
			else {
				if (main_array[i][j] != Players[player])
					limit = 0;
				else {
					limit++;
					if (limit == limit_for_win)
						win = true;
				}
			}

			j++;
		}
		j = cursor_X - limit_for_win;
		limit = 0;
		for (i = cursor_Y - limit_for_win; i < cursor_Y + limit_for_win + 1; i++) {
			if (i < 0 || j < 0 || row <= i || col <= j) {
				limit = 0;
			}
			else {
				if (main_array[i][j] != Players[player])
					limit = 0;
				else {
					limit++;
					if (limit == limit_for_win)
						win = true;
				}
			}
			j++;
		}
		if (player % 2 == 0)
			player++;
		else
			player--;
	}

	return 0;
}
