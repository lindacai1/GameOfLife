// GameOfLifeConsole.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <omp.h>


int _tmain(int argc, _TCHAR* argv[])
{
	// This statement should only print once
	printf("Starting Program!\n");

#pragma omp parallel
	{
		// This statement will run on each thread.
		// If there are 4 threads, this will execute 4 times in total
		printf("Running on multiple threads\n");
	}

	// We're out of the parallelized secion.
	// Therefor, this should execute only once
	printf("Finished!\n");

	std::cout << "Hello" << std::endl;

	return 0;
}

