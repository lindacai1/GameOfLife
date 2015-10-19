#include "disk_board.h"
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
	std::vector<ColumnBuffer> buffers;

	int64_t pos = 0;

	// Make 5 column buffers with <= 5 y-values using data stream
	for (int i = 0; i < 5; ++i) {
		ColumnBuffer cb;
		if (!readBuffer(cb, pos)) {
			break;
		}

		buffers.push_back(cb);
		pos = cb.getNextStartPos();
	}






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

DiskBoard::ColumnBuffer::ColumnBuffer() : x(0), startPos(0), len(0), yPos(0) {
}

int64_t DiskBoard::ColumnBuffer::getNextStartPos() {
	return startPos + (sizeof(int64_t)) * (2 + len);
}