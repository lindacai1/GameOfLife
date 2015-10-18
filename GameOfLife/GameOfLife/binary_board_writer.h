#include <ostream>
#include <stdint.h>

class BinaryBoardWriter {
private:
	std::ostream& out;
	bool isStart;
	int64_t curX;
	int64_t len;
	int64_t lenPos;
	void startSegment(int64_t x);
	void finishSegment();
public:
	BinaryBoardWriter(std::ostream& out);
	~BinaryBoardWriter();
	void add(int64_t x, int64_t y);
};