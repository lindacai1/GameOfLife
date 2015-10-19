#include "disk_board.h"
#include "binary_board_writer.h"
#include <limits>
using std::unique_ptr;
using std::istream;
using std::iostream;

//unique_ptr<istream> livecells;

DiskBoard::DiskBoard(unique_ptr<istream> data) : livecells(std::move(data)) {
}

// Need to specify explicitly because VS2013 doesn't provide default move constructor
DiskBoard::DiskBoard(DiskBoard&& x) : livecells(std::move(x.livecells)) {
}

/* Our data (livecells) is a sorted binary file of the format: x, length of y values, y values
 * 
 * Algorithm:
 */
DiskBoard DiskBoard::nextIteration(unique_ptr<iostream> output) {
	//std::vector<ColumnBuffer> buffers;

	//int64_t pos = 0;

	///* Read in a 5x5 area at a time
	// * Make 5 column buffers with <= 5 y-values using data stream 
	// */
	//for (int i = 0; i < 5; ++i) {
	//	ColumnBuffer cb;
	//	if (!readBuffer(cb, pos)) {
	//		break;
	//	}

	//	buffers.push_back(cb);
	//	pos = cb.getNextStartPos();
	//}

	//int64_t currentCol = buffers.front().x;

	//// Get the minimum possible x value of the next column to check
	//int64_t mNextCol = std::numeric_limits<int64_t>::min();

	BinaryBoardWriter writer(*output);
	BinaryBoardReader reader(*livecells);

	ColumnIterator cIter(*livecells);


	return DiskBoard(std::unique_ptr<istream>());
}
	
bool DiskBoard::readBuffer(ColumnBuffer& cb, int64_t pos) {
	livecells->seekg(pos);
	cb.startPos = pos;
	int64_t x;
	livecells->read(reinterpret_cast<char*> (&x), sizeof(x));
	if (livecells->eof()) {
		return false;
	}
	cb.x = x;

	int64_t len;
	livecells->read(reinterpret_cast<char*> (&len), sizeof(len));
	if (livecells->eof()) {
		return false;
	}
	cb.len = len;

	cb.yPos = livecells->tellg();
	for (int i = 0; i < 5 && i < len; ++i) {
		int64_t y;
		livecells->read(reinterpret_cast<char*> (&y), sizeof(y));
		if (livecells->eof()) {
			return false;
		}
		cb.bufferedVals.push_back(y);
	}

	return true;
}

struct ColumnInfo {
	int64_t x;
	int64_t startPos;
	int64_t len;
	bool isValid; // True if this is a valid column

	ColumnInfo() : x(0), startPos(0), len(0), isValid(false) {
	}

	int64_t getNextPos() {
		return startPos + (sizeof(int64_t)) * (2 + len);
	}

	void setFromStream(istream& input, int64_t pos) {
		input.seekg(pos);
		startPos = pos;
		input.read(reinterpret_cast<char*> (&x), sizeof(x));
		if (input.eof()) {
			isValid = false;
			return;
		}
		input.read(reinterpret_cast<char*> (&len), sizeof(len));
		if (input.eof()) {
			isValid = false;
			return;
		}
		isValid = true;
	}
};

class ColumnIterator {
	ColumnInfo ci;
	istream& input;
	int64_t minNextColToCheck = std::numeric_limits<int64_t>::min();
	bool sawMaxNum = false;
	
	ColumnIterator(istream& input) : input(input) {
		ci.setFromStream(input, 0);
	}

	// TODO: Handle overflow
	bool getNextColToCheck(int64_t& outCol) {
		while (ci.isValid) {
			if (sawMaxNum) {
				return false;
			}
			// Check column to the left
			if (ci.x > std::numeric_limits<int64_t>::min()) {
				if (ci.x - 1 >= minNextColToCheck) {
					minNextColToCheck = ci.x;
					outCol = ci.x - 1;
					return true;
				}
			}
			// Check current column
			if (ci.x >= minNextColToCheck) {
				if (ci.x < std::numeric_limits<int64_t>::max()) {
					minNextColToCheck = ci.x + 1;
				} else {
					minNextColToCheck = ci.x;
					sawMaxNum = true;
				}
				outCol = ci.x;
				return true;
			}
			// Check column to the right
			if (ci.x < std::numeric_limits<int64_t>::max()) {
				if (ci.x == std::numeric_limits<int64_t>::max() - 1) {
					sawMaxNum = true;
					minNextColToCheck = ci.x + 1; // +2 would overflow
					outCol = ci.x + 1;
					return true;
				}
				if (ci.x + 1 >= minNextColToCheck) {
					minNextColToCheck = ci.x + 2;
					outCol = ci.x + 1;
					return true;
				}
			}
			ci.setFromStream(input, ci.getNextPos());
		}
		return false;
	}
};

DiskBoard::ColumnBuffer::ColumnBuffer() : x(0), startPos(0), len(0), yPos(0) {
}

int64_t DiskBoard::ColumnBuffer::getNextStartPos() {
	return startPos + (sizeof(int64_t)) * (2 + len);
}