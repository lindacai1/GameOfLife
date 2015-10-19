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

	class ColumnIterator {
		ColumnInfo ci;
		std::istream& input;
		int64_t minNextColToCheck;
		bool sawMaxNum;
		ColumnIterator(std::istream& input);
		bool getNextColToCheck(int64_t& outCol);
	};

	struct ColumnInfo {
		int64_t x;
		int64_t startPos;
		int64_t len;
		bool isValid; // True if this is a valid column
		ColumnInfo();
		int64_t getNextPos();
		void setFromStream(istream& input, int64_t pos);
	};

};