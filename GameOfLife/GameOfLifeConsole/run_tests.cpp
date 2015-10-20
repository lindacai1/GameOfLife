#include "test_methods.h"
#include "board.h"
#include "board_io.h"
#include "binary_board_writer.h"
#include "disk_board.h"
#include <iostream>
#include <string>
#include <limits>
#include <chrono>

void assert(bool b, std::string s = "") {
	if (!b) {
		std::cout << "TEST FAILED: " << s << std::endl;
	}
}

void testBlinker() {
	Board board1;
	board1.addLivecell(0, 0);
	board1.addLivecell(0, 1);
	board1.addLivecell(0, 2);

	Board board2;
	board2.addLivecell(-1, 1);
	board2.addLivecell(0, 1);
	board2.addLivecell(1, 1);
	assert(board1.nextIteration() == board2, "testBlinker - next iteration != expected board");
	assert(board2.nextIteration() == board1, "testBlinker - next iteration != expected board");
}

void testBlinker1() {
	Board board1;
	board1.addLivecell(10, 20);
	board1.addLivecell(10, 21);
	board1.addLivecell(10, 22);

	Board board2;
	board2.addLivecell(9, 21);
	board2.addLivecell(10, 21);
	board2.addLivecell(11, 21);
	assert(board1.nextIteration() == board2, "testBlinker - next iteration != expected board");
	assert(board2.nextIteration() == board1, "testBlinker - next iteration != expected board");
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
	{
		std::ofstream output("IOFiles/output1.txt");
		BoardIO::write(board1, output); 
	}
	Board board2;
	{
		std::ifstream input("IOFiles/output1.txt");
		board2 = BoardIO::read(input); 
	}
	assert(board1 == board2, "testOutput1 - output board != expected board");
}

// Test functionality of BinaryBoardWriter
void testBinaryBoardWrite1() {
	{
		std::ofstream output("IOFiles/binaryBoard.bin", std::ofstream::binary);
		BinaryBoardWriter bbw(output);
		bbw.add(0, 1);
		bbw.add(0, 2);
		bbw.add(0, 3);
		bbw.add(1, 1); 
	}

	std::ifstream input("IOFiles/binaryBoard.bin", std::ifstream::binary);
	std::string sIn = readBinaryBoard(input);
	std::vector<int64_t> v = {0, 3, 1, 2, 3, 1, 1, 1};
	std::string sExpected = encode(v);

	assert(sIn == sExpected, "testBinaryBoardWrite1");
}

void testBinaryBoardWrite2() {
	{
		std::ofstream output("IOFiles/binaryBoard.bin", std::ofstream::binary);
		BinaryBoardWriter bbw(output);
		bbw.add(0, 1);
	}

	std::ifstream input("IOFiles/binaryBoard.bin", std::ifstream::binary);
	std::string sIn = readBinaryBoard(input);
	std::vector<int64_t> v = { 0, 1, 1 };
	std::string sExpected = encode(v);

	assert(sIn == sExpected, "testBinaryBoardWrite2");
}

void testBinaryBoardWrite3() {
	// Empty board
	{
		std::ofstream output("IOFiles/binaryBoard.bin", std::ofstream::binary);
	}

	std::ifstream input("IOFiles/binaryBoard.bin", std::ifstream::binary);
	std::string sIn = readBinaryBoard(input);
	std::vector<int64_t> v;
	std::string sExpected = encode(v);

	assert(sIn == sExpected, "testBinaryBoardWrite3 - should be empty");
}

void testDiskAlgorithm1() {
	{
		std::ofstream output("IOFiles/diskBoardIn1.bin", std::ofstream::binary);
		BinaryBoardWriter bbw(output);
		// Cells must be written in column (x), row (y) order in order for DiskBoard to properly process them
		bbw.add(0, 1);
		bbw.add(0, 2);
		bbw.add(0, 3);
		bbw.add(1, 1);
	}
	auto s = std::make_unique<std::ifstream>("IOFiles/diskBoardIn1.bin", std::ifstream::binary);
	DiskBoard db(std::move(s));

	{
		auto o = std::make_unique<std::fstream>("IOFiles/diskBoardOut1.bin", std::fstream::binary | std::ifstream::in | std::ifstream::out | std::ifstream::trunc);
		db.nextIteration(std::move(o));
	}

	std::ifstream input("IOFiles/diskBoardOut1.bin", std::ifstream::binary);
	std::string sIn = readBinaryBoard(input);
	// Expected values for next iteration
	std::vector<int64_t> v = { -1, 1, 2, 0, 2, 1, 2, 1, 1, 1 };
	std::string sExpected = encode(v);

	assert(sIn == sExpected, "testDiskAlgorithm1");
}

void testDiskAlgorithm2() {
	{
		std::ofstream output("IOFiles/diskBoardIn.bin", std::ofstream::binary);
		BinaryBoardWriter bbw(output);
		bbw.add(100, 100);
		bbw.add(100, 101);
		bbw.add(100, 102);
	}
	auto s = std::make_unique<std::ifstream>("IOFiles/diskBoardIn.bin", std::ifstream::binary);
	DiskBoard db(std::move(s));

	{
		auto o = std::make_unique<std::fstream>("IOFiles/diskBoardOut.bin", std::fstream::binary | std::ifstream::in | std::ifstream::out | std::ifstream::trunc);
		db.nextIteration(std::move(o));
	}

	std::ifstream input("IOFiles/diskBoardOut.bin", std::ifstream::binary);
	std::string sIn = readBinaryBoard(input);
	// Expected values for next iteration
	std::vector<int64_t> v = { 99, 1, 101, 100, 1, 101, 101, 1, 101 };
	std::string sExpected = encode(v);

	assert(sIn == sExpected, "testDiskAlgorithm2");
}

