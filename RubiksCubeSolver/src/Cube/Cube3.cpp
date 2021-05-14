#include "rcspch.h"

#include "Cube3.h"

const char faces[FACES] = { 'U', 'L', 'F', 'R', 'B', 'D' };

const unsigned char edgeMove[FACES][E_PER_FACE] =
{
	{1, 3, 2, 0},	// U
	{4, 9, 6, 1},	// L
	{6, 11, 7, 3},	// F
	{7, 10, 5, 2},	// R
	{5, 8, 4, 0},	// B
	{9, 8, 10, 11}	// D
};

const unsigned char cornerMove[FACES][C_PER_FACE] = 
{
	{2, 3, 1, 0},	// U
	{4, 6, 2, 0},	// L
	{6, 7, 3, 2},	// F
	{7, 5, 1, 3},	// R
	{5, 4, 0, 1},	// B
	{5, 7, 6, 4}	// D
};

// An edge is properly oriented when it's U or D facelet is on the U or D face.
// In the case that the piece is not on the U or D face, the edge is properly oriented when it's F or B facelet is on the F or B face.
// An orientation of 0 is correct, 1 is flipped
// The index of a given rotation is tied to the position from edgeMove of the same index,
// (this is however not relevant since every subarray only contains similar integers.)
const unsigned char edgeOrient[FACES][C_PER_FACE] =
{
	{0, 0, 0, 0},	// U
	{1, 1, 1, 1},	// L
	{0, 0, 0, 0},	// F
	{1, 1, 1, 1},	// R
	{0, 0, 0, 0},	// B
	{0, 0, 0, 0}	// D
};

// A corner is properly oriented when it's U or D facelet is on the U or D face.
// An orientation of 0 is correct, 1 is one clockise twists, 2 one counter clockise twist.
// The index of a given rotation is tied to the position from cornerMove of the same index.
const unsigned char cornerOrient[FACES][C_PER_FACE] =
{
	{0, 0, 0, 0},	// U
	{2, 1, 2, 1},	// L
	{2, 1, 2, 1},	// F
	{2, 1, 2, 1},	// R
	{2, 1, 2, 1},	// B
	{0, 0, 0, 0}	// D
};

// Allowed moves in phase 1
const std::vector<char> subGroupOne = {
	0, 1,	1, 1,	2, 1,	3, 1,	4, 1,	5, 1,
	0, 2,	1, 2,	2, 2,	3, 2,	4, 2,	5, 2,
	0, 3,	1, 3,	2, 3,	3, 3,	4, 3,	5, 3
};

// Allowed moves in phase 2
const std::vector<char> subGroupTwo = {
	0, 1,									5, 1,
	0, 2,	1, 2,	2, 2,	3, 2,	4, 2,	5, 2,
	0, 3,									5, 3
};

const size_t UDSliceCombinationIndexMax = 495;
const size_t EdgeTwistIndexMax = 2048;
const size_t CornerTwistIndexMax = 2187;
const size_t tableSize = EdgeTwistIndexMax*CornerTwistIndexMax;

int Choose(int n, int k)
{
	int binCoff = 1; // Binomial Coefficiant
	for (int i = 2; i <= n; i++) // n!
	{
		binCoff *= i;
	}
	for (int i = 2; i <= k; i++) // Divide by k!
	{
		binCoff /= i;
	}
	for (int i = 2; i <= n - k; i++) // Divide by (n - k)!
	{
		binCoff /= i;
	}
	return binCoff;
}

void Cube3::Orient(const unsigned char& mov)
{
	// EDGES
	for (int i = 0; i < E_PER_FACE; i++) {
		e[edgeMove[mov][i]].SetR((e[edgeMove[mov][i]].GetR() + edgeOrient[mov][i]) % 2);
	}

	// CORNERS
	for (int i = 0; i < C_PER_FACE; i++) {
		c[cornerMove[mov][i]].SetR((c[cornerMove[mov][i]].GetR() + cornerOrient[mov][i]) % 3);
	}
}

void Cube3::Move(const unsigned char& mov)
{
	// Moves are performed as follows,
	// Grab the voxel that would've been moved last to a temporary voxel canister.
	// Permute all voxels except last using some kind of loop.
	// Update last voxel using the temporary voxel stored from earlier.

	// EDGES
	Voxel eTemp = e[edgeMove[mov][0]];

	for (int i = 0; i < E_PER_FACE - 1; ++i)
	{
		e[edgeMove[mov][i]] = e[edgeMove[mov][(i + 1) % E_PER_FACE]];
	}

	e[edgeMove[mov][E_PER_FACE - 1]] = eTemp;


	// CORNERS
	Voxel cTemp = c[cornerMove[mov][0]];

	for (int i = 0; i < C_PER_FACE - 1; ++i)
	{
		c[cornerMove[mov][i]] = c[cornerMove[mov][(i + 1) % C_PER_FACE]];
	}

	c[cornerMove[mov][C_PER_FACE - 1]] = cTemp;

}

std::vector<char> Cube3::GenerateMatrix()
{
	// Priorities in ordering (Clockwise rotations after first)
	// Observe that these are tied to the orientation convention used for cornerOrient.
	// 1 White & Yellow
	// (2 Green, Blue, Orange & Red)
	const char cornerColors[C_SIZE][3] = {
		{'W', 'B', 'O'},
		{'W', 'R', 'B'},
		{'W', 'O', 'G'},
		{'W', 'G', 'R'},
		{'Y', 'O', 'B'},
		{'Y', 'B', 'R'},
		{'Y', 'G', 'O'},
		{'Y', 'R', 'G'}
	};

	// Priorities in ordering
	// Observe that these are tied to the orientation convention used for edgeOrient.
	// Which results in orange-red and green-blue switching (for some reason, just tested and it works)
	// 1 White & Yellow
	// 2 Orange & Red
	// (3 Green & Blue)
	const char edgeColors[E_SIZE][2] = {
		{'W', 'B'},
		{'W', 'O'},
		{'W', 'R'},
		{'W', 'G'},
		{'O', 'B'},
		{'R', 'B'},
		{'O', 'G'},
		{'R', 'G'},
		{'Y', 'B'},
		{'Y', 'O'},
		{'Y', 'R'},
		{'Y', 'G'}
	};

	// Sides from cube3::faces has colors;
	const char faceColors[FACES] = { 'W', 'O', 'G', 'R', 'B', 'Y' };

	// Facelet coords for corners, using priorities from cornerColors.
	// Coordinates uses convention from showLayout (in ConsoleRender) and rendered.
	const char cornerFaceletCoords[C_SIZE][3][2] = {
		{{0, 0}, {4, 6}, {1, 2}},
		{{0, 2}, {3, 0}, {4, 8}},
		{{0, 6}, {1, 8}, {2, 0}},
		{{0, 8}, {2, 2}, {3, 6}},
		{{5, 6}, {1, 0}, {4, 0}},
		{{5, 8}, {4, 2}, {3, 2}},
		{{5, 0}, {2, 6}, {1, 6}},
		{{5, 2}, {3, 8}, {2, 8}}
	};

	// Facelet coords for edges, using priorities from edgeColors.
	// Coordinates uses convention from showLayout and rendered.
	const char edgeFaceletCoords[E_SIZE][2][2] = {
		{{0, 1}, {4, 7}},
		{{0, 3}, {1, 5}},
		{{0, 5}, {3, 3}},
		{{0, 7}, {2, 1}},
		{{1, 1}, {4, 3}},
		{{3, 1}, {4, 5}},
		{{1, 7}, {2, 3}},
		{{3, 7}, {2, 5}},
		{{5, 7}, {4, 1}},
		{{5, 3}, {1, 3}},
		{{5, 5}, {3, 5}},
		{{5, 1}, {2, 7}}
	};

	std::vector<char> generatedMatrix;
	generatedMatrix.resize(FACES * FACELET_PER_FACE);

	// Generate corner pieces
	for (int i = 0; i < C_SIZE; i++) {
		for (int j = 0; j < FACELET_PER_CORNER; j++) {
			// Index calculations.
			char faceIndex = cornerFaceletCoords[i][j][0];
			char faceletIndex = cornerFaceletCoords[i][j][1];
			char matrixIndex = faceIndex * FACELET_PER_FACE + faceletIndex;

			generatedMatrix[matrixIndex] = cornerColors[c[i].GetId()][(c[i].GetR() + j) % FACELET_PER_CORNER];
		}
	}

	// Generate edge pieces
	for (int i = 0; i < E_SIZE; i++) {
		for (int j = 0; j < FACELET_PER_EDGE; j++) {
			// Index calculations.
			char faceIndex = edgeFaceletCoords[i][j][0];
			char faceletIndex = edgeFaceletCoords[i][j][1];
			char matrixIndex = faceIndex * FACELET_PER_FACE + faceletIndex;

			generatedMatrix[matrixIndex] = edgeColors[e[i].GetId()][(e[i].GetR() + j) % FACELET_PER_EDGE];
		}
	}

	// Generate middle pieces
	for (int i = 0; i < FACES; i++) {
		// Index calculations.
		char faceIndex = i;
		char faceletIndex = 4; // Middle piece facelet index is 4.
		char matrixIndex = faceIndex * FACELET_PER_FACE + faceletIndex;

		generatedMatrix[matrixIndex] = faceColors[i];
	}

	return generatedMatrix;
}

int Cube3::EdgeTwistIndex()
{
	int index = 0;
	for (int i = 0; i < E_SIZE - 1; i++) {
		// index += pow(FACELET_PER_EDGE, i) * e[i].GetR();
		index |= e[i].GetR() << i;
	}
	return index;
}

int Cube3::CornerTwistIndex()
{
	int index = 0;
	for (int i = 0; i < C_SIZE - 1; i++) {
		index += pow(FACELET_PER_CORNER, i) * c[i].GetR();
	}
	return index;
}

int Cube3::UDSliceCombinationIndex()
{
	short int middleEdges[12] = { 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0 };

	int index = 0;
	short int passedMiddleEdges = -1;
	for (int i = 0; i < E_SIZE; i++) {
		if (middleEdges[e[i].GetId()]) {
			passedMiddleEdges++;
		}
		else if (passedMiddleEdges != -1) {
			index += Choose(i, passedMiddleEdges);
		}
	}
	return index;
}

int Cube3::CornerPermutationIndex()
{
	return 0;
}

int Cube3::UDEdgePermutationIndex()
{
	return 0;
}

// 0 - EdgeTwistIndex * CornerTwistIndex
// 1 - EdgeTwistIndex * UDSliceCombinationIndex
// 2 - CornerTwistIndex * UDSliceCombinationIndex
int Cube3::PruneIndex(char indexId)
{

	switch (indexId) {
	case 0: {
		int x = EdgeTwistIndex();
		int y = CornerTwistIndex();
		return x*CornerTwistIndexMax + y;
	}
	case 1: {
		int x = EdgeTwistIndex();
		int y = UDSliceCombinationIndex();
		return x*UDSliceCombinationIndexMax + y;
	}
	case 2: {
		int x = CornerTwistIndex();
		int y = UDSliceCombinationIndex();
		return x*UDSliceCombinationIndexMax + y;
	}
	}
}

Cube3::Cube3()
{
	for (int i = 0; i < C_SIZE; ++i)
	{
		c[i].SetId(i);
		c[i].SetR(0);
	}

	for (int i = 0; i < E_SIZE; ++i)
	{
		e[i].SetId(i);
		e[i].SetR(0);
	}

	GenerateMatrix();
}

// mov 0 - 5, pow 1-3
// mov corresponding to following, (Up, left, front, right, back, down)
// pow corresponding to following, (Clockwise quarter, half, Counter clockwise quarter)
void Cube3::Rotate(const unsigned char &mov, const unsigned char& pow)
{	
	for (int i = 0; i < pow; i++) {
		Orient(mov);
		Move(mov);
	}
}

void Cube3::Scramble(short int amount, int randSeed, bool log)
{
	// Initilize random seed
	srand(randSeed);

	for (int i = 0; i < amount; i++) {

		char move = rand() % FACES; // Random move, 0 to (including) 5
		char pow = 1 + rand() % 3; // Random power of that move, 1 to (inluding) 3

		Rotate(move, pow);

		const char power[3] = { ' ', '2', 39 };

		if (log) {
			std::cout << faces[move] << power[pow - 1] << " -> ";
		}
	}
	if (log) {
		std::cout << std::endl;
	}
}

bool Cube3::IsSolved(short int solveState)
{
	// Fully solved
	if (solveState == 0)
	{
		for (int i = 0; i < C_SIZE; ++i)
		{
			if (c[i].GetId() != i) {
				return false;
			}
			if (c[i].GetR() != 0) {
				return false;
			}
		}
		for (int i = 0; i < E_SIZE; ++i)
		{
			if (e[i].GetId() != i) {
				return false;
			}
			if (e[i].GetR() != 0) {
				return false;
			}
		}
		return true;
	}

	// Orientation solved
	if (solveState == 1) {
		for (int i = 0; i < C_SIZE; ++i)
		{
			if (c[i].GetR() != 0) {
				return false;
			}
		}
		for (int i = 0; i < E_SIZE; ++i)
		{
			if (e[i].GetR() != 0) {
				return false;
			}
		}
		return true;
	}

	std::cout << "Solve state not found!" << std::endl;
	return false;
}

void Cube3::ConsoleRender()
{
	// Rendering 3D cube in 2D with following convention
	//		B
	//	O	W	R
	//		G	 
	//		Y
	// -1 = blank, rest follows index from faceColors
	const char showLayout[4][3] = {
		{-1, 4, -1},
		{ 1, 0, 3 },
		{-1, 2, -1},
		{-1, 5, -1}
	};

	// Update cube state
	std::vector<char> matrix = GenerateMatrix();

	// Show
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 3; k++) {
				if (showLayout[i][k] == -1) {
					std::cout << "      ";
				}
				else {
					for (int l = 0; l < 3; l++) {
						char faceIndex = showLayout[i][k];
						char faceletIndex = j*3 + l;
						char matrixIndex = faceIndex * FACELET_PER_FACE + faceletIndex;
						std::cout << matrix[matrixIndex] << " ";
					}
				}
			}
			std::cout << std::endl;
		}
	}
}

void Cube3::ConsolePrint()
{
	std::cout << "Corners:" << std::endl;
 
	for (int i = 0; i < C_SIZE; ++i)
	{
		std::cout << c[i].ToString() << std::endl;
	}

	std::cout << "Edges:" << std::endl;

	for (int i = 0; i < E_SIZE; ++i)
	{
		std::cout << e[i].ToString() << std::endl;
	}
}

/* SOLVER-ALGORITHMS */

// For efficiency reasons, all moves are stored in pairs of two in a single vector. Therefore, a vector stride of 2.
// {[move], [power], ..., [move], [power]}

// Implementation of the Kociemba algorithm using IterativeDeepening and Treesearch
Cube3 KociembaAlgorithm(Cube3 position, std::vector<char>& moves)
{
	std::vector<char> movesOne;
	std::vector<char> movesTwo;

	// Phase 1
	//position = IterativeDeepening(position, 18, 1, movesOne, allowedMovesOne);

	// Phase 2
	//position = IterativeDeepening(position, 12, 0, movesTwo, allowedMovesTwo);

	// Moves conversion.
	int movesOneLength = movesOne.size();
	int movesTwoLength = movesTwo.size();
	int movesLength = movesOneLength + movesTwoLength;

	moves.resize(movesOne.size() + movesTwo.size());
	for (int i = 0; i < movesLength; i++) {
		moves[i] = movesOne[i];
		int movesPhaseTwoIndex = i + movesOneLength;
		moves[movesPhaseTwoIndex] = movesTwo[i];
	}

	return position;
}

// Calls treesearch with increasing depth, for breadth-first results.
// Makes sure that the first found solution is the shortest actual solution for that sub-problem.
Cube3 IterativeDeepening(Cube3 position, char maxDepth, char solveState, std::vector<char>& moves, const std::vector<char>& possibleMoves, std::vector<char>& pruneTable1)
{
	// One move each layer of depth, initialize with a number not corresponding to any move.
	int movesSize = maxDepth * MOVE_STRIDE;
	moves.resize(movesSize);
	for (int i = 0; i < maxDepth * MOVE_STRIDE; i++) {
		moves[i] = -1;
	}

	// Iterative deepening
	for (int i = 0; i < maxDepth; i++) {
		std::cout << "Current depth: " << i << std::endl;
		position = Treesearch(position, i, i, solveState, moves, possibleMoves, pruneTable1);

		if (position.IsSolved(solveState)) {
			return position;
		}
	}
	std::cout << "Solution not found in " << maxDepth << " layers of depth!" << std::endl;
	return position;
}

// Recursive function to be used in combination with Iterative deepening (IDA) - Only tests if solved at last function call (depth = 0)
Cube3 Treesearch(Cube3 position, char maxDepth, char depth, char solveState, std::vector<char>& moves, const std::vector<char>& possibleMoves, std::vector<char>& pruneTable1)
{
	// Iterative deepening...
	if (depth == 0) {
		return position;
	}

	else {
		// Prunes - Don't go deeper if we can already know it's unsolvable with remaining depth.
		if (false) {
			return position;
		}

		// Call each possible move from this position.
		for (int i = 0; i < possibleMoves.size() - 1; i += MOVE_STRIDE) {
			int possiblePowIndex = i + 1;

			char nextMove = possibleMoves[i];
			char nextPower = possibleMoves[possiblePowIndex];

			int movesIndex = (maxDepth - depth) * MOVE_STRIDE;
			int movesPowIndex = movesIndex + 1;

			// Don't repeat moves of same face
			if (movesIndex > 1) {
				int previousMovesIndex = movesIndex - MOVE_STRIDE;
				if (moves[previousMovesIndex] == nextMove) {
					continue;
				}
			}

			// Rotate.
			position.Rotate(nextMove, nextPower);

			moves[movesIndex] = possibleMoves[i];
			moves[movesPowIndex] = possibleMoves[possiblePowIndex];

			// Recursion
			Cube3 result = Treesearch(position, maxDepth, depth - 1, solveState, moves, possibleMoves, pruneTable1);

			if (result.IsSolved(solveState)) { // Solved? break recursion.
				return result;
			}
			else { // Undo rotation
				position.Rotate(nextMove, C_PER_FACE - nextPower);
			}
		}
		return position;
	}
}

/* PRUNING-TABLES */

void generatePruneTable(std::vector<char>& table, size_t tableSize, std::vector<char> possibleMoves, char indexId, std::string name) {
	// Pruning table initialization
	table.resize(tableSize, -1);

	// First in first out queues, separate for holding corresponding depth
	std::queue<Cube3> fifoQueue;
	std::queue<char> fifoQueueDepth;
	fifoQueue.push(Cube3());
	fifoQueueDepth.push(0);

	// Set first table index
	table[fifoQueue.front().PruneIndex(indexId)] = 0;

	// DEBUG ...
	long int counter = 0;
	auto start = std::chrono::system_clock::now(); // Starting time
	auto sinceLast = std::chrono::system_clock::now();
	// ... DEBUG

	while (!(fifoQueue.empty())) {
		counter++; // DEBUG

		// Grab queued position
		Cube3 position = fifoQueue.front();
		char depth = fifoQueueDepth.front();
		fifoQueue.pop();
		fifoQueueDepth.pop();

		// Test all new position
		for (int i = 0; i < possibleMoves.size() - 1; i += 2) {
			// Next move
			int possiblePowIndex = i + 1;
			char nextMove = possibleMoves[i];
			char nextPower = possibleMoves[possiblePowIndex];
			position.Rotate(nextMove, nextPower);

			// Table index
			if (table[position.PruneIndex(indexId)] == -1) { // Only enqueue new cases
				fifoQueue.push(position);
				fifoQueueDepth.push(depth + 1);
				table[position.PruneIndex(indexId)] = depth + 1;
			}

			// Undo move
			position.Rotate(nextMove, C_PER_FACE - nextPower);
		}

		// DEBUG ...
		if (!(fifoQueueDepth.empty()) && fifoQueueDepth.front() != depth) {
			// Elapsed time
			auto end = std::chrono::system_clock::now();
			
			std::chrono::duration<double> elapsedSeconds = end - start;
			std::chrono::duration<double> elapsedSecondsSinceLast = end - sinceLast;

			std::cout << "Generated " << name << " up to and including depth " << (int)depth << std::endl <<
				"With a total of " << counter << " positions queued." << std::endl <<
				"Time elapsed: " << elapsedSecondsSinceLast.count() << "s (" << elapsedSeconds.count() << "s total)" << std::endl <<
				std::endl;

			sinceLast = std::chrono::system_clock::now();
			counter = 0;
		}
		// ... DEBUG
	}
}

// EdgeTwistIndex * CornerTwistIndex
void tableOne(std::vector<char>& table)
{
	generatePruneTable(table, EdgeTwistIndexMax * CornerTwistIndexMax, subGroupOne, 0, "table one");
}

// EdgeTwistIndexMax * UDSliceCombinationIndexMax
void tableTwo(std::vector<char>& table)
{
	generatePruneTable(table, EdgeTwistIndexMax * UDSliceCombinationIndexMax, subGroupOne, 1, "table two");
}

// CornerTwistIndex * UDSliceCombinationIndexMax
void tableThree(std::vector<char>& table)
{
	generatePruneTable(table, CornerTwistIndexMax * UDSliceCombinationIndexMax, subGroupOne, 2, "table three");
}