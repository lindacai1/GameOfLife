#include "board.h"
using std::vector;
using std::pair;
using std::set;

#define DEBUG_MESSAGE(x)// std::cout << x << std::endl

// Add live cells by row, column
// Sorted map, sorted set of y values
void Board::addLivecell(int64_t x, int64_t y) 
{
	livecells.insert(std::make_pair(x, y));
}

Board Board::nextIteration() 
{
	Board next;
	/* For each live cell, calculate the next state of itself and all of its neighbors
	* and add to the next iteration
	*/

	// Convert set to vector so OpenMP can divide into chunks
	std::vector<pair<int64_t, int64_t>> vlivecells(livecells.begin(), livecells.end());

	for (size_t i = 0; i < vlivecells.size(); ++i) 
	{
		pair<int64_t, int64_t> cell = vlivecells[i];
		int64_t x = cell.first;
		int64_t y = cell.second;
		processCell(next, x, y);
	}
	return next;
}

int Board::numLiveNeighbors(int64_t x, int64_t y) {

	int num = 0;
	int64_t minX, maxX, minY, maxY;
	getBounds(x, minX, maxX);
	getBounds(y, minY, maxY);

	for (int64_t i = minX; i <= maxX; i++) {
		for (int64_t j = minY; j <= maxY; j++) {
			if (i == x && y == j) {
				continue;
			}
			if (isAliveCell(i, j)) {
				num++;
			}
		}
	}
	return num;
}

/*
* Check cell (x,y) and all neighbors whether they should be alive or dead in the next iteration
* and if so, add to the argument board's livecells
*/
void Board::processCell(Board& board, int64_t x, int64_t y) {
	int64_t minX, maxX, minY, maxY;
	getBounds(x, minX, maxX);
	getBounds(y, minY, maxY);

	for (int64_t i = minX; i <= maxX; i++) {
		for (int64_t j = minY; j <= maxY; j++) {
			int numLiveN = numLiveNeighbors(i, j);
			if (numLiveN == 3) {
				board.addLivecell(i, j);
				continue;
			}
			if (isAliveCell(i, j)) {
				if (numLiveN == 2) {
					board.addLivecell(i, j);
				}
			}
		}
	}
}

void Board::getBounds(int64_t index, int64_t& start, int64_t& end) {
	start = index == std::numeric_limits<int64_t>::min() ? index : index - 1;
	end = index == std::numeric_limits<int64_t>::max() ? index : index + 1;
}

bool Board::isAliveCell(int64_t x, int64_t y) {
	auto it = livecells.find(std::make_pair(x, y));
	return it != livecells.end();
}

// Return whether this board equals other board
bool Board::operator==(const Board &other) const {
	return this->livecells == other.livecells;
}