void testDiskAlgorithm3() {
	{
		std::ofstream output("IOFiles/diskBoardIn.bin", std::ofstream::binary);
		BinaryBoardWriter bbw(output);
		bbw.add(-100, 100);
		bbw.add(-100, 101);
		bbw.add(-100, 102);
		bbw.add(-100, 103);
		bbw.add(-100, 104);
		bbw.add(-100, 105);
	}
	auto s = std::make_unique<std::ifstream>("IOFiles/diskBoardIn.bin", std::ifstream::binary);
	DiskBoard db(std::move(s));

	{
		auto o = std::make_unique<std::fstream>("IOFiles/diskBoardOut.bin", std::fstream::binary | std::ifstream::in | std::ifstream::out | std::ifstream::trunc);
		db.nextIteration(std::move(o));
	}

	std::ifstream input("IOFiles/diskBoardOut.bin", std::ifstream::binary);
	std::string sIn = readBinaryBoard(input);
	// Expected values for next iteration
	std::vector<int64_t> v = { 
		-101, 4, 101, 102, 103, 104, 
		-100, 4, 101, 102, 103, 104, 
		-99, 4, 101, 102, 103, 104 
	};
	std::string sExpected = encode(v);

	assert(sIn == sExpected, "testDiskAlgorithm3");
}

void testDiskAlgorithm4() {
	{
		std::ofstream output("IOFiles/diskBoardIn.bin", std::ofstream::binary);
		BinaryBoardWriter bbw(output);
		bbw.add(-2000000000001, -2000000000001);
		bbw.add(-2000000000001, -2000000000000);
		bbw.add(-2000000000000, -2000000000000);
		bbw.add(0, 1);
		bbw.add(1, 2);
		bbw.add(2, 0);
		bbw.add(2, 1);
		bbw.add(2, 2);


	}
	auto s = std::make_unique<std::ifstream>("IOFiles/diskBoardIn.bin", std::ifstream::binary);
	DiskBoard db(std::move(s));

	{
		auto o = std::make_unique<std::fstream>("IOFiles/diskBoardOut.bin", std::fstream::binary | std::ifstream::in | std::ifstream::out | std::ifstream::trunc);
		db.nextIteration(std::move(o));
	}

	std::ifstream input("IOFiles/diskBoardOut.bin", std::ifstream::binary);
	std::string sIn = readBinaryBoard(input);
	// Expected values for next iteration
	std::vector<int64_t> v = {
		-2000000000001, 2, -2000000000001, -2000000000000,
		-2000000000000, 2, -2000000000001, -2000000000000,
		1, 2, 0, 2,
		2, 2, 1, 2,
		3, 1, 1
	};

	std::string sExpected = encode(v);

	assert(sIn == sExpected, "testDiskAlgorithm4");
}

void benchmarkParallel(Board& board) {
	std::string s = "Next iteration using OpenMP... ";
	std::cout << s;
	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
	board.nextIteration();
	std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
	auto duration = (std::chrono::duration_cast<std::chrono::milliseconds>)(t2 - t1).count();
	std::cout << duration << " milliseconds" << std::endl;
	{
		std::ofstream output("IOFiles/parallelBenchmark.txt", std::ofstream::app);
		output << s << duration << " milliseconds" << "\n"; 
	}
}

void benchmarkSerial(Board& board) {
	std::string s = "Next iteration without OpenMP... ";
	std::cout << s;
	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
	board.nextIterationSerial();
	std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
	auto duration = (std::chrono::duration_cast<std::chrono::milliseconds>)(t2 - t1).count();
	std::cout << duration << " milliseconds" << std::endl;
	{
		std::ofstream output("IOFiles/serialBenchmark.txt", std::ofstream::app);
		output << s << duration << " milliseconds" << "\n"; 
	}
}

void runAllTests() {
	testBlinker();
	testBlinker1();
	testMiddle1();
	testCorners1();
	testCorners2();
	testOverflowBoundary1();
	testInput1();
	testOutput1();
	testBinaryBoardWrite1();
	testBinaryBoardWrite2();
	testBinaryBoardWrite3();
	testDiskAlgorithm1();
	testDiskAlgorithm2();
	testDiskAlgorithm3();
	testDiskAlgorithm4();

}

void runAllBenchmarks() {
	Board rBoard = makeLargeRandomBoard();
	Board aBoard = makeLargeAdjacentBoard();
	{
		std::ofstream output2("IOFiles/parallelBenchmark.txt");
		output2 << "Starting parallel benchmark test on Board with 4 million randomly selected cells" << "\n";
	}
	for (int i = 0; i < 10; i++) {
		benchmarkParallel(rBoard);
	}
	{
		std::ofstream output2("IOFiles/parallelBenchmark.txt", std::ofstream::app);
		output2 << "Starting parallel benchmark test on Board with 4 million adjacent cells" << "\n";
	}
	for (int i = 0; i < 10; i++) {
		benchmarkParallel(aBoard);
	}
	{
		std::ofstream output1("IOFiles/serialBenchmark.txt");
		output1 << "Starting serial benchmark test on Board with 4 million randomly selected cells" << "\n";
	}
	for (int i = 0; i < 10; i++) {
		benchmarkSerial(rBoard);
	}
	{
		std::ofstream output1("IOFiles/serialBenchmark.txt", std::ofstream::app);
		output1 << "Starting serial benchmark test on Board with 4 million adjacent cells" << "\n";
	}
	for (int i = 0; i < 10; i++) {
		benchmarkSerial(aBoard);
	}

}
