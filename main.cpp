
//#include "pch.h"
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
#define KEY_J 106
#define KEY_M 109
#define KEY_P 112
#define KEY_U 117

/*Játékos változó*/
const char Players[] = { 'O','X' };

/*Színkezelő változó*/
const HANDLE  hConsole = GetStdHandle(STD_OUTPUT_HANDLE);


using namespace std;
int cursor_X = 0;
int cursor_Y = 0;

int cursor_x = 0;
int cursor_y = 0;

char**** initMainBoard(int row, int col, bool make_it_empty = false) {
	char default_v;
	if (make_it_empty) {
		default_v = '-';
	}
	else {
		default_v = '.';
	}
	char**** newCharArray = new char*** [row];
	for (int i = 0; i < row; i++) {
		newCharArray[i] = new char** [col];
		for (int j = 0; j < col; j++) {
			newCharArray[i][j] = new char* [2];
			for (int k = 0; k < 2; k++)
			{
				newCharArray[i][j][k] = new char[2];
				for (int l = 0; l < 2; l++)
				{
					newCharArray[i][j][k][l] = default_v;
				}

			}
		}
	}
	return newCharArray;
}


char**** cloneArrayValues(int row, int col, int row_extra, int col_extra, char**** original_board) {


	/* Lemásolja a orignal array értékeit */
	char**** newCharArray = initMainBoard(row, col, true);
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
			newCharArray[i + row_extra][j + col_extra] = original_board[i][j];
		}
		delete[] original_board[i];
	}
	delete[] original_board;
	return newCharArray;
}

