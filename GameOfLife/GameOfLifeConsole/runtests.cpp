#include "testmethods.h"
#include "board.h"
#include "board_io.h"
#include <iostream>
#include <string>
#include <limits>

void assert(bool b, std::string s = "") {
	if (!b) {
		std::cout << "FAIL: " << s << std::endl;
	}
}

void testCorners1() {
	Board board1;
	board1.addLivecell(0, 0);
	board1.addLivecell(0, 1);
	board1.addLivecell(1, 0);

	Board board2;
	board2.addLivecell(0, 0);
	board2.addLivecell(0, 1);
	board2.addLivecell(1, 0);
	board2.addLivecell(1, 1);
	assert(board1.nextIteration() == board2, "testCorners1 - next iteration != expected board");

}

void testCorners2() {

}

// Wrapping not implemented so test whether livecells overflow when reaching min/max boundary
// Signed overflow is undefined so cannot be relied on if wanted to implement wrapping
void testOverflowBoundary1() {
	int64_t imin = std::numeric_limits<int64_t>::min();
	int64_t imax = std::numeric_limits<int64_t>::max();
	Board board1;
	board1.addLivecell(imax, 0);
	board1.addLivecell(imax, 1);
	board1.addLivecell(imin, 1);

	board1 = board1.nextIteration();
	assert(board1.livecells.find(std::make_pair(imin, 0)) == board1.livecells.end(), "testOverflowBoundary1 - overflow occurs");
	assert(board1.livecells.empty());
}

void testInput1() {
	std::ifstream input("IOFiles/input1.txt");
	Board iboard = BoardIO::read(input);
	input.close();
	Board board = makeSample1();
	assert(iboard == board, "testInput1 - read input board != expected board");
}

void testOutput1() {
	Board board1 = makeSample1().nextIteration();
	assert(board1 == makeSample2(), "testOutput1 - next iteration != expected board");
	{std::ofstream output("IOFiles/output1.txt");
	BoardIO::write(board1, output); }
	Board board2;
	{std::ifstream input("IOFiles/output1.txt");
	board2 = BoardIO::read(input); }
	assert(board1 == board2, "testOutput1 - output board != expected board");
}

void runAllTests() {
	testCorners1();
	testCorners2();
	testOverflowBoundary1();
	testInput1();
	testOutput1();
}
