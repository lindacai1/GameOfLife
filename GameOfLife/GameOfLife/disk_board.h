#include <istream>
#include <memory>
#include <vector>

class DiskBoard {
	class ColumnBuffer;
public:
	DiskBoard(std::unique_ptr<std::istream> data);
	DiskBoard nextIteration(std::unique_ptr<std::iostream> output);
	DiskBoard(DiskBoard&&);
private:
	std::unique_ptr<std::istream> livecells;
	bool readBuffer(ColumnBuffer& cb, int64_t pos);
	
	class ColumnBuffer {
	public:
		int64_t x;
		int64_t startPos; // start of column; is the position of the x-value
		int64_t len;
		int64_t yPos; // start position of current set of y-values
		std::vector<int64_t> bufferedVals;
		ColumnBuffer();
		int64_t getNextStartPos();
	};
};