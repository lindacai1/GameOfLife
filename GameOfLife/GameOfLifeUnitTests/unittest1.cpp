#include "stdafx.h"
#include "CppUnitTest.h"
#include "board.h"
#include "board_io.h"
#include "testmethods.h"

#include <windows.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#define LOG_MESSAGE(x) { std::stringstream ss; ss << x << "\n"; Logger::WriteMessage(ss.str().c_str()); }

namespace GameOfLifeUnitTests
{		
	TEST_CLASS(InputOutput)
	{
	//public:
		TEST_METHOD(Input1)
		{

			std::ifstream input("../GameOfLifeUnitTests/IOFiles/input1.txt");
			Board iboard = BoardIO::read(input);
			input.close();
			Board board = makeSample1();

			Assert::IsTrue(iboard == board);
			//TCHAR s[100];
			//DWORD a = GetCurrentDirectory(100, s);
		}
		TEST_METHOD(Output1)
		{
			Board board = makeSample1();
			
			Board board1 = board.nextIteration();

			{std::ofstream output("../GameOfLifeUnitTests/IOFiles/output1.txt");
			BoardIO::write(board1, output); }
			Board board2;
			{std::ifstream input("../GameOfLifeUnitTests/IOFiles/output1.txt");
			board2 = BoardIO::read(input); }
			Assert::IsTrue(board1 == board2);
			Assert::IsTrue(true);
		}

	};
	TEST_CLASS(OpenMPBenchmark)
	{

	};
	TEST_CLASS(DoesNotFitInMemory) {};
}