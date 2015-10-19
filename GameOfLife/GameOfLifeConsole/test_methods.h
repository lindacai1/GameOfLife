#pragma once
#include "board.h"

Board makeSample1();
Board makeSample2();
Board makeLargeRandomBoard();
Board makeLargeAdjacentBoard();
void makeLargeBinaryFile();
std::string readBinaryBoard(std::istream& in);
std::string encode(const std::vector<int64_t>& expected);
void printBoard(Board& board);