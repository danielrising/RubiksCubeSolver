#pragma once

#include "Voxel/Voxel.h"

// 3x3x3-Cube constants
#define FACES 6
#define C_SIZE 8
#define E_SIZE 12
#define C_PER_FACE 4
#define E_PER_FACE 4
#define E_PER_SLICE 4
#define FACELET_PER_CORNER 3
#define FACELET_PER_EDGE 2
#define FACELET_PER_FACE (C_PER_FACE + E_PER_FACE + 1)

// Datastructure constants (strides)
#define MOVE_STRIDE 2

// Pruning table switch
#define PRUNE_EDGECORNERTWIST 0
#define PRUNE_EDGETWIST_UDCOMB 1
#define PRUNE_CORNERTWIST_UDCOMB 2
#define PRUNE_CORNERPERM 3
#define PRUNE_EDGEPERM 4
#define PRUNE_PHASETWO 5
#define PRUNE_PHASEONE 6

// Pruning index max sizes
#define UDSLICE_COMBINATION_INDEX_SIZE Choose(E_SIZE, E_PER_SLICE)
#define EDGE_TWIST_INDEX_SIZE pow(FACELET_PER_EDGE, E_SIZE - 1)
#define CORNER_TWIST_INDEX_SIZE pow(FACELET_PER_CORNER, C_SIZE - 1)
#define CORNER_PERM_INDEX_SIZE Factorial(C_SIZE)
#define UDEDGE_PERM_INDEX_SIZE Factorial(E_SIZE - E_PER_SLICE)

// n choose k
int Choose(int n, int k);
// ! (faculty/factorial)
int Factorial(int n);

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
Cube3 KociembaAlgorithm(Cube3 position, std::vector<char>& moves, const std::vector<char>& indexIdsOne, const std::vector<std::vector<char>*>& pruneTablesOne, const std::vector<char>& indexIdsTwo, const std::vector<std::vector<char>*>& pruneTablesTwo);
Cube3 IterativeDeepening(Cube3 position, char maxDepth, char solveState, std::vector<char>& moves, const std::vector<char>& possibleMoves, const std::vector<char>& indexIds, const std::vector<std::vector<char>*>& pruneTables);
Cube3 Treesearch(Cube3 position, char maxDepth, char treeMaxDepth, char depth, char solveState, std::vector<char>& moves, const std::vector<char>& possibleMoves, const std::vector<char>& indexIds, const std::vector<std::vector<char>*>& pruneTables);

// Pruning-tables
void GeneratePruneTable(std::vector<char>& table, size_t tableSize, std::vector<char> possibleMoves, char indexId, std::string name);

void TableOne(std::vector<char>& table);
void TableTwo(std::vector<char>& table);
void TableThree(std::vector<char>& table);
void TableFour(std::vector<char>& table);
void TableFive(std::vector<char>& table);
void TableSix(std::vector<char>& table);
void TableSeven(std::vector<char>& table);