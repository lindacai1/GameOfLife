#include "testmethods.h"
#include "board.h"
#include "board_io.h"
#include "binary_board_writer.h"
#include <iostream>
#include <string>
#include <limits>
#include <chrono>

void assert(bool b, std::string s = "") {
	if (!b) {
		std::cout << "FAIL: " << s << std::endl;
	}
}

void testMiddle1() {
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

// Upper left corner
void testCorners1() {
	int64_t imin = std::numeric_limits<int64_t>::min();
	int64_t imax = std::numeric_limits<int64_t>::max();

	Board board1;
	board1.addLivecell(imin, imax);
	board1.addLivecell(imin + 1, imax);
	board1.addLivecell(imin, imax - 1);

	Board board2 = board1.nextIteration();
	board1.addLivecell(imin + 1, imax - 1);
	assert(board1 == board2, "testCorners1");
}

// Upper right corner
void testCorners2() {
	int64_t imax = std::numeric_limits<int64_t>::max();

	Board board1;
	board1.addLivecell(imax, imax);
	board1.addLivecell(imax, imax - 1);
	board1.addLivecell(imax - 1, imax);

	Board board2 = board1.nextIteration();
	board1.addLivecell(imax - 1, imax - 1);
	assert(board1 == board2, "testCorners2");
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

void testBinaryBoardWrite1() {
	std::ofstream output("IOFiles/binaryBoard1.txt");
	BinaryBoardWriter bbw(output);
	bbw.add(0, 1);
	bbw.add(0, 2);
	bbw.add(0, 3);
	bbw.add(1, 1);
}

void testDiskAlgorithm1() {
	makeLargeInputFile();
}

void benchmarkParallel() {
	Board board = makeLargeBoard();
	std::string s = "Next iteration using OpenMP... ";
	std::cout << s;
	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
	board.nextIteration();
	std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
	auto duration = (std::chrono::duration_cast<std::chrono::milliseconds>)(t2 - t1).count();
	std::cout << duration << " milliseconds" << std::endl;
	{std::ofstream output("IOFiles/parallelBenchmark.txt", std::ofstream::app);
	output << s << duration << " milliseconds" << "\n"; }
}

void benchmarkSerial() {
	Board board = makeLargeBoard();
	std::string s = "Next iteration without OpenMP... ";
	std::cout << s;
	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
	board.nextIterationSerial();
	std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
	auto duration = (std::chrono::duration_cast<std::chrono::milliseconds>)(t2 - t1).count();
	std::cout << duration << " milliseconds" << std::endl;
	{std::ofstream output("IOFiles/serialBenchmark.txt", std::ofstream::app);
	output << s << duration << " milliseconds" << "\n"; }
}

void runAllTests() {
	testMiddle1();
	testCorners1();
	testCorners2();
	testOverflowBoundary1();
	testInput1();
	testOutput1();
	testBinaryBoardWrite1();
	//testDiskAlgorithm1();
}

void runAllBenchmarks() {
	{std::ofstream output("IOFiles/serialBenchmark.txt");
	output << "Starting serial benchmark test on Board with 4 million cells" << "\n"; }
	for (int i = 0; i < 10; i++) {
		benchmarkSerial();
	}
	{std::ofstream output("IOFiles/parallelBenchmark.txt");
	output << "Starting parallel benchmark test on Board with 4 million cells" << "\n";
	}
	for (int i = 0; i < 10; i++) {
		benchmarkParallel();
	}
}
