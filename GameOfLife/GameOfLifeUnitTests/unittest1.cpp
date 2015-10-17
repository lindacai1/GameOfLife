#include "stdafx.h"
#include "CppUnitTest.h"
#include "board.h"
#include "board_io.h"

#include <windows.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#define LOG_MESSAGE(x) { std::stringstream ss; ss << x << "\n"; Logger::WriteMessage(ss.str().c_str()); }

namespace GameOfLifeUnitTests
{		
	TEST_CLASS(InputOutput)
	{
	public:
		void printBoard(Board& board)
		{
			for (const std::pair<int64_t, int64_t>& column : board.livecells) {
				int64_t x = column.first;
				int64_t y = column.second;
				LOG_MESSAGE(x << " " << y);
			}
		}
		TEST_METHOD(Input1)
		{

			std::ifstream input("../GameOfLifeUnitTests/IOFiles/input1.txt");
			Board iboard = BoardIO::read(input);
			input.close();
			Board board;
			board.addLivecell(0, 1);
			board.addLivecell(1, 2);
			board.addLivecell(2, 0);
			board.addLivecell(2, 1);
			board.addLivecell(2, 2);
			board.addLivecell(-2000000000000, -2000000000000);
			board.addLivecell(-2000000000001, -2000000000001);
			board.addLivecell(-2000000000001, -2000000000000);

			Assert::IsTrue(iboard == board);
			//TCHAR s[100];
			//DWORD a = GetCurrentDirectory(100, s);
		}
		TEST_METHOD(Output1)
		{
			Board board;
			board.addLivecell(0, 1);
			board.addLivecell(1, 2);
			board.addLivecell(2, 0);
			board.addLivecell(2, 1);
			board.addLivecell(2, 2);
			board.addLivecell(-2000000000000, -2000000000000);
			board.addLivecell(-2000000000001, -2000000000001);
			board.addLivecell(-2000000000001, -2000000000000);
			
			Board board1 = board.nextIteration();

			{std::ofstream output("../GameOfLifeUnitTests/IOFiles/output1.txt");
			BoardIO::write(board1, output); }
			Board board2;
			{std::ifstream input("../GameOfLifeUnitTests/IOFiles/output1.txt");
			board2 = BoardIO::read(input); }
			Assert::IsTrue(board1 == board2);
		}

	};
	TEST_CLASS(DoesNotFitInMemory) {};
}