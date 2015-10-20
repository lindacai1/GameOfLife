#pragma once

#include "binary_board_writer.h"
#include <istream>
#include <memory>
#include <vector>

class BoardBuffer;
class ColumnBuffer;

class DiskBoard {
public:
	DiskBoard(std::unique_ptr<std::istream> data);
	DiskBoard nextIteration(std::unique_ptr<std::iostream> output);
	DiskBoard(DiskBoard&&);
private:
	std::unique_ptr<std::istream> livecells;
	bool readBuffer(ColumnBuffer& cb, int64_t pos);
	void processCol(int64_t col, BoardBuffer& reader, BinaryBoardWriter& writer);
};