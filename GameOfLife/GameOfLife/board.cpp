#include "board.h"
#include <omp.h>
#include <limits>
#include <iostream>
using std::vector;
using std::pair;
using std::set;

#define DEBUG_MESSAGE(x)// std::cout << x << std::endl

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
	vector<pair<int64_t, int64_t>> vlivecells(livecells.begin(), livecells.end());
	#pragma omp parallel
	{
		set<pair<int64_t, int64_t>> nlivecells;
		#pragma omp for
		for (int64_t i = 0; i < (int64_t)vlivecells.size(); ++i) {
			pair<int64_t, int64_t> cell = vlivecells[(size_t)i];
			int64_t x = cell.first;
			int64_t y = cell.second;
			processCell(nlivecells, x, y);
		}
		#pragma omp critical
		{
			next.livecells.insert(nlivecells.begin(), nlivecells.end());
		}
	}
	return next;

}

//int Board::numLiveNeighbors(int64_t x, int64_t y) {
//
//	int num = 0;
//	int64_t minX, maxX, minY, maxY;
//	getBounds(x, minX, maxX, 1);
//	getBounds(y, minY, maxY, 1);
//
//	for (int64_t i = minX; i <= maxX; i++) {
//		for (int64_t j = minY; j <= maxY; j++) {
//			if (i == x && y == j) {
//				continue;
//			}
//			if (isAliveCell(i, j)) {
//				num++;
//			}
//		}
//	}
//	return num;
//}

/*
* Check cell (x,y) and all neighbors whether they should be alive or dead in the next iteration
* and if so, add to the argument board's livecells
*/
void Board::processCell(set<pair<int64_t, int64_t>>& nlivecells, int64_t x, int64_t y) {
	int64_t minX, maxX, minY, maxY;
	getBounds(x, minX, maxX, 2);
	getBounds(y, minY, maxY, 2);

	bool chunk[5][5] = { false };
	for (int64_t i = minX; i <= maxX; ++i) {
		for (int64_t j = minY; j <= maxY; ++j) {
			if (isAliveCell(i, j)) {
				chunk[i - x + 2][j - y + 2] = true;
			}
		}
	}

	getBounds(x, minX, maxX, 1);
	getBounds(y, minY, maxY, 1);
	for (int64_t i = minX; i <= maxX; ++i) {
		for (int64_t j = minY; j <= maxY; ++j) {
			int x2 = (int)(i - x + 2);
			int y2 = (int)(j - y + 2);
			/* Count up neighbors of potential live cells (neighbors of live cell)
			 * to determine if cell will be live or dead
			 */
			int numNeighbors = 0;
			for (int n = x2 - 1; n <= x2 + 1; ++n) {
				for (int m = y2 - 1; m <= y2 + 1; ++m) {
					if (n == x2 && m == y2) {
						continue;
					}
					if (chunk[n][m]) {
						++numNeighbors;
					}
				}
			}
			if (numNeighbors == 3 || (chunk[x2][y2] && numNeighbors == 2)) {
				nlivecells.insert(std::make_pair(i, j));
			}
		}
	}
}

// Get bounds for 5x5 or 3x3 area where index is in the middle; offset = 2 or = 1 respectively
void Board::getBounds(int64_t index, int64_t& start, int64_t& end, int offset) {
	int64_t imin = std::numeric_limits<int64_t>::min();
	int64_t imax = std::numeric_limits<int64_t>::max();

	if (index >= imin + offset) {
		start = index - offset;
	} else {
		start = imin;
	}
	if (imax - offset >= index) {
		end = index + offset;
	}
	else {
		end = imax;
	}
}

bool Board::isAliveCell(int64_t x, int64_t y) {
	auto it = livecells.find(std::make_pair(x, y));
	return it != livecells.end();
}

// Return whether this board equals other board
bool Board::operator==(const Board &other) const {
	return this->livecells == other.livecells;
}
