#pragma once

#include "Voxel/Voxel.h"

#define FACES 6
#define C_SIZE 8
#define E_SIZE 12
#define C_PER_FACE 4
#define E_PER_FACE 4

/*3X3X3 CUBE CLASS*/
class Cube3
{
private:
	Voxel c[C_SIZE];
	Voxel e[E_SIZE];

	char generatedMatrix[FACES][9];

	void Orient(const unsigned char& mov);
	void Move(const unsigned char& mov);
	void GenerateMatrix();

public:
	Cube3();

	// Manipulations
	void Rotate(const unsigned char &mov, const unsigned char &pow);
	void Scramble(short int amount, bool log);

	// Reads
	bool IsSolved(short int solveState);
	void ConsoleRender();
	void ConsolePrint();
};

// Solver-algorithms
void IterativeDeepening(Cube3 position, short int depth, short int solveState, std::vector<short int>& moves);
Cube3 Treesearch(Cube3 position, short int maxDepth, short int depth, short int solveState, std::vector<short int>& moves);