/*
	https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life

	WISH I FOUND THIS OUT SOONER:
		In Conway's game of life, the entire matrix should be looped over only once: all changes are done simultaneously.

		This, in effect, would cause the need for two matrices: the old state and the new one

		- https://stackoverflow.com/questions/38660983/algorithms-extreme-specifics-about-conways-game-of-life
*/

#include <iostream>
#include <iomanip>
#include <random>
#include <Windows.h>
#include <fstream>
#include <filesystem>

const int boardX = 48;
const int boardY = 48;

const char aliveCell = '#';
const char deadCell = ' ';

char newBoard[boardY][boardX] = {
	"                                 ",
	"                                 ",
	"                                 ",
	"                                 ",
	"                                 ",
	"                                 ",
	"                                 ",
	"                                 ",
	"                                 ",
	"                                 ",
	"                                 ",
	"                                 ",
	"                                 ",
	"                                 ",
	"                                 ",
	"                                 ",
	"                                 ",
	"                                 ",
	"                                 ",
	"                                 ",
	"                                 ",
	"                                 ",
	"                                 ",
	"                                 ",
	"                                 ",
	"                                 ",
	"                                 ",
	"                                 ",
	"                                 ",
	"                                 ",
	"                                 ",
	"                                 ",
	"                                 ",
	"                                 ",
	"                                 ",
	"                                 ",
	"                                 ",
	"                                 ",
	"                                 ",
	"                                 ",
	"                                 ",
	"                                 ",
	"                                 ",
	"                                 ",
	"                                 ",
	"                                 ",
	"                                 ",
	"                                 ",
};							          

char oldBoard[boardY][boardX] = {};

void showCursor(bool flag) {
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = flag;
	SetConsoleCursorInfo(out, &cursorInfo);
}

void generateStartingCells(int CHANCE) {
	int index = 0;
	srand(CHANCE + time(0));
	for (int y = 0; y < boardY; y++) {
		for (int x = 0; x < boardX; x++) {
			int randnum = rand() % CHANCE; // generates num between 0 and CHANCE

			if (randnum == CHANCE -1) { // can be any num between 0 and chance
				newBoard[y][x] = aliveCell;
			}
		}
	}
}

/*

		1 2 3 4 5
	   1
	   2  - - -
	   3  - + -
	   4  - - -
	   5

	   cell in question = 3,3

											  coords (X, Y switched up)
	   top left neighbour = x - 1 and y -1    (2,2)
	   top neighbour = x, y - 1				  (2,3)
	   top right neighbour = x + 1, y - 1	  (2,4)
	   left neighbour = x - 1, y			  ()
	   right neighbour = x + 1, y
	   bottom left neighbour = x - 1, y + 1
	   bottom neighbour = x, y + 1
	   bottom right neighbour = x + 1, y + 1

*/

