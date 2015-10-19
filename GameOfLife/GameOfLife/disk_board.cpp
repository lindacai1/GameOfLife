#include "disk_board.h"
#include <limits>
using std::unique_ptr;
using std::istream;
using std::iostream;

//unique_ptr<istream> livecells;

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

	int64_t getOffset(int64_t index) {
		return startPos + (sizeof(int64_t)) * (2 + index);
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


class ColumnBuffer {
public:
	ColumnInfo ci;
	std::vector<int64_t> yValues;
	int64_t toLoad = 0;

	ColumnBuffer(ColumnInfo ci) : ci(ci) {
	}

	void loadY(istream& input, int64_t y) {
		// TODO overflow
		while (yValues.size() == 0 || yValues.back() < y + 5) {
			if (toLoad == ci.len) {
				return;
			}
			input.seekg(ci.getOffset(toLoad));
			int64_t read;
			if (input.read(reinterpret_cast<char*>(&read), sizeof(read))) {
				yValues.push_back(read);
			} else {
				return;
			}
		}

		while (yValues.size() > 10) {
			yValues.erase(yValues.begin());
		}
	}

	void resetBuffer() {
		toLoad = 0;
		yValues.clear();
	}
};

DiskBoard::DiskBoard(unique_ptr<istream> data) : livecells(std::move(data)) {
}

// Need to specify explicitly because VS2013 doesn't provide default move constructor
DiskBoard::DiskBoard(DiskBoard&& x) : livecells(std::move(x.livecells)) {
}

struct RowPosition {
	ColumnInfo ci;
	int64_t y;
	int64_t index; // index in ci

	RowPosition(ColumnInfo ci, istream& input) : y(0), index(0), ci(ci) {
		if (ci.len > 0) {
			input.seekg(ci.getOffset(0));
			input.read(reinterpret_cast<char*>(&y), sizeof(y));
		}
	}
};

class RowIterator {
	std::vector<RowPosition> positions;
	istream& input;

	bool getMinY(int64_t& min_y, int& row_index) {
		bool found = false;
		for (size_t i = 0; i < positions.size(); ++i) {
			auto& pos = positions[i];
			if (pos.index < pos.ci.len) {
				if (found) {
					if (pos.y < min_y) {
						min_y = pos.y;
						row_index = i;
					}
				} else {
					found = true;
					min_y = pos.y;
					row_index = i;
				}
			}
		}

		return found;
	}

	void nextIndex(int index) {
		auto& pos = positions[index];
		++pos.index;
		if (pos.index < pos.ci.len) {
			input.seekg(pos.ci.getOffset(pos.index));
			input.read(reinterpret_cast<char*>(&pos.y), sizeof(pos.y));
		}
	}
public:
	RowIterator(istream& input) : input(input) {}
	void addColumnInfo(ColumnInfo ci) {
		positions.push_back(RowPosition(ci, input));
	}

	int64_t min_next = std::numeric_limits<int64_t>::min();
	bool next_y(int64_t& y) {
		int64_t min_y;
		int row_index;
		while (getMinY(min_y, row_index)) {
			if (min_y - 1 >= min_next) {
				y = min_y - 1;
			} else if (min_y >= min_next) {
				y = min_y;
			} else if (min_y + 1 >= min_next) {
				y = min_y + 1;
			} else {
				nextIndex(row_index);
				continue;
			}
			min_next = y + 1;
			return true;
		}

		return false;
	}
};

class BoardBuffer {
public:
	BoardBuffer(istream& input) : input(input) {
		int64_t pos = 0;
		for (int i = 0; i < 5; ++i) {
			ColumnInfo ci;
			ci.setFromStream(input, pos);
			if (!ci.isValid) {
				break;
			}

			buffer.push_back(ColumnBuffer(ci));
			pos = ci.getNextPos();
		}
	}
	RowIterator getRowsForColumn(int64_t col) {
		RowIterator it(input);
		for (auto& buf : buffer) {
			int64_t x = buf.ci.x;
			if (x >= col - 1 && x <= col + 1) {
				it.addColumnInfo(buf.ci);
			}
		}
		return it;
	}

	void setCheckedCol(int64_t col) {
		for (auto& buf : buffer) {
			buf.resetBuffer();
		}

		while (buffer.back().ci.x < col + 5) { // TODO overflow
			ColumnInfo ci;
			ci.setFromStream(input, buffer.back().ci.getNextPos());
			if (!ci.isValid) {
				break;
			}

			buffer.push_back(ColumnBuffer(ci));
		}

		while (buffer.size() > 20) { // TODO reduce this number?
			buffer.erase(buffer.begin());
		}
	}
	bool checkAlive(int64_t x, int64_t y) {
		for (auto& buf : buffer) {
			if (buf.ci.x != x) {
				continue;
			}

			for (auto yVal : buf.yValues) {
				if (y == yVal) {
					return true;
				}
			}

			return false;
		}

		return false;
	}

private:
	istream& input;
	std::vector<ColumnBuffer> buffer; // Unless the board is empty, has >= 1 entry
};

class ColumnIterator {
	ColumnInfo ci;
	istream& input;
	int64_t minNextColToCheck = std::numeric_limits<int64_t>::min();
	bool sawMaxNum = false;

public:
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

/* Our data (livecells) is a sorted binary file of the format: x, length of y values, y values
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
	BoardBuffer buffer(*livecells);

	ColumnIterator colIter(*livecells);
	int64_t outCol;
	while (colIter.getNextColToCheck(outCol)) {
		processCol(outCol, buffer, writer);
	}

	return DiskBoard(std::move(output));
}

void processCol(int64_t col, BoardBuffer reader, BinaryBoardWriter writer) {
	reader.setCheckedCol(col);
	RowIterator rowIter = reader.getRowsForColumn(col);
	int64_t row;
	while (rowIter.next_y(row)) {
		if (reader.checkAlive(col, row)) {
			writer.add(col, row);
		}
	}
}
	
//bool DiskBoard::readBuffer(ColumnBuffer& cb, int64_t pos) {
//	livecells->seekg(pos);
//	cb.startPos = pos;
//	int64_t x;
//	livecells->read(reinterpret_cast<char*> (&x), sizeof(x));
//	if (livecells->eof()) {
//		return false;
//	}
//	cb.x = x;
//
//	int64_t len;
//	livecells->read(reinterpret_cast<char*> (&len), sizeof(len));
//	if (livecells->eof()) {
//		return false;
//	}
//	cb.len = len;
//
//	cb.yPos = livecells->tellg();
//	for (int i = 0; i < 5 && i < len; ++i) {
//		int64_t y;
//		livecells->read(reinterpret_cast<char*> (&y), sizeof(y));
//		if (livecells->eof()) {
//			return false;
//		}
//		cb.bufferedVals.push_back(y);
//	}
//
//	return true;
//}


//DiskBoard::ColumnBuffer::ColumnBuffer() : x(0), startPos(0), len(0), yPos(0) {
//}
//
//int64_t DiskBoard::ColumnBuffer::getNextStartPos() {
//	return startPos + (sizeof(int64_t)) * (2 + len);
//}