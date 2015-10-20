Game of Life

/GameOfLife

Main implementations
-Board -> in-memory implementation, uses OpenMP for parallel computations of new cells
-DiskBoard -> out-of-core implementation designed to handle data that may not all fit into memory at a time; process data on disk
-> uses binary data format for cells written out by BinaryBoardWriter

/GameOfLifeConsole
-Unit Tests and Benchmarks -> See /IOFiles/*Benchmark.txt

/GameOfLifeGUI
-Simple GUI for 512x512 board implemented using QT