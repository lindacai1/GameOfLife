#include "disk_board.h"
using std::unique_ptr;
using std::istream;
using std::iostream;

//unique_ptr<istream> livecells;

DiskBoard::DiskBoard(unique_ptr<istream> data) : livecells(std::move(data)) {
}

/* Our data (livecells) is a sorted binary file of the format: x, length of y values, y values
 * Stream in a small number of values of 3 adjacent rows
 */
DiskBoard DiskBoard::nextIteration(unique_ptr<iostream> output) {
	
}