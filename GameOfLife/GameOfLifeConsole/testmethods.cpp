#include "testmethods.h"
#include <iostream>
#include <istream>
#include <fstream>
#include <random>

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
	board.addLivecell(1, 0);
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

Board makeLargeBoard() {
	Board lboard;
	std::mt19937 randgen(0);
	for (int i = 0; i < 4000000; i++) {
		lboard.addLivecell(randgen(), randgen());
	}
	return lboard;
}

void makeLargeInputFile() {
	std::mt19937 randgen(0);
	std::ofstream output("IOFiles/largeInput1.txt");
	Board lboard;
	for (int i = 0; i < 1000000; i++) {
		lboard.addLivecell(randgen(), randgen());
	}
	for (auto& cell : lboard.livecells) {
		output << cell.first << " " << cell.second << "\n";
	}
}

void printBoard(Board& board)
{
	for (const std::pair<int64_t, int64_t>& column : board.livecells) {
		int64_t x = column.first;
		int64_t y = column.second;
		std::cout << x << " " << y << std::endl;
	}
}