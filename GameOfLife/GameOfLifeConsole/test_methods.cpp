#include "test_methods.h"
#include "binary_board_writer.h"
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

Board makeLargeRandomBoard() {
	Board lboard;
	std::mt19937 randgen(0);
	for (int i = 0; i < 4000000; i++) {
		lboard.addLivecell(randgen(), randgen());
	}
	return lboard;
}

Board makeLargeAdjacentBoard() {
	Board lboard;
	std::mt19937 randgen(0);
	int j = 0;
	for (int i = 0; i < 4000000; i++) {
		if (i == 1000000 || i == 2000000 || i == 3000000) {
			j++;
		}
		lboard.addLivecell(i, j);
	}
	return lboard;
}


/* Converts a binary input stream/file to a string
 * For testing our DiskBoard and BinaryBoardWriter classes
 */
std::string readBinaryBoard(std::istream& in) {
	char c;
	std::string s;
	while (in.get(c)) {
		s += c;
	}
	return s;
}

std::string encode(const std::vector<int64_t>& expected) {
	std::string s;
	for (int64_t num : expected) {
		s.append(reinterpret_cast<const char*> (&num), sizeof(num));
	}
	return s;
}


void printBoard(Board& board) {
	for (const std::pair<int64_t, int64_t>& column : board.livecells) {
		int64_t x = column.first;
		int64_t y = column.second;
		std::cout << x << " " << y << std::endl;
	}
}