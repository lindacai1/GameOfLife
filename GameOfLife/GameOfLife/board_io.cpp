#include "board_io.h"

Board BoardIO::read(std::istream& in) {
	Board board;
	int64_t x, y;
	while (in >> x >> y) {
		board.addLivecell(x, y);
	}
	return board;
}
void BoardIO::write(const Board& board, std::ostream& out) {
	for (auto& cell : board.livecells) {
		out << cell.first << " " << cell.second << "\n";
	}
}