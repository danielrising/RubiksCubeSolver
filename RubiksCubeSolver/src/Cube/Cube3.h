#pragma once

#include "Voxel/Voxel.h"

#define FACES 6
#define C_SIZE 8
#define E_SIZE 12
#define C_PER_FACE 4
#define E_PER_FACE 4

#define FACELET_PER_CORNER 3
#define FACELET_PER_EDGE 2
#define FACELET_PER_FACE (C_PER_FACE + E_PER_FACE + 1)

#define MOVE_STRIDE 2

/*3X3X3 CUBE CLASS*/
class Cube3
{
private:
	Voxel c[C_SIZE];
	Voxel e[E_SIZE];

	void Orient(const unsigned char& mov);
	void Move(const unsigned char& mov);
	std::vector<char> GenerateMatrix();

public:
	Cube3();

	// Manipulations
	void Rotate(const unsigned char &mov, const unsigned char &pow);
	void Scramble(short int amount, int randSeed, bool log);

	// Reads
	bool IsSolved(short int solveState);
	void ConsoleRender();
	void ConsolePrint();

	// Pruning-table numbers
	int getPermutationNumberCorner();
	int getOrientationNumberCorner();
	int getPermutationNumberEdge(bool firstHalf);
	int getOrientationNumberEdge(bool firstHalf);
};

// Solver-algorithms
Cube3 KociembaAlgorithm(Cube3 position, std::vector<char>& moves);
Cube3 IterativeDeepening(Cube3 position, char maxDepth, char solveState, std::vector<char>& moves, const std::vector<char>& possibleMoves);
Cube3 Treesearch(Cube3 position, char maxDepth, char depth, char solveState, std::vector<char>& moves, const std::vector<char>& possibleMoves);

// Pruning tables