int countNeighbours(int y, int x, char board[boardY][boardX]) {

	int num = 0;

	// EFFICIENT METHOD, NOT MINE
	for (int i = -1; i < 2; i++) {
		for (int j = -1; j < 2; j++) {

			int row = (y + i + boardY) % boardY;
			int col = (x + j + boardX) % boardX;

			if (board[row][col] == aliveCell) {
				num++;
			} // this counts cell in question, so ... see if statement outside of for loop
		}
	}
	if (board[y][x] == aliveCell && num > 0) {
		num--;
	}

	// MY METHOD, LONG AND DRAWN OUT
	/*
	if (!(y == 0 && x == 0) && (y < (boardY-1) && x < (boardY-1))) { // centre of board
		if (board[y - 1][x - 1] == aliveCell) { num++; }
		if (board[y - 1][x] == aliveCell) { num++; }
		if (board[y - 1][x + 1] == aliveCell) { num++; }
		if (board[y][x - 1] == aliveCell) { num++; }
		if (board[y][x + 1] == aliveCell) { num++; }
		if (board[y + 1][x - 1] == aliveCell) { num++; }
		if (board[y + 1][x] == aliveCell) { num++; }
		if (board[y + 1][x + 1] == aliveCell) { num++; }
	}

	// BORDERS

	else if (y == 0 || (x > 0 && x < boardX -1)) { // top border
		if (board[y][x - 1] == aliveCell) { num++; }
		if (board[y][x + 1] == aliveCell) { num++; }
		if (board[y + 1][x - 1] == aliveCell) { num++; }
		if (board[y + 1][x] == aliveCell) { num++; }
		if (board[y + 1][x + 1] == aliveCell) { num++; }
	}
	else if (x == 0 || (y > 0 && y < boardY -1)) { // left border
		if (board[y - 1][x] == aliveCell) { num++; }
		if (board[y - 1][x + 1] == aliveCell) { num++; }
		if (board[y][x + 1] == aliveCell) { num++; }
		if (board[y + 1][x] == aliveCell) { num++; }
		if (board[y + 1][x + 1] == aliveCell) { num++; }
	}
	else if (!(y < boardY-1) || x < boardX-1) { // bottom border
		if (board[y - 1][x - 1] == aliveCell) { num++; }
		if (board[y - 1][x] == aliveCell) { num++; }
		if (board[y - 1][x + 1] == aliveCell) { num++; }
		if (board[y][x - 1] == aliveCell) { num++; }
		if (board[y][x + 1] == aliveCell) { num++; }
	}
	else if (!(x < boardX - 1) || y < boardY - 1) { // right border
		if (board[y - 1][x - 1] == aliveCell) { num++; }
		if (board[y - 1][x] == aliveCell) { num++; }
		if (board[y][x - 1] == aliveCell) { num++; }
		if (board[y + 1][x - 1] == aliveCell) { num++; }
		if (board[y + 1][x] == aliveCell) { num++; }
	}
	
	// CORNERS

	else if (y == 0 && x == 0) { // top left
		if (board[y][x + 1] == aliveCell) { num++; }
		if (board[y + 1][x] == aliveCell) { num++; }
		if (board[y + 1][x + 1] == aliveCell) { num++; }
	}
	else if (!(x < boardX-1) && !(y <  boardY-1)) { // bottom right
		if (board[y - 1][x - 1] == aliveCell) { num++; }
		if (board[y - 1][x] == aliveCell) { num++; }
		if (board[y][x - 1] == aliveCell) { num++; }
	}
	else if (!(x < boardX - 1) && y == 0) { // top right
		if (board[y][x - 1] == aliveCell) { num++; }
		if (board[y + 1][x - 1] == aliveCell) { num++; }
		if (board[y + 1][x] == aliveCell) { num++; }
	}
	
	else if (x == 0 && !(y < boardY - 1)) { // bottom left
		if (board[y - 1][x] == aliveCell) { num++; }
		if (board[y - 1][x + 1] == aliveCell) { num++; }
		if (board[y][x + 1] == aliveCell) { num++; }
	}
	*/
	return num;
}

void processAliveCell(int y, int x, char board[boardY][boardX])
{
	int neighbours = countNeighbours(y, x, board);

	if (neighbours <= 1 || neighbours >= 4) {
		newBoard[y][x] = deadCell;
	}
}

void processDeadCell(int y, int x, char board[boardY][boardX])
{
	int neighbours = countNeighbours(y, x, board);
	if (neighbours == 3) {
		newBoard[y][x] = aliveCell;
	}
}

void newGeneration(char board[boardY][boardX]) {
	for (int y = 0; y < boardY; y++) {
		for (int x = 0; x < boardX; x++) {
			if (newBoard[y][x] == deadCell) { processDeadCell(y, x, board); }
			if (newBoard[y][x] == aliveCell) { processAliveCell(y, x, board); }
		}
	}
}

void countTotalAliveCells() {
	int num = 0;
	for (int y = 0; y < boardY; y++) {
		for (int x = 0; x < boardX; x++) {
			if (newBoard[y][x] == aliveCell) {
				num++;
			}
		}
	}
	std::cout << "Total Alive Cells: " << num << std::endl;
}

void printBoard() {
	for (int y = 0; y < boardY; y++) {
		for (int x = 0; x < boardX; x++) {
			std::cout << std::setw(2) << newBoard[y][x];
		}
		std::cout << std::endl;
	}
}

void updateOldBoard() {
	for (int y = 0; y < boardY; y++) {
		for (int x = 0; x < boardX; x++) {
			oldBoard[y][x] = newBoard[y][x];
		}
	}
}

void updateGame(int generation) {
	updateOldBoard();
	showCursor(false);
	system("cls");
	std::cout << "Generation: " << generation << "\t";
	countTotalAliveCells();
	printBoard();
	newGeneration(oldBoard);
}

int main() {
	std::cout << "Enter the 1 in n chance for a cell to be generated in the first generation: ";
	int num;
	std::cin >> num;
	generateStartingCells(num);

	showCursor(false);
	int generation = 1;
	std::cout << "Generation: " << generation << "\t";
	countTotalAliveCells();
	printBoard();
	Sleep(2000);
	while (!GetAsyncKeyState(VK_ESCAPE)) {
		updateGame(generation);
		generation++;
	}
	return 0;
}