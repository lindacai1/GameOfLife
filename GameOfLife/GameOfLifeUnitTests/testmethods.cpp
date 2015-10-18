#include "testmethods.h"
#include <iostream>

Board makeSample1() {
	Board board;
	board.addLivecell(0, 1);
	board.addLivecell(1, 2);
	board.addLivecell(2, 0);
	board.addLivecell(2, 1);
	board.addLivecell(2, 2);
	board.addLivecell(-2000000000000, -2000000000000);
	board.addLivecell(-2000000000001, -2000000000001);
	board.addLivecell(-2000000000001, -2000000000000);
	return board;
}

Board makeSample2() {
	Board board;
	board.addLivecell(0, 1);
	board.addLivecell(1, 2);
	board.addLivecell(2, 1);
	board.addLivecell(2, 2);
	board.addLivecell(3, 1);
	board.addLivecell(-2000000000000, -2000000000000);
	board.addLivecell(-2000000000001, -2000000000001);
	board.addLivecell(-2000000000001, -2000000000000);
	board.addLivecell(-2000000000000, -2000000000001);
	return board;
}

void printBoard(Board& board)
{
	for (const std::pair<int64_t, int64_t>& column : board.livecells) {
		int64_t x = column.first;
		int64_t y = column.second;
		std::cout << x << " " << y << std::endl;
	}
}