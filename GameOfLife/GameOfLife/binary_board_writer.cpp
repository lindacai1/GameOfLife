#include "binary_board_writer.h"

/* 
 * Write live cells in binary file where each segment is of the
 * format: x_value (8 bytes), length_of_y_values (8 bytes), y values (8*#y_values bytes)
 */

BinaryBoardWriter::BinaryBoardWriter(std::ostream& out) : out(out), isStart(true), curX(-1), len(0), lenPos(-1) {
}

BinaryBoardWriter::~BinaryBoardWriter() {
	finishSegment();
}

// add must be called with (x, y) pairs in-order (row, column)
void BinaryBoardWriter::add(int64_t x, int64_t y) {
	if (isStart) {
		startSegment(x);
		isStart = false;
	} else if (x != curX) {
		finishSegment();
		startSegment(x);
	}
	out.write(reinterpret_cast<const char*> (&y), sizeof(y));
	++len;
}

void BinaryBoardWriter::startSegment(int64_t x) {
	out.write(reinterpret_cast<const char*> (&x), sizeof(x));
	lenPos = out.tellp();
	int64_t initLen = (int64_t)0;
	out.write(reinterpret_cast<const char*> (&initLen), sizeof(int64_t));
	curX = x;
}

void BinaryBoardWriter::finishSegment() {
	int64_t curPos = out.tellp();
	out.seekp(lenPos);
	out.write(reinterpret_cast<const char*> (&len), sizeof(int64_t));
	out.seekp(curPos);
	len = 0;
}