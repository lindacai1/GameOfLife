#pragma once

#include <stdint.h>
#include <vector>
#include <map>
#include <set>

class Board {
public:
	// Set for fast search operations and duplicate handling
	std::set<std::pair<int64_t, int64_t>> livecells;
	void addLivecell(int64_t x, int64_t y);
	Board nextIteration();
	int numLiveNeighbors(int64_t x, int64_t y);
	void processCell(std::set<std::pair<int64_t, int64_t>>& nlivecells, int64_t x, int64_t y);
	void getBounds(int64_t index, int64_t& start, int64_t& end, int offset);
	bool isAliveCell(int64_t x, int64_t y);
	bool Board::operator==(const Board &other) const;
private:

};