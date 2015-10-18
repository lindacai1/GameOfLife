#include <istream>
#include <memory>

class DiskBoard {
	DiskBoard(std::unique_ptr<std::istream> data);
	std::unique_ptr<std::istream> livecells;
	DiskBoard nextIteration(std::unique_ptr<std::iostream> output);
};