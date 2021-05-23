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

// Pruning index max sizes
#define UDSLICE_COMBINATION_INDEX_SIZE Choose(E_SIZE, E_PER_SLICE)
#define EDGE_TWIST_INDEX_SIZE (int)pow(FACELET_PER_EDGE, E_SIZE - 1)
#define CORNER_TWIST_INDEX_SIZE (int)pow(FACELET_PER_CORNER, C_SIZE - 1)
#define CORNER_PERM_INDEX_SIZE Factorial(C_SIZE)
#define UDEDGE_PERM_INDEX_SIZE Factorial(E_SIZE - E_PER_SLICE)

// Pruning table switch
#define PRUNE_EDGECORNERTWIST 0
#define PRUNE_EDGETWIST_UDCOMB 1
#define PRUNE_CORNERTWIST_UDCOMB 2
#define PRUNE_PHASETWO 3

// Colored Messages
#define CONSOLE_BLACK 0
#define CONSOLE_BLUE 1
#define CONSOLE_GREEN 2
#define CONSOLE_AQUA 3
#define CONSOLE_RED 4
#define CONSOLE_PURPLE 5
#define CONSOLE_YELLOW 6
#define CONSOLE_WHITE 7
#define CONSOLE_GRAY 8
#define CONSOLE_LIGHT_BLUE 9
#define CONSOLE_LIGHT_GREEN 10
#define CONSOLE_LIGHT_AQUA 11
#define CONSOLE_LIGHT_RED 12
#define CONSOLE_LIGHT_PURPLE 13
#define CONSOLE_LIGHT_YELLOW 14
#define CONSOLE_BRIGHT_WHITE 15
void printColored(std::string text, int color);

// n choose k
int Choose(int n, int k);
// ! (faculty/factorial)
size_t Factorial(int n);

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
	void setStateFromCin();

	// Manipulations
	void Rotate(const unsigned char &mov, const unsigned char &pow);
	void Scramble(short int amount, int randSeed, bool log);

	// Reads
	bool IsSolved(short int solveState);
	void ConsoleRender();
	void ConsolePrint();
};

// Solver-algorithms
Cube3 KociembaAlgorithm(Cube3 position, std::vector<char>& moves, const std::vector<char>& indexIdsOne, const std::vector<std::vector<char>*>& pruneTablesOne, const std::vector<char>& indexIdsTwo, const std::vector<std::vector<char>*>& pruneTablesTwo);
Cube3 IterativeDeepening(Cube3 position, char maxDepth, char solveState, std::vector<char>& moves, const std::vector<char>& possibleMoves, const std::vector<char>& indexIds, const std::vector<std::vector<char>*>& pruneTables);
Cube3 Treesearch(Cube3 position, char maxDepth, char treeMaxDepth, char depth, char solveState, std::vector<char>& moves, const std::vector<char>& possibleMoves, const std::vector<char>& indexIds, const std::vector<std::vector<char>*>& pruneTables);

// Pruning-tables
void GeneratePruneTable(std::vector<char>& table, size_t tableSize, std::vector<char> possibleMoves, char indexId, std::string name);

void GeneratePhaseOneEC(std::vector<char>& table);
void GeneratePhaseOneEUD(std::vector<char>& table);
void GeneratePhaseOneCUD(std::vector<char>& table);
void GeneratePhaseTwo(std::vector<char>& table);

void WriteToFile(std::vector<char>& table, std::string fileName);
void ReadFromFile(std::vector<char>& table, std::string fileName);
void GenerateTables(bool log, std::vector<char>& tablePhaseOneEC, std::vector<char>& tablePhaseOneEUD, std::vector<char>& tablePhaseOneCUD, std::vector<char>& tablePhaseTwo);

// Format move number
void printFormatted(std::vector<char>& moves);