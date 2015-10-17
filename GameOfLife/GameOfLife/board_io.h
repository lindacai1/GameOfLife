#pragma once

#include <istream>
#include <fstream>
#include "board.h"

class BoardIO {
public:
	static Board read(std::istream& in);
	static void write(const Board& board, std::ostream& out);
};
