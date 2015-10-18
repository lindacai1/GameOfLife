#include "binary_board_writer.h";

/* 
 * Write live cells in binary file where each segment is of the
 * format: x_value (8 bytes), length_of_y_values (8 bytes), y values (8*#y_values bytes)
 */

BinaryBoardWriter::BinaryBoardWriter(std::ostream& out) : out(out), isStart(true), curX(-1), len(0), lenPos(-1) {
}

BinaryBoardWriter::~BinaryBoardWriter() {
	finishSegment();
}

// add must be called with (x, y) pairs in-order (row, column)
void BinaryBoardWriter::add(int64_t x, int64_t y) {
	if (isStart) {
		startSegment(x);
		isStart = false;
	} else if (x != curX) {
		finishSegment();
		startSegment(x);
	}
	out.write(reinterpret_cast<const char*> (&y), sizeof(y));
	++len;
}

void BinaryBoardWriter::startSegment(int64_t x) {
	out.write(reinterpret_cast<const char*> (&x), sizeof(x));
	lenPos = out.tellp();
	int64_t initLen = (int64_t)0;
	out.write(reinterpret_cast<const char*> (&initLen), sizeof(int64_t));
	curX = x;
}

void BinaryBoardWriter::finishSegment() {
	int64_t curPos = out.tellp();
	out.seekp(lenPos);
	out.write(reinterpret_cast<const char*> (&len), sizeof(int64_t));
	out.seekp(curPos);
	len = 0;
}


//void temporary_function() {
//	bool isFirst = true;
//	int64_t curx = -1;
//	int64_t length = 0;
//	int64_t len_pos = -1;
//	for (auto& cell : lboard.livecells) {
//		// If done writing y values for curx, rewrite the length field and begin new segment
//		if (!isFirst && cell.first != curx) {
//			int64_t cur_pos = output.tellp();
//			output.seekp(len_pos);
//			output.write(reinterpret_cast<const char*> (length), sizeof(int64_t));
//			output.seekp(cur_pos);
//			isFirst = true;
//			length = 0;
//		}
//		if (isFirst) {
//			curx = cell.first;
//			// Write x field, length field (keep position so we can write final value later), and y value
//			output.write(reinterpret_cast<const char*> (&cell.first), sizeof(cell.first));
//			len_pos = output.tellp();
//			output.write(reinterpret_cast<const char*> (0), sizeof(int64_t));
//			output.write(reinterpret_cast<const char*> (&cell.second), sizeof(cell.second));
//			++length;
//			isFirst = false;
//			// Continue writing y values
//		} else if (cell.first == curx) {
//			output.write(reinterpret_cast<const char*> (&cell.second), sizeof(cell.second));
//			++length;
//		}
//	}
//}