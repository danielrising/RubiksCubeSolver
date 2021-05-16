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

#define PRUNE_FACE_ONE 0
#define PRUNE_FACE_TWO 1

// n choose k
int Choose(int n, int k);

/*3X3X3 CUBE CLASS*/
class Cube3
{
private:
	Voxel c[C_SIZE];
	Voxel e[E_SIZE];

	void Orient(const unsigned char& mov);
	void Move(const unsigned char& mov);
	std::vector<char> GenerateMatrix();

	// Pruning table indices

	// These indices represents part of the cubes current configuration.
	// Using these makes it possible lookup if current position is solvable with remaining depth.
	// Each unique combination of the subset has to return a unique index.
	int EdgeTwistIndex(); // Edge twist configuration number (0 ... 2048 - 1 or 2^11 )
	int CornerTwistIndex(); // Corner twist configuration number (0 ... 2187 - 1 or 3^7)
	int UDSliceCombinationIndex(); // Where are the UD Slice edges (0 ... 495 - 1 or 12 choose 4)
	int CornerPermutationIndex(); // Corner permutation number (0 ... 40 320 - 1 or 8!)
	int UDEdgePermutationIndex(); // U- or D-face edges permutation number (0 ... 40 320 - 1 or 8!)

public:
	// Pruning-table indices
	int PruneIndex(char indexId);

	Cube3();

	// Manipulations
	void Rotate(const unsigned char &mov, const unsigned char &pow);
	void Scramble(short int amount, int randSeed, bool log);

	// Reads
	bool IsSolved(short int solveState);
	void ConsoleRender();
	void ConsolePrint();
};

// Move

// Solver-algorithms
Cube3 KociembaAlgorithm(Cube3 position, std::vector<char>& moves);
Cube3 IterativeDeepening(Cube3 position, char maxDepth, char solveState, std::vector<char>& moves, const std::vector<char>& possibleMoves, std::vector<char>& pruneTable1);
Cube3 Treesearch(Cube3 position, char maxDepth, char depth, char solveState, std::vector<char>& moves, const std::vector<char>& possibleMoves, std::vector<char>& pruneTable1);

// Pruning-tables
void GeneratePruneTable(std::vector<char>& table, size_t tableSize, std::vector<char> possibleMoves, char indexId, std::string name);

void TableOne(std::vector<char>& table);
void TableTwo(std::vector<char>& table);
void TableThree(std::vector<char>& table);