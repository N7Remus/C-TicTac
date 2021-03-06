
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

int row = 1;
int col = 1;
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
char**** main_array = initMainBoard(row, col);
int cursor_X = 0;
int cursor_Y = 0;

int cursor_x = 0;
int cursor_y = 0;


int player = 0;
int vision = 3;
int limit_for_win = 5;
int limit = 0;
bool win = false;
char** mozgatando = new char* [2];


int lehejezett_ernyo = 0; //max 36 - játék vége
int jatekoslapka = 1; //max 16

int akcio = 1;
int mode = 0;

int mozgatas_elotti_X;
int mozgatas_elotti_Y;
bool mozgathathat = false;
bool jelenleg_mozgat = false;
bool jelolhet = true;
bool passzolhat = false;
bool ujjat_lehejezhet = false;
int oldalszomszed_szam = 0;
int elozo_korben_jelolt_Y = NULL;
int elozo_korben_jelolt_X = NULL;
int ebben_a_korben_lerakott_X;
int ebben_a_korben_lerakott_Y;
void ervenytelen_lepes(string s) {
	cout << endl
		<< s << endl
		<< "Folytatashoz -Enter..." << endl;
	cin.ignore();

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
void lepes_UP() {
	cursor_y++;
	if (cursor_y > 1) {
		cursor_Y++;
		if (cursor_Y > row - 1) {
			cout << endl << "Ervenytelen" << endl << "Folytatashoz uss le egy billentyut..." << endl;
			cin.ignore();
			cursor_Y--;
			cursor_y--;
		}
		else {
			cursor_y -= 2;
		}
	}
}
void lepes_UP_dupla(int csokkent = 0) {
	cursor_Y++;
	if (cursor_Y > row-csokkent) {
		ervenytelen_lepes("Ervenytelen");
		cursor_Y--;
	}
}
void lepes_DOWN() {
	cursor_y--;
	if (cursor_y < 0) {
		cursor_Y--;
		if (cursor_Y < 0) {
			cout << endl << "Ervenytelen" << endl << "Folytatashoz uss le egy billentyut..." << endl;
			cin.ignore();
			cursor_Y++;
			cursor_y++;
		}
		else {
			cursor_y += 2;
		}
	}
}
void lepes_DOWN_dupla(int csokkent = 0) {
	cursor_Y--;
	if (cursor_Y < -1+csokkent) {
		ervenytelen_lepes("Ervenytelen");
		cursor_Y++;
	}
}
void lepes_LEFT() {
	cursor_x--;
	if (cursor_x < 0) {
		cursor_X--;
		if (cursor_X < 0) {
			cout << endl << "Ervenytelen" << endl << "Folytatashoz uss le egy billentyut..." << endl;
			cin.ignore();
			cursor_X++;
			cursor_x++;
		}
		else {
			cursor_x += 2;
		}
	}
}
void lepes_LEFT_dupla(int csokkent = 0) {
	cursor_X--;
	if (cursor_X < -1+csokkent) {
		ervenytelen_lepes("Ervenytelen");
		cursor_X++;
	}

}
void lepes_RIGHT() {
	cursor_x++;
	if (cursor_x > 1) {
		cursor_X++;
		if (cursor_X > col - 1) {
			cout << endl << "Ervenytelen" << endl << "Folytatashoz uss le egy billentyut..." << endl;
			cin.ignore();
			cursor_X--;
			cursor_x--;
		}
		else {
			cursor_x -= 2;
		}
	}
}
void lepes_RIGHT_dupla(int csokkent = 0) {
	cursor_X++;
	if (cursor_X > col-csokkent) {
		ervenytelen_lepes("Ervenytelen");
		cursor_X--;
	}
}
bool palyaszakadas() {
	bool palyaszakadas = false;
	int oldalszomszed_szam_V = 0;
	int i;
	/* Tömb debugoláshoz illetve vizuális pályaszakadás keresésre */
	int** arr = new int* [row];
	for (int i = 0; i < row; i++)
	{
		arr[i] = new int[col];
		for (int j = 0; j < col; j++)
		{
			arr[i][j] = 0;
		}
	}
	for (i = row - 1; i > -1; i--) {
		for (int j = 0; j < col; j++)
		{
			// megnézzük hogy a vizsgált elemnek van e szomszédja 
			oldalszomszed_szam_V = 0;
			// ha amit vizsgálok ne legyen 'üres'
			if (main_array[i][j][0][0] != '-' ) {

				// Amit vizsgálok oldalszomszéd ne legyen üres, ha az akkor nincs oldalszomszéd,

				// illetve ha az amit ki akarok venni
				if (i - 1 >= 0) {
					if (main_array[i - 1][j][0][0] != '-') {
						oldalszomszed_szam_V++;
					}
				}
				if (i + 1 < row) {
					if (main_array[i + 1][j][0][0] != '-' ) {
						oldalszomszed_szam_V++;
					}
				}
				if (j - 1 >= 0) {
					if (main_array[i][j - 1][0][0] != '-' ) {
						oldalszomszed_szam_V++;
					}
				}
				if (j + 1 < col) {
					if (main_array[i][j + 1][0][0] != '-' ) {
						oldalszomszed_szam_V++;
					}
				}
				if (oldalszomszed_szam_V < 1 && jatekoslapka>2) {
					//palyaszakadas = true;
				}
				oldalszomszed_szam_V++;

			}
			if (oldalszomszed_szam_V < 1) {
				cout << 0;
				arr[i][j] = 0;
			}
			else {
				cout << 1;
				arr[i][j] = 1;
			}
		}
		cout << endl;
	}

	int legnagyobb;
	int incremental = 2;
	cout << endl;

	for (i = row - 1; i > -1; i--) {
		for (int j = col - 1; j > -1; j--)
		{

			/* Amit vizsgálok ne legyen 'üres' */
			if (arr[i][j] != 0) {
				legnagyobb = 0;
				if (i - 1 >= 0) {
					if (arr[i - 1][j] > legnagyobb) {
						legnagyobb = arr[i - 1][j];
					}
				}
				if (i + 1 < row) {
					if (arr[i + 1][j] > legnagyobb) {
						legnagyobb = arr[i + 1][j];
					}
				}
				if (j - 1 >= 0) {
					if (arr[i][j - 1] > legnagyobb) {
						legnagyobb = arr[i][j - 1];
					}
				}
				if (j + 1 < col) {
					if (arr[i][j + 1] > legnagyobb) {
						legnagyobb = arr[i][j + 1];
					}
				}
				/* Van nagyobb szomszéd? */
				if (legnagyobb > arr[i][j]) {
					arr[i][j] = legnagyobb;
				}
				else if (legnagyobb == arr[i][j])
				{
					if (incremental - 2 == 0) {
						arr[i][j] = incremental;
						incremental++;
					}
				}
				else {
					arr[i][j] = incremental;
					incremental++;
				}
			}
		}
		for (int j = 0; j < col; j++)
		{

			/* Amit vizsgálok ne legyen 'üres' */
			if (arr[i][j] != 0) {
				legnagyobb = arr[i][j];
				if (i - 1 >= 0) {
					if (arr[i - 1][j] > legnagyobb) {
						legnagyobb = arr[i - 1][j];
					}
				}
				if (i + 1 < row) {
					if (arr[i + 1][j] > legnagyobb) {
						legnagyobb = arr[i + 1][j];
					}
				}
				if (j - 1 >= 0) {
					if (arr[i][j - 1] > legnagyobb) {
						legnagyobb = arr[i][j - 1];
					}
				}
				if (j + 1 < col) {
					if (arr[i][j + 1] > legnagyobb) {
						legnagyobb = arr[i][j + 1];
					}
				}
				if (legnagyobb > arr[i][j]) {
					arr[i][j] = legnagyobb;
				}
				else if (legnagyobb == arr[i][j])
				{

				}
				else {
					arr[i][j] = incremental;
					incremental++;
				}
			}

			cout << arr[i][j];
		}

		cout << endl;
	}
	for (i = 0; i < row; i++) {
		for (int j = col - 1; j > -1; j--)
		{

			/* Amit vizsgálok ne legyen 'üres' */
			if (arr[i][j] != 0) {
				legnagyobb = 0;
				if (i - 1 >= 0) {
					if (arr[i - 1][j] > legnagyobb) {
						legnagyobb = arr[i - 1][j];
					}
				}
				if (i + 1 < row) {
					if (arr[i + 1][j] > legnagyobb) {
						legnagyobb = arr[i + 1][j];
					}
				}
				if (j - 1 >= 0) {
					if (arr[i][j - 1] > legnagyobb) {
						legnagyobb = arr[i][j - 1];
					}
				}
				if (j + 1 < col) {
					if (arr[i][j + 1] > legnagyobb) {
						legnagyobb = arr[i][j + 1];
					}
				}
				/* Van nagyobb szomszéd? */
				if (legnagyobb > arr[i][j]) {
					arr[i][j] = legnagyobb;
				}
				else if (legnagyobb == arr[i][j])
				{
					if (incremental - 2 == 0) {
						arr[i][j] = incremental;
						incremental++;
					}
				}
				else {
					arr[i][j] = incremental;
					incremental++;
				}
			}
		}
		for (int j = 0; j < col; j++)
		{

			/* Amit vizsgálok ne legyen 'üres' */
			if (arr[i][j] != 0) {
				legnagyobb = arr[i][j];
				if (i - 1 >= 0) {
					if (arr[i - 1][j] > legnagyobb) {
						legnagyobb = arr[i - 1][j];
					}
				}
				if (i + 1 < row) {
					if (arr[i + 1][j] > legnagyobb) {
						legnagyobb = arr[i + 1][j];
					}
				}
				if (j - 1 >= 0) {
					if (arr[i][j - 1] > legnagyobb) {
						legnagyobb = arr[i][j - 1];
					}
				}
				if (j + 1 < col) {
					if (arr[i][j + 1] > legnagyobb) {
						legnagyobb = arr[i][j + 1];
					}
				}
				if (legnagyobb > arr[i][j]) {
					arr[i][j] = legnagyobb;
				}
				else if (legnagyobb == arr[i][j])
				{

				}
				else {
					arr[i][j] = incremental;
					incremental++;
				}
			}

			cout << arr[i][j];
		}

		cout << endl;
	}
	/* Ellenörzés hogy minden elem ugyan az e, ha nem akkor pályaszakadás van*/

	for (i = row - 1; i > -1; i--) {
		for (int j = col - 1; j > -1; j--)
		{
			/* Amit vizsgálok ne legyen 'üres' */
			if (arr[i][j] != 0) {
				if (arr[i][j] != incremental - 1 && jatekoslapka > 2)
					palyaszakadas = true;
			}
			else {
//				palyaszakadas = false;
			}
		}
	}
	cin.ignore();

	for (int i = 0; i < row; i++)
	{
		delete[] arr[i];
	}
	delete[] arr;
	return palyaszakadas;
}
void bovit() {
	if (cursor_Y < 0) {
		row++;
		main_array = cloneArrayValues(row, col, 1, 0, main_array);
		cursor_Y = 0;
		mozgatas_elotti_Y++;
	}
	if (cursor_Y >= row) {
		row++;
		main_array = cloneArrayValues(row, col, -1, 0, main_array);
		//mozgatas_elotti_Y--;
	}
	if (cursor_X < 0) {
		col++;
		main_array = cloneArrayValues(row, col, 0, 1, main_array);
		cursor_X = 0;
		mozgatas_elotti_X++;
	}
	if (cursor_X >= col) {
		col++;
		main_array = cloneArrayValues(row, col, 0, -1, main_array);
		//mozgatas_elotti_X--;
	}
}
void ellenorzes() {
	int i;
	int j;
	int k;
	int l;
	//Ellenörzés ##########################################################
							/*Ellenörzés a sorban*/
	limit = 0;
	/*sor*/
	i = cursor_Y;
	/*subsor*/
	k = cursor_y;
	/*oszlop*/
	for (j = cursor_X - limit_for_win; j < cursor_X + limit_for_win + 1; j++) {
		/*suboszlop*/
		for (l = 0; l < 2; l++)
		{
			if (i < 0 || j < 0 || row <= i || col <= j) {
				/*érvénytelen lapkák*/
			}
			else {
				if (main_array[i][j][k][l] == Players[player])
				{
					limit++;
					if (limit == limit_for_win)
						win = true;
				}
				else {
					limit = 0;
				}

			}
		}
	}
	/*Ellenörzés oszlopban */
	limit = 0;
	/*oszlop*/
	j = cursor_X;
	/*suboszlop*/
	l = cursor_x;
	/*sor*/
	for (i = cursor_Y - limit_for_win; i < cursor_Y + limit_for_win + 1; i++) {
		/*subsor*/
		for (k = 0; k < 2; k++)
		{
			if (i < 0 || j < 0 || row <= i || col <= j) {
				/*érvénytelen lapkák*/
			}
			else {
				if (main_array[i][j][k][l] == Players[player])
				{
					limit++;
					if (limit == limit_for_win)
						win = true;
				}
				else {
					limit = 0;
				}

			}
		}
	}
	/* Ellenörzés átlóban */
	j = cursor_X - limit_for_win;
	limit = 0;
	for (i = cursor_Y + limit_for_win; i > cursor_Y - limit_for_win - 1; i--) {
		if (i < 0 || j < 0 || row <= i || col <= j) {
			limit = 0;
		}
		else {
			if (main_array[i][j][1][0] != Players[player])
				limit = 0;
			else {
				limit++;
				if (limit == limit_for_win)
					win = true;
			}
			if (main_array[i][j][0][1] != Players[player])
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

			if (main_array[i][j][0][0] != Players[player])
				limit = 0;
			else {
				limit++;
				if (limit == limit_for_win)
					win = true;
			}
			if (main_array[i][j][1][1] != Players[player])
				limit = 0;
			else {
				limit++;
				if (limit == limit_for_win)
					win = true;
			}
		}
		j++;
	}
}
bool oldalszomszed() {

	/* Megnézzük hogy van e már oldalszomszédja */
	bool o = false;
	if (cursor_Y - 1 >= 0) {
		if (main_array[cursor_Y - 1][cursor_X][0][0] != '-') {
			o = true;

		}
	}
	if (cursor_Y + 1 < row) {
		if (main_array[cursor_Y + 1][cursor_X][0][0] != '-') {
			o = true;

		}
	}
	if (cursor_X - 1 >= 0) {
		if (main_array[cursor_Y][cursor_X - 1][0][0] != '-') {
			o = true;

		}
	}
	if (cursor_X + 1 < col) {
		if (main_array[cursor_Y][cursor_X + 1][0][0] != '-') {
			o = true;
		}
	}
	return o;
}
void kijelzes() {
	int i;
	int j;
	int k;
	int l;
	cout << endl << "Cursor X:" << cursor_X << " - " << "Cursor Y:" << cursor_Y << endl;
	cout << endl << "Cursor x:" << cursor_x << " - " << "Cursor y:" << cursor_y << endl;
	cout << endl << "row : " << row << ":" << " col : " << col << " jatekos:" << Players[player] << endl;
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
						else
							SetConsoleTextAttribute(hConsole, 8);

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
				if (player == 1)
					SetConsoleTextAttribute(hConsole, 53);
				else
					SetConsoleTextAttribute(hConsole, 64);
				if (j != cursor_X + vision)
					cout << "|";
				SetConsoleTextAttribute(hConsole, 7);


			}
			if (k == 1)
				cout << "\n";

		}
		cout << "\n";
		SetConsoleTextAttribute(hConsole, 9);

		for (int v = 0; v < 3 + (vision * 2) * 3; v++)
		{
			if (v != 3 + (vision * 2) * 3 - 1)
				if (player == 1)
					SetConsoleTextAttribute(hConsole, 53);
				else
					SetConsoleTextAttribute(hConsole, 64);
			if ((v + 1) % 3 == 0)
				cout << "+";
			else
				cout << "-";
		}
		SetConsoleTextAttribute(hConsole, 7);

		cout << "\n";
	}


}

void status() {
	if (!jelolhet && !mozgathathat && !ujjat_lehejezhet && !passzolhat && !jelenleg_mozgat) {
		if (player == 0)
			player = 1;
		else
			player = 0;
		jelolhet = true;
		passzolhat = false;
		ujjat_lehejezhet = true;
		if (jatekoslapka > 1)
			mozgathathat = true;

		ebben_a_korben_lerakott_X = -1;
		ebben_a_korben_lerakott_X = -1;
	}
}

int main(int argc, char** argv) {



	while (!win)
	{
		/*################################kijezlő################################################*/
		system("cls");
		SetConsoleTextAttribute(hConsole, 7);

		status();
		kijelzes();
		/*################################mód_kiválasztás################################################*/

		/* Jelölsz vagy mozgatsz?
			- az elején nem mozgathat

			jelöl/(uj ->jelöl)		->mozgathat/pasz
			mozgat					->mozgat/pasz

		*/
		if (mode == 0) {
			if (jelolhet && lehejezett_ernyo < jatekoslapka * 4)
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

			if (ujjat_lehejezhet && jatekoslapka < 16)
				SetConsoleTextAttribute(hConsole, 7);
			else
				SetConsoleTextAttribute(hConsole, 8);
			cout << "Uj lerakashoz - U" << endl;

			switch (_getch()) {

			case KEY_J:
				if (jelolhet && lehejezett_ernyo < jatekoslapka * 4) {
					mode = 1;
					ujjat_lehejezhet = false;
					akcio = 2;
					if (jatekoslapka > 1)
						mozgathathat = true;
					passzolhat = true;
					lehejezett_ernyo++;
				}
				else
				{
					ervenytelen_lepes("Nem Jelolhet!");
					cin.ignore();
				}
				continue;

				break;

			case KEY_U:
				if (ujjat_lehejezhet) {
					mode = 2;
					ujjat_lehejezhet = false;
					jatekoslapka++;
					mozgathathat = false;
				}
				else
				{
					cout << "Nem helyezhet le ujjat!" << "Folytatashoz uss le egy billentyut..." << endl;
					cin.ignore();
				}
				continue;
				break;

			case KEY_M:
				if (mozgathathat) {
					mode = 3;
					if (akcio == 2) {
						akcio = 1;
						mozgathathat = false;
						passzolhat = false;
						jelenleg_mozgat = true;
					}
					else {
						mozgathathat = true;
						passzolhat = true;
					}

					akcio++;
					jelolhet = false;
					ujjat_lehejezhet = false;
				}
				else
				{
					cout << "Nem mozgathat!" << "Folytatashoz uss le egy billentyut..." << endl;
					cin.ignore();
				}
				continue;


				break;
			case KEY_P:
				if (passzolhat) {
					if (player == 0)
						player = 1;
					else
						player = 0;
					jelolhet = true;
					ujjat_lehejezhet = true;
					if (jatekoslapka > 1)
						mozgathathat = true;
					passzolhat = false;
					akcio = 1;
					ebben_a_korben_lerakott_X = -1;
					ebben_a_korben_lerakott_X = -1;
				}
				else
				{
					cout << "Nem Passzolhat!" << "Folytatashoz uss le egy billentyut..." << endl;
					cin.ignore();
				}
				continue;
			}
		}
		/*################################modóok###############################################################*/
		else {
			//mode 1 jelölés->mode0
			if (mode == 1) {
				cout << "Jeloleshez - Enter";
				switch (_getch()) {
				case KEY_UP:
					lepes_UP();
					break;
				case KEY_DOWN:
					lepes_DOWN();
					break;
				case KEY_LEFT:
					lepes_LEFT();
					break;
				case KEY_RIGHT:
					lepes_RIGHT();
					break;
				case KEY_ENTER:
					if (main_array[cursor_Y][cursor_X][cursor_y][cursor_x] != '.') {
						ervenytelen_lepes("ervenytelen");
						continue;
					}
					else {
						main_array[cursor_Y][cursor_X][cursor_y][cursor_x] = Players[player];
						ellenorzes();
						if (win)
						{
							cout << endl << Players[player] << " NYERT!!";
						}
						mode = 0;
						jelolhet = false;
						ebben_a_korben_lerakott_X = cursor_X;
						ebben_a_korben_lerakott_Y = cursor_Y;
					}
					break;
				}
			}
			//mode 2 új lehelyezése->mode0
			if (mode == 2) {
				switch (_getch()) {
				case KEY_UP:
					lepes_UP_dupla();
					break;
				case KEY_DOWN:
					lepes_DOWN_dupla();
					break;
				case KEY_LEFT:
					lepes_LEFT_dupla();
					break;
				case KEY_RIGHT:
					lepes_RIGHT_dupla();
					break;
				case KEY_ENTER:
					/*bővítés*/
					bovit();
					bool err = false;
					bool o = oldalszomszed();
					/* Megnézzük hogy van e már valami ott ahova új lapot akar lerakni */
					if (cursor_Y >= 0 && cursor_Y < row && cursor_X >= 0 && cursor_X < col) {
						if (main_array[cursor_Y][cursor_X][0][0] != '-') {
							err = true;
						}

					}
					if (err) {
						ervenytelen_lepes("Mar foglalt a lapka");
					}
					else if (!o) {
						ervenytelen_lepes("Nincs oldalszomszed");
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
					lepes_UP_dupla();
					break;
				case KEY_DOWN:
					lepes_DOWN_dupla();
					break;
				case KEY_LEFT:
					lepes_LEFT_dupla();
					break;
				case KEY_RIGHT:
					lepes_RIGHT_dupla();
					break;
				case KEY_ENTER:
					/*bővítés*/
					bovit();
					bool err = false;

					/*A mozgatandóhoz sorba vagy  oszlopba egyezik e */
					if (mozgatas_elotti_X == cursor_X || mozgatas_elotti_Y == cursor_Y || mozgatas_elotti_Y - cursor_Y == mozgatas_elotti_X-cursor_X || mozgatas_elotti_Y - cursor_Y == -(mozgatas_elotti_X - cursor_X)) {
						
							//ervenytelen_lepes("OK");
						
					}else {
						ervenytelen_lepes("Nem jó sorban/oszlopba lett mozgatva");
						continue;
					}

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
						ervenytelen_lepes("Ervenytelen");
						continue;
					}
					else if (!oldalszomszed) {
						ervenytelen_lepes("Nincs oldalszomszed!");
						continue;
					}
					else {

						for (int k = 0; k < 2; k++)
						{
							for (int l = 0; l < 2; l++)
							{
								main_array[cursor_Y][cursor_X][k][l] = mozgatando[k][l];

							}
						}
						//kijelzes();
						bool p = palyaszakadas();
						if (p) {
							ervenytelen_lepes("Palyaszakadas");
							for (int k = 0; k < 2; k++)
							{
								for (int l = 0; l < 2; l++)
								{
									main_array[mozgatas_elotti_Y][mozgatas_elotti_X][k][l] = mozgatando[k][l];
								}
							}
							for (int k = 0; k < 2; k++)
							{
								for (int l = 0; l < 2; l++)
								{
									main_array[cursor_Y][cursor_X][k][l] = '-';
								}
							}
							cursor_Y = mozgatas_elotti_Y;
							cursor_X = mozgatas_elotti_X;
							mode = 3;
							continue;

						}
						ellenorzes();
						if (win)
						{
							cout << endl << Players[player] << " NYERT!!";
						}
						mode = 0;
						jelenleg_mozgat = false;
						break;
					}
				}

			}
			//mode 3 tile áthelyezése honnan->mode4
			if (mode == 3) {
				switch (_getch()) {
				case KEY_UP:
					lepes_UP_dupla(1);
					break;
				case KEY_DOWN:
					lepes_DOWN_dupla(1);
					break;
				case KEY_LEFT:
					lepes_LEFT_dupla(1);
					break;
				case KEY_RIGHT:
					lepes_RIGHT_dupla(1);
					break;
				case KEY_ENTER:
					oldalszomszed_szam = 0;
					/*Ellenörizzük hogy van e 4 szomszédos lapka - Körbe van e zárva*/
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

					/* Ellenőrizzük hogy van e pályaszakadás ha kivesszük */
					bool p = false;

					if (oldalszomszed_szam == 4) {
						ervenytelen_lepes("Ervenytelen korbe van zarva");
						continue;
					}
					else if (main_array[cursor_Y][cursor_X][cursor_y][cursor_x] == '-') {
						ervenytelen_lepes("A lapka üres");
						continue;
					}
					
					else if (ebben_a_korben_lerakott_X!=cursor_X || ebben_a_korben_lerakott_Y!=cursor_Y) {
						mozgatas_elotti_X = cursor_X;
						mozgatas_elotti_Y = cursor_Y;
						ebben_a_korben_lerakott_X = -1;
						ebben_a_korben_lerakott_X = -1;
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
					else {
						ervenytelen_lepes("Ebben a korben lerakott nem mozgathato");
					}
					break;
				}
			}

		} 
	}
	if (!win) {
		cout << "Dontetlen!";
		cin.ignore();
	}
	/*felszabadítás*/
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			for (int k = 0; k < 2; k++)
			{
				delete[] main_array[i][j][k];
			}
			delete[] main_array[i][j];
		}
		delete[] main_array[i];
	}
	delete[] main_array;
	delete[] mozgatando;
	return 0;

}