int main(int argc, char** argv) {

	int player = 0;
	int row = 1;
	int col = 1;
	int vision = 2;
	int limit_for_win = 5;
	int limit = 0;
	bool win = false;
	char** mozgatando = new char*[2];
	
	int lehejezett_ernyo = 0; //max 36 - játék vége
	int jatekoslapka = 0; //max 16

	int akcio = 1;
	int mode = 0;

	bool mozgathathat = false;
	bool mozgatott = false;
	bool jelolhet = true;
	bool jelolt = false;
	bool passzolhat = false;
	int oldalszomszed_szam = 0;

	char**** main_array = initMainBoard(row, col);
	int i;
	int j;
	int k;
	int l;
	while (true)
	{
		/*################################kijezlő################################################*/
		system("cls");
		SetConsoleTextAttribute(hConsole, 7);

		cout << endl << "Cursor X:" << cursor_X << " - " << "Cursor Y:" << cursor_Y << endl;
		cout << endl << "Cursor x:" << cursor_x << " - " << "Cursor y:" << cursor_y << endl;
		cout << endl << "row : " << row << ":" << " col : " << col << endl;

		/*sor*/
		for (i = cursor_Y + vision; i > cursor_Y - vision - 1; i--) {
			/*subsor*/

			for (k = 1; k > -1; k--)
			{
				/*oszlop*/
				for (j = cursor_X - vision; j < cursor_X + vision + 1; j++) {

					/*suboszlop*/
					for (l = 0; l < 2; l++)
					{
						if (i < 0 || j < 0 || row <= i || col <= j) {
							if (mode == 2 && i == cursor_Y && j == cursor_X)
								SetConsoleTextAttribute(hConsole, 14);
							else if (mode == 3 && i == cursor_Y && j == cursor_X)
								SetConsoleTextAttribute(hConsole, 23);
							else if (mode == 4 && i == cursor_Y && j == cursor_X)
								SetConsoleTextAttribute(hConsole, 41);

							cout << ":";
						}
						else {
							if (i == cursor_Y && j == cursor_X && k == cursor_y && l == cursor_x)
							{
							
								if (main_array[i][j][k][j] == Players[0] || main_array[i][j][k][j] == Players[1])
									SetConsoleTextAttribute(hConsole, BACKGROUND_RED);
								else
									SetConsoleTextAttribute(hConsole, BACKGROUND_GREEN);
							}
							else
								SetConsoleTextAttribute(hConsole, 7);
							if (mode == 2 && i == cursor_Y && j == cursor_X)
								SetConsoleTextAttribute(hConsole, 14);
							else if (mode == 3 && i == cursor_Y && j == cursor_X)
								SetConsoleTextAttribute(hConsole, 23);
							else if (mode == 4 && i == cursor_Y && j == cursor_X)
								SetConsoleTextAttribute(hConsole, 41);

							cout << main_array[i][j][k][l];

						}
					}
					SetConsoleTextAttribute(hConsole, 9);
					if (j != cursor_X + vision)
						cout << "|";
					SetConsoleTextAttribute(hConsole, 7);


				}
				if (k == 1)
					cout << "\n";

			}
			cout << "\n";
			SetConsoleTextAttribute(hConsole, 9);

			for (int v = 0; v < (3 + vision) * 3; v++)
			{
				if (v != (3 + vision) * 3 - 1)
					if ((v + 1) % 3 == 0)
						cout << "+";
					else
						cout << "-";
			}
			SetConsoleTextAttribute(hConsole, 7);

			cout << "\n";
		}


		/*################################mód_kiválasztás################################################*/

		/* Jelölsz vagy mozgatsz?
			- az elején nem mozgathat

		*/
		if (mode == 0) {
			if (jelolhet)
				SetConsoleTextAttribute(hConsole, 7);
			else
				SetConsoleTextAttribute(hConsole, 8);
			cout << "Jeloleshez - J" << endl;
			if (mozgathathat)
				SetConsoleTextAttribute(hConsole, 7);
			else
				SetConsoleTextAttribute(hConsole, 8);
			cout << "Mozgatashoz - M" << endl;
			if (passzolhat)
				SetConsoleTextAttribute(hConsole, 7);
			else
				SetConsoleTextAttribute(hConsole, 8);
			cout << "Passzolashoz - P" << endl;
			if (jatekoslapka < 16)
				SetConsoleTextAttribute(hConsole, 7);
			else
				SetConsoleTextAttribute(hConsole, 8);
			cout << "Uj lerakashoz - U" << endl;

			switch (_getch()) {
			case KEY_J:
				mode = 1;
				break;
			case KEY_U:
				mode = 2;
				break;
			case KEY_M:
				mode = 3;
				break;
			}
		}
		/*################################modóok###############################################################*/
		else {
			//mode 1 jelölés->mode0
			if (mode == 1) {
				cout << "Jeloleshez - Enter";
				switch (_getch()) {
				case KEY_UP:
					cursor_y++;
					if (cursor_y > 1) {
						cursor_Y++;
						if (cursor_Y > row - 1) {
							cout << endl << "Ervenytelen" << endl << "Folytatashoz uss le egy billentyut..." << endl;
							cin.ignore();
							cursor_Y--;
							cursor_y--;
							continue;
						}
						else {
							cursor_y -= 2;
						}
					}
					break;
				case KEY_DOWN:
					cursor_y--;
					if (cursor_y < 0) {
						cursor_Y--;
						if (cursor_Y < 0) {
							cout << endl << "Ervenytelen" << endl << "Folytatashoz uss le egy billentyut..." << endl;
							cin.ignore();
							cursor_Y++;
							cursor_y++;
							continue;
						}
						else {
							cursor_y += 2;
						}
					}
					break;
				case KEY_LEFT:
					cursor_x--;
					if (cursor_x < 0) {
						cursor_X--;
						if (cursor_X < 0) {
							cout << endl << "Ervenytelen" << endl << "Folytatashoz uss le egy billentyut..." << endl;
							cin.ignore();
							cursor_X++;
							cursor_x++;
							continue;
						}
						else {
							cursor_x += 2;
						}
					}
					break;
				case KEY_RIGHT:
					cursor_x++;
					if (cursor_x > 1) {
						cursor_X++;
						if (cursor_X > col - 1) {
							cout << endl << "Ervenytelen" << endl << "Folytatashoz uss le egy billentyut..." << endl;
							cin.ignore();
							cursor_X--;
							cursor_x--;
							continue;
						}
						else {
							cursor_x -= 2;
						}
					}
					break;
				case KEY_ENTER:
					if (main_array[cursor_Y][cursor_X][cursor_y][cursor_x] != '.') {
						cout << "Ervenytelen" << endl << "Folytatashoz uss le egy billentyut..." << endl;
						cin.ignore();
						continue;
					}
					else {
						main_array[cursor_Y][cursor_X][cursor_y][cursor_x] = Players[player];
						mode = 0;
					}
					break;
				}
			}
			//mode 2 új lehelyezése->mode0
			if (mode == 2) {
				switch (_getch()) {
				case KEY_UP:
					cursor_y++;
					if (cursor_y > 1) {
						cursor_Y++;
						if (cursor_Y > row) {
							cout << endl << "Ervenytelen" << endl << "Folytatashoz uss le egy billentyut..." << endl;
							cin.ignore();
							cursor_Y--;
							cursor_y--;
							continue;
						}
						else {
							cursor_y -= 2;
						}
					}
					break;
				case KEY_DOWN:
					cursor_y--;
					if (cursor_y < 0) {
						cursor_Y--;
						if (cursor_Y < -1) {
							cout << endl << "Ervenytelen" << endl << "Folytatashoz uss le egy billentyut..." << endl;
							cin.ignore();
							cursor_Y++;
							cursor_y++;
							continue;
						}
						else {
							cursor_y += 2;
						}
					}
					break;
				case KEY_LEFT:
					cursor_x--;
					if (cursor_x < 0) {
						cursor_X--;
						if (cursor_X < -1) {
							cout << endl << "Ervenytelen" << endl << "Folytatashoz uss le egy billentyut..." << endl;
							cin.ignore();
							cursor_X++;
							cursor_x++;
							continue;
						}
						else {
							cursor_x += 2;
						}
					}
					break;
				case KEY_RIGHT:
					cursor_x++;
					if (cursor_x > 1) {
						cursor_X++;
						if (cursor_X > col) {
							cout << endl << "Ervenytelen" << endl << "Folytatashoz uss le egy billentyut..." << endl;
							cin.ignore();
							cursor_X--;
							cursor_x--;
							continue;
						}
						else {
							cursor_x -= 2;
						}
					}
					break;
				case KEY_ENTER:
					/*bővítés*/
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
					bool err = false;
					/* Megnézzük hogy van e már valami ott ahova új lapot akar lerakni */
					if (cursor_Y >= 0 && cursor_Y < row && cursor_X >= 0 && cursor_X < col) {
						if (main_array[cursor_Y][cursor_X][0][0] != '-') {
							err = true;
						}

					}
					/* Megnézzük hogy van e már oldalszomszédja */
					bool oldalszomszed = false;
					if (cursor_Y - 1 >= 0) {
						if (main_array[cursor_Y-1][cursor_X][0][0] != '-') {
							oldalszomszed = true;
						
						}
					}
					if (cursor_Y + 1 < row) {
						if (main_array[cursor_Y +1][cursor_X][0][0] != '-') {
							oldalszomszed = true;

						}
					}
					if (cursor_X - 1 >= 0) {
						if (main_array[cursor_Y][cursor_X - 1][0][0] != '-') {
							oldalszomszed = true;

						}
					}
					if (cursor_X + 1 < col) {
						if (main_array[cursor_Y][cursor_X + 1][0][0] != '-') {
							oldalszomszed = true;
						}
					}

					if (err ) {
						cout << endl << "Ervenytelen" << endl << "Folytatashoz uss le egy billentyut..." << endl;
						cin.ignore();
					}
					else if (!oldalszomszed) {
						cout << endl << "Nincs oldalszomszed" << endl << "Folytatashoz uss le egy billentyut..." << endl;
						cin.ignore();
					}
					else {
						for (int i = 0; i < 2; i++)
						{
							for (int j = 0; j < 2; j++)
							{
								main_array[cursor_Y][cursor_X][i][j] = '.';
							}
						}
						mode = 0;

						break;
					}
				}
			}
			//mode 4 tile áthelyezése hova ->mode0
			if (mode == 4) {
				switch (_getch()) {
				case KEY_UP:
					cursor_y++;
					if (cursor_y > 1) {
						cursor_Y++;
						if (cursor_Y > row) {
							cout << endl << "Ervenytelen" << endl << "Folytatashoz uss le egy billentyut..." << endl;
							cin.ignore();
							cursor_Y--;
							cursor_y--;
							continue;
						}
						else {
							cursor_y -= 2;
						}
					}
					break;
				case KEY_DOWN:
					cursor_y--;
					if (cursor_y < 0) {
						cursor_Y--;
						if (cursor_Y < -1) {
							cout << endl << "Ervenytelen" << endl << "Folytatashoz uss le egy billentyut..." << endl;
							cin.ignore();
							cursor_Y++;
							cursor_y++;
							continue;
						}
						else {
							cursor_y += 2;
						}
					}
					break;
				case KEY_LEFT:
					cursor_x--;
					if (cursor_x < 0) {
						cursor_X--;
						if (cursor_X < -1) {
							cout << endl << "Ervenytelen" << endl << "Folytatashoz uss le egy billentyut..." << endl;
							cin.ignore();
							cursor_X++;
							cursor_x++;
							continue;
						}
						else {
							cursor_x += 2;
						}
					}
					break;
				case KEY_RIGHT:
					cursor_x++;
					if (cursor_x > 1) {
						cursor_X++;
						if (cursor_X > col) {
							cout << endl << "Ervenytelen" << endl << "Folytatashoz uss le egy billentyut..." << endl;
							cin.ignore();
							cursor_X--;
							cursor_x--;
							continue;
						}
						else {
							cursor_x -= 2;
						}
					}
					break;
				case KEY_ENTER:
					/*bővítés*/
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
					bool err = false;
					/* Megnézzük hogy van e már valami ott ahova új lapot akar lerakni */
					if (cursor_Y >= 0 && cursor_Y < row && cursor_X >= 0 && cursor_X < col) {
						if (main_array[cursor_Y][cursor_X][0][0] != '-') {
							err = true;
						}

					}
					/* Megnézzük hogy van e már oldalszomszédja */
					bool oldalszomszed = false;
					if (cursor_Y - 1 >= 0) {
						if (main_array[cursor_Y - 1][cursor_X][0][0] != '-') {
							oldalszomszed = true;

						}
					}
					if (cursor_Y + 1 < row) {
						if (main_array[cursor_Y + 1][cursor_X][0][0] != '-') {
							oldalszomszed = true;

						}
					}
					if (cursor_X - 1 >= 0) {
						if (main_array[cursor_Y][cursor_X - 1][0][0] != '-') {
							oldalszomszed = true;

						}
					}
					if (cursor_X + 1 < col) {
						if (main_array[cursor_Y][cursor_X + 1][0][0] != '-') {
							oldalszomszed = true;
						}
					}

					if (err) {
						cout << endl << "Ervenytelen" << endl << "Folytatashoz uss le egy billentyut..." << endl;
						cin.ignore();
					}
					else if (!oldalszomszed) {
						cout << endl << "Nincs oldalszomszed" << endl << "Folytatashoz uss le egy billentyut..." << endl;
						cin.ignore();
					}
					else {

						for (int k = 0; k < 2; k++)
						{
							for (int l = 0; l < 2; l++)
							{
								main_array[cursor_Y][cursor_X][k][l] = mozgatando[k][l];

							}
						}
						//						main_array[cursor_Y][cursor_X]=mozgatando;

						mode = 0;

						break;
					}
				}

			}
			//mode 3 tile áthelyezése honnan->mode4
			if (mode == 3) {
				switch (_getch()) {
				case KEY_UP:
					cursor_y++;
					if (cursor_y > 1) {
						cursor_Y++;
						if (cursor_Y > row - 1) {
							cout << endl << "Ervenytelen" << endl << "Folytatashoz uss le egy billentyut..." << endl;
							cin.ignore();
							cursor_Y--;
							cursor_y--;
							continue;
						}
						else {
							cursor_y -= 2;
						}
					}
					break;
				case KEY_DOWN:
					cursor_y--;
					if (cursor_y < 0) {
						cursor_Y--;
						if (cursor_Y < 0) {
							cout << endl << "Ervenytelen" << endl << "Folytatashoz uss le egy billentyut..." << endl;
							cin.ignore();
							cursor_Y++;
							cursor_y++;
							continue;
						}
						else {
							cursor_y += 2;
						}
					}
					break;
				case KEY_LEFT:
					cursor_x--;
					if (cursor_x < 0) {
						cursor_X--;
						if (cursor_X < 0) {
							cout << endl << "Ervenytelen" << endl << "Folytatashoz uss le egy billentyut..." << endl;
							cin.ignore();
							cursor_X++;
							cursor_x++;
							continue;
						}
						else {
							cursor_x += 2;
						}
					}
					break;
				case KEY_RIGHT:
					cursor_x++;
					if (cursor_x > 1) {
						cursor_X++;
						if (cursor_X > col - 1) {
							cout << endl << "Ervenytelen" << endl << "Folytatashoz uss le egy billentyut..." << endl;
							cin.ignore();
							cursor_X--;
							cursor_x--;
							continue;
						}
						else {
							cursor_x -= 2;
						}
					}
					break;
				case KEY_ENTER:
					oldalszomszed_szam = 0;
					if (cursor_Y - 1 >= 0) {
						if (main_array[cursor_Y - 1][cursor_X][0][0] != '-') {
							oldalszomszed_szam++;

						}
					}
					if (cursor_Y + 1 < row) {
						if (main_array[cursor_Y + 1][cursor_X][0][0] != '-') {
							oldalszomszed_szam++;

						}
					}
					if (cursor_X - 1 >= 0) {
						if (main_array[cursor_Y][cursor_X - 1][0][0] != '-') {
							oldalszomszed_szam++;

						}
					}
					if (cursor_X + 1 < col) {
						if (main_array[cursor_Y][cursor_X + 1][0][0] != '-') {
							oldalszomszed_szam++;
						}
					}
					if (oldalszomszed_szam==4) {
						cout << "Ervenytelen korbe van zarva" << endl << "Folytatashoz uss le egy billentyut..." << endl;
						cin.ignore();
						continue;
					}
					else if (main_array[cursor_Y][cursor_X][cursor_y][cursor_x] == '-') {
						cout << "Ervenytelen" << endl << "Folytatashoz uss le egy billentyut..." << endl;
						cin.ignore();
						continue;
					}
					/*Ellenörizzük hogy van e 4 szomszédos lapka*/

					else {
						//mozgatando = main_array[cursor_Y][cursor_X];
						for (int k = 0; k < 2; k++)
						{
							mozgatando[k] = new char[2];
							for (int l = 0; l < 2; l++)
							{
								mozgatando[k][l] = main_array[cursor_Y][cursor_X][k][l];
								main_array[cursor_Y][cursor_X][k][l] = '-';
							}

						}
						
						mode = 4;
					}
					break;
				}
			}
			
		}
		/*felszabadítás*/
	}
	return 0;

}
