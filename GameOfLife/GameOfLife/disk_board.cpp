#include "disk_board.h"
#include <limits>
using std::unique_ptr;
using std::istream;
using std::iostream;

/* DiskBoard used is to compute the next generation of cells in Game of Life
 * Input: Binary stream of cells in the format x : # of y values : y values (each value is 8 bytes)
 * Output: Binary stream of cells in the same format
 * Algorithm: Read in at least 5 columns at a time (as column buffers) and for each cell (x, y) consider it's
 * neighbor columns x - 1, x + 1. It considers the smallest column first and uses RowIterator to tell what y
 * values check. The boardbuffer holds on to enough column headers to evaluate the current column.
 * We use processCol to determine whether the cells in the column should be alive in the next iteration.
 * Entire columns will be processed in-order and output in-order to 
 * guarantee the outputted binary stream will be ordered and valid.
 */

struct ColumnInfo {
	int64_t x;
	int64_t startPos;
	int64_t len;
	bool isValid; // True if this is a valid column

	ColumnInfo() : x(0), startPos(0), len(0), isValid(false) {
	}

	// Get position of next column
	int64_t getNextPos() {
		return startPos + (sizeof(int64_t)) * (2 + len);
	}

	// Get position of next y-value using offset
	int64_t getOffset(int64_t index) {
		return startPos + (sizeof(int64_t)) * (2 + index);
	}

	// Set values using stream and position in stream
	void setFromStream(istream& input, int64_t pos) {
		input.clear();
		input.seekg(pos);
		startPos = pos;
		input.read(reinterpret_cast<char*> (&x), sizeof(x));
		if (!input) {
			isValid = false;
			return;
		}
		input.read(reinterpret_cast<char*> (&len), sizeof(len));
		if (!input) {
			isValid = false;
			return;
		}
		isValid = true;
	}
};

// Holds on to a number of y values for each column
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
			input.clear();
			input.seekg(ci.getOffset(toLoad));
			++toLoad;
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
			input.clear();
			input.seekg(ci.getOffset(0));
			input.read(reinterpret_cast<char*>(&y), sizeof(y));
		}
	}
};

// Tells what row to look at next for a column
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
			input.clear();
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

	void loadY(int64_t y) {
		for (auto& buf : buffer) {
			buf.loadY(input, y);
		}
	}

private:
	istream& input;
	std::vector<ColumnBuffer> buffer; // Unless the board is empty, has >= 1 entry
};

// Tells us which columns we need to examine
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

	BinaryBoardWriter writer(*output);
	BoardBuffer buffer(*livecells);

	ColumnIterator colIter(*livecells);
	int64_t outCol;
	while (colIter.getNextColToCheck(outCol)) {
		processCol(outCol, buffer, writer);
	}

	return DiskBoard(std::move(output));
}

// Game of Life cell logic
bool nextItAlive(int64_t x, int64_t y, BoardBuffer& reader) {
	int numAlive = 0;
	for (int64_t x1 = x - 1; x1 <= x + 1; ++x1) {
		for (int64_t y1 = y - 1; y1 <= y + 1; ++y1) {
			if (x1 == x && y1 == y) {
				continue;
			}

			if (reader.checkAlive(x1, y1)) {
				++numAlive;
			}
		}
	}

	if (numAlive == 3) {
		return true;
	}

	if (numAlive == 2 && reader.checkAlive(x, y)) {
		return true;
	}

	return false;
}

// Process a column (x) : look at neighbors of row (y) and check whether cells should be alive
void DiskBoard::processCol(int64_t col, BoardBuffer& reader, BinaryBoardWriter& writer) {
	reader.setCheckedCol(col);
	RowIterator rowIter = reader.getRowsForColumn(col);
	int64_t row;
	while (rowIter.next_y(row)) {
		reader.loadY(row);
		if (nextItAlive(col, row, reader)) {
			writer.add(col, row);
		}
	}
}