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

size_t Factorial(int n)
{
	int factorial = 1;
	for (int i = n; i > 1; i--) {
		factorial *= i;
	}
	return factorial;
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
		index += (int)pow(FACELET_PER_CORNER, i) * c[i].GetR();
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
	int index = 0;

	for (int i = 1; i < C_SIZE; i++) {
		unsigned char id = c[i].GetId();
		int count = 0;

		for (int j = 0; j < i; j++) {
			if (c[j].GetId() > id) {
				count++;
			}
		}

		index += count * (int)Factorial(i);
	}

	return index;
}

int Cube3::UDEdgePermutationIndex()
{
	int index = 0;

	for (int i = 1; i < E_PER_SLICE * 2; i++) {
		int plusFourI = 0;
		if (i > 3) plusFourI = 4; // Skip middle slice

		unsigned char id = e[i + plusFourI].GetId();
		int count = 0;

		for (int j = 0; j < i; j++) {
			int plusFourJ = 0;
			if (j > 3) plusFourJ = 4; // Skip middle slice

			if (e[j + plusFourJ].GetId() > id) {
				count++;
			}
		}

		index += count * (int)Factorial(i);
	}

	return index;
}

// 0 - EdgeTwistIndex * CornerTwistIndex
// 1 - EdgeTwistIndex * UDSliceCombinationIndex
// 2 - CornerTwistIndex * UDSliceCombinationIndex
int Cube3::PruneIndex(char indexId)
{

	switch (indexId) {
	case PRUNE_EDGECORNERTWIST: {
		int x = EdgeTwistIndex();
		int y = CornerTwistIndex();
		return x * CORNER_TWIST_INDEX_SIZE + y;
	}
	case PRUNE_EDGETWIST_UDCOMB: {
		int x = EdgeTwistIndex();
		int y = UDSliceCombinationIndex();
		return x * UDSLICE_COMBINATION_INDEX_SIZE + y;
	}
	case PRUNE_CORNERTWIST_UDCOMB: {
		int x = CornerTwistIndex();
		int y = UDSliceCombinationIndex();
		return x * UDSLICE_COMBINATION_INDEX_SIZE + y;
	}
	case PRUNE_PHASETWO: {
		int x = CornerPermutationIndex();
		int y = UDEdgePermutationIndex();
		return x * (int)UDEDGE_PERM_INDEX_SIZE + y;
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

	// Subgroup one
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
		for (int i = E_PER_FACE; i < 2 * E_PER_FACE; i++) {
			if (!(e[i].GetId() == 4 || e[i].GetId() == 5 || e[i].GetId() == 6 || e[i].GetId() == 7)) {
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
		std::cout << "[" << (int)c[i].GetId() << ", " << (int)c[i].GetR() << "]" << std::endl;
	}

	std::cout << "Edges:" << std::endl;

	for (int i = 0; i < E_SIZE; ++i)
	{
		std::cout << "[" << (int)e[i].GetId() << ", " << (int)e[i].GetR() << "]" << std::endl;
	}
}

/* SOLVER-ALGORITHMS */

// For efficiency reasons, all moves are stored in pairs of two in a single vector. Therefore, a vector stride of 2.
// {[move], [power], ..., [move], [power]}

// Implementation of the Kociemba algorithm using IterativeDeepening and Treesearch
Cube3 KociembaAlgorithm(Cube3 position, std::vector<char>& moves, const std::vector<char>& indexIdsOne, const std::vector<std::vector<char>*>& pruneTablesOne, const std::vector<char>& indexIdsTwo, const std::vector<std::vector<char>*>& pruneTablesTwo)
{
	// STARTING TIME
	auto start = std::chrono::system_clock::now();
	std::cout << "Solving using Kociemba's two phase algorithm." << std::endl
		<< "IDA Phase one... ";

	std::vector<char> movesOne;
	std::vector<char> movesTwo;

	// Phase 1
	position = IterativeDeepening(position, 12, 1, movesOne, subGroupOne, indexIdsOne, pruneTablesOne);

	// PHASE ONE TIME
	auto second = std::chrono::system_clock::now();;
	std::chrono::duration<double> elapsedSeconds = second - start;
	std::cout << "Finished in " << elapsedSeconds.count() << "s " << std::endl
		<< "IDA Phase two... ";

	// Phase 2
	position = IterativeDeepening(position, 18, 0, movesTwo, subGroupTwo, indexIdsTwo, pruneTablesTwo);
	
	// PHASE TWO TIME
	elapsedSeconds = std::chrono::system_clock::now() - second;
	std::cout << "Finished in " << elapsedSeconds.count() << "s" << std::endl;

	// Moves conversion.
	int movesOneLength = movesOne.size();
	int movesTwoLength = movesTwo.size();
	int movesLength = movesOneLength + movesTwoLength;
	
	moves.resize(movesLength);
	for (int i = 0; i < movesOneLength; i++) {
		moves[i] = movesOne[i];
	}
	for (int i = 0; i < movesTwoLength; i++) {
		int movesPhaseTwoIndex = i + movesOneLength;
		moves[movesPhaseTwoIndex] = movesTwo[i];
	}

	// CLOSING TIME
	elapsedSeconds = std::chrono::system_clock::now() - start;
	std::cout << "Solved in " << elapsedSeconds.count() << "s total." << std::endl << std::endl;

	return position;
}

// Calls treesearch with increasing depth, for breadth-first results.
// Makes sure that the first found solution is the shortest actual solution for that sub-problem.
Cube3 IterativeDeepening(Cube3 position, char maxDepth, char solveState, std::vector<char>& moves, const std::vector<char>& possibleMoves, const std::vector<char>& indexIds, const std::vector<std::vector<char>*>& pruneTables)
{
	// One move each layer of depth, initialize with a number not corresponding to any move.
	int movesSize = maxDepth * MOVE_STRIDE;
	moves.resize(movesSize);
	for (int i = 0; i < maxDepth * MOVE_STRIDE; i++) {
		moves[i] = -1;
	}

	// Iterative deepening
	for (int i = 0; i <= maxDepth; i++) {
		std::cout << i << "... ";
		position = Treesearch(position, maxDepth, i, 0, solveState, moves, possibleMoves, indexIds, pruneTables);

		if (position.IsSolved(solveState)) {
			return position;
		}
	}
	std::cout << "Solution not found in " << (int)maxDepth << " layers of depth!" << std::endl;
	return position;
}

// Recursive function to be used in combination with Iterative deepening (IDA) - Only tests if solved at last function call (depth = 0)
Cube3 Treesearch(Cube3 position, char maxDepth, char treeMaxDepth, char depth, char solveState, std::vector<char>& moves, const std::vector<char>& possibleMoves, const std::vector<char>& indexIds, const std::vector<std::vector<char>*>& pruneTables)
{
	// Iterative deepening...
	if (depth == treeMaxDepth) {
		return position;
	}

	else {
		// Prunes - Don't go deeper if we can already know it's unsolvable with remaining depth.
		for (int i = 0; i < pruneTables.size(); i++) {
			int indexId = indexIds[i];
			int index = position.PruneIndex(indexId);

			int remainingDepth = maxDepth - depth;
			int pruneValue = (*pruneTables[i])[index];

			if (remainingDepth < pruneValue) {
				return position;
			}
		}

		// Call each possible move from this position.
		for (int i = 0; i < possibleMoves.size() - 1; i += MOVE_STRIDE) {
			int possiblePowIndex = i + 1;

			char nextMove = possibleMoves[i];
			char nextPower = possibleMoves[possiblePowIndex];

			int movesIndex = depth * MOVE_STRIDE;
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
			Cube3 result = Treesearch(position, maxDepth, treeMaxDepth, depth + 1, solveState, moves, possibleMoves, indexIds, pruneTables);

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

void GeneratePruneTable(std::vector<char>& table, size_t tableSize, std::vector<char> possibleMoves, char indexId, std::string name) {
	// Pruning table initialization
	table.resize(tableSize, -1);

	// First in first out queues, separate for holding corresponding depth
	std::queue<Cube3> fifoQueue;
	std::queue<char> fifoQueueDepth;
	fifoQueue.push(Cube3());
	fifoQueueDepth.push(0);
	char depth;

	// Set first table index
	table[fifoQueue.front().PruneIndex(indexId)] = 0;

	// STARTING TIME
	auto start = std::chrono::system_clock::now();
	auto sinceLast = std::chrono::system_clock::now();
	std::cout << "Generating " << name << ", [DEPTH (TIME)]" << std::endl;

	while (!(fifoQueue.empty())) {
		// Grab queued position
		Cube3 position = fifoQueue.front();
		depth = fifoQueueDepth.front();
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

		// CLOSING TIME
		if (!(fifoQueueDepth.empty()) && fifoQueueDepth.front() != depth) {
			std::chrono::duration<double> elapsedSecondsSinceLast = std::chrono::system_clock::now() - sinceLast;

			std::cout << (int)depth << " (" << elapsedSecondsSinceLast.count() << "s), ";

			sinceLast = std::chrono::system_clock::now();
		}
	}

	std::chrono::duration<double> elapsedSeconds = std::chrono::system_clock::now() - start;
	std::chrono::duration<double> elapsedSecondsSinceLast = std::chrono::system_clock::now() - sinceLast;
	std::cout << (int)depth << " (" << elapsedSecondsSinceLast.count() << "s)" << std::endl
		<< "Finished generation in " << elapsedSeconds.count() << "s total." << std::endl;
}

// EdgeTwistIndex * CornerTwistIndex
void GeneratePhaseOneEC(std::vector<char>& table)
{
	GeneratePruneTable(table, (size_t)EDGE_TWIST_INDEX_SIZE * (size_t)CORNER_TWIST_INDEX_SIZE, subGroupOne, PRUNE_EDGECORNERTWIST, "table one");
}

// EdgeTwistIndexMax * UDSliceCombinationIndexMax
void GeneratePhaseOneEUD(std::vector<char>& table)
{
	GeneratePruneTable(table, (size_t)EDGE_TWIST_INDEX_SIZE * (size_t)UDSLICE_COMBINATION_INDEX_SIZE, subGroupOne, PRUNE_EDGETWIST_UDCOMB, "table two");
}

// CornerTwistIndex * UDSliceCombinationIndexMax
void GeneratePhaseOneCUD(std::vector<char>& table)
{
	GeneratePruneTable(table, (size_t)CORNER_TWIST_INDEX_SIZE * (size_t)UDSLICE_COMBINATION_INDEX_SIZE, subGroupOne, PRUNE_CORNERTWIST_UDCOMB, "table three");
}

// CornerPermIndex * UDEdgePermIndex
void GeneratePhaseTwo(std::vector<char>& table)
{
	GeneratePruneTable(table, CORNER_PERM_INDEX_SIZE * UDEDGE_PERM_INDEX_SIZE, subGroupTwo, PRUNE_PHASETWO, "phase two");
}

void WriteToFile(std::vector<char>& table, std::string fileName) {
	// STARTING TIME
	auto start = std::chrono::system_clock::now();
	std::cout << "Writing to disk, as " << fileName << "... ";

	// Writing to file
	std::ofstream fOut(fileName);
	for (long int i = 0; i < table.size(); i++) {
		fOut << table[i];
	}
	fOut.close();

	// CLOSING TIME
	std::chrono::duration<double> elapsedSeconds = std::chrono::system_clock::now() - start;
	std::cout << " Finished in " << elapsedSeconds.count() << "s" << std::endl;
}

void ReadFromFile(std::vector<char>& table, std::string fileName) {
	// STARTING TIME
	auto start = std::chrono::system_clock::now();
	std::cout << "Reading disk, from " << fileName << "... ";

	// Reading from file
	std::ifstream fIn(fileName);
	char c;
	while (fIn.get(c)) {
		table.push_back(c);
	}

	// CLOSING TIME
	std::chrono::duration<double> elapsedSeconds = std::chrono::system_clock::now() - start;
	std::cout << " Finished in " << elapsedSeconds.count() << "s" << std::endl;
}

void GenerateTables(std::vector<char>& tablePhaseOneEC, std::vector<char>& tablePhaseOneEUD, std::vector<char>& tablePhaseOneCUD, std::vector<char>& tablePhaseTwo) {
	std::ifstream temp;

	// Phase one
	if (tablePhaseOneEC.empty()) {
		temp.open("phaseone-EC.prun");
		if (temp.good()) {
			tablePhaseOneEC.reserve(EDGE_TWIST_INDEX_SIZE * CORNER_TWIST_INDEX_SIZE);
			ReadFromFile(tablePhaseOneEC, "phaseone-EC.prun");
		}
		else {
			GeneratePhaseOneEC(tablePhaseOneEC);
			WriteToFile(tablePhaseOneEC, "phaseone-EC.prun");
		}
		temp.close();
	}
	else {
		std::cout << "Skipped phaseone-EC.prun. (Already initialized)" << std::endl;
	}
	std::cout << std::endl;

	if (tablePhaseOneEUD.empty()) {
		temp.open("phaseone-EUD.prun");
		if (temp.good()) {
			tablePhaseOneEUD.reserve(UDSLICE_COMBINATION_INDEX_SIZE * EDGE_TWIST_INDEX_SIZE);
			ReadFromFile(tablePhaseOneEUD, "phaseone-EUD.prun");
		}
		else {
			GeneratePhaseOneEUD(tablePhaseOneEUD);
			WriteToFile(tablePhaseOneEUD, "phaseone-EUD.prun");
		}
		temp.close();
	}
	else {
		std::cout << "Skipped phaseone-EUD.prun. (Already initialized)" << std::endl;
	}
	std::cout << std::endl;

	if (tablePhaseOneCUD.empty()) {
		temp.open("phaseone-CUD.prun");
		if (temp.good()) {
			tablePhaseOneCUD.reserve(UDSLICE_COMBINATION_INDEX_SIZE * CORNER_TWIST_INDEX_SIZE);
			ReadFromFile(tablePhaseOneCUD, "phaseone-CUD.prun");
		}
		else {
			GeneratePhaseOneCUD(tablePhaseOneCUD);
			WriteToFile(tablePhaseOneCUD, "phaseone-CUD.prun");
		}
		temp.close();
	}
	else {
		std::cout << "Skipped phaseone-CUD.prun. (Already initialized)" << std::endl;
	}
	std::cout << std::endl;

	// Phase two
	if (tablePhaseTwo.empty()) {
		temp.open("phasetwo.prun");
		if (temp.good()) {
			tablePhaseTwo.reserve(CORNER_PERM_INDEX_SIZE * UDEDGE_PERM_INDEX_SIZE);
			ReadFromFile(tablePhaseTwo, "phasetwo.prun");
		}
		else {
			GeneratePhaseTwo(tablePhaseTwo);
			WriteToFile(tablePhaseTwo, "phasetwo.prun");
		}
		temp.close();
	}
	else {
		std::cout << "Skipped phasetwo.prun. (Already initialized)" << std::endl;
	}
	std::cout << std::endl;
}

void printFormatted(std::vector<char>& moves)
{
	if (moves.empty()) {
		return;
	}

	const char faces[FACES] = { 'U', 'L', 'F', 'R', 'B', 'D' };
	const char power[3] = { ' ', '2', 39 };

	std::cout << faces[moves[0]] << power[moves[1] - 1];
	for (int i = MOVE_STRIDE; i < moves.size(); i += MOVE_STRIDE) {
		int powIndex = i + 1;

		if (moves[i] != -1) {
			std::cout << " -> " << faces[moves[i]] << power[moves[powIndex] - 1];
		}
	}
	std::cout << std::endl << std::endl;
}