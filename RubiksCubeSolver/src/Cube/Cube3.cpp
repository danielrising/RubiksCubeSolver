#include "rcspch.h"

#include "Cube3.h"

const unsigned char faces[FACES] = { 'U', 'L', 'F', 'R', 'B', 'D' };

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

void Cube3::GenerateMatrix()
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
		{'Y', 'R', 'G'},
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
		{'Y', 'G'},
	};

	// Sides from cube3::faces has colors;
	const char faceColors[FACES] = { 'W', 'O', 'G', 'R', 'B', 'Y' };

	// Facelet coords for corners, using priorities from cornerColors.
	// Coordinates uses convention from showLayout (in ConsoleRender) and rendered.
	const unsigned short cornerFaceletCoords[C_SIZE][3][2] = {
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
	const unsigned short edgeFaceletCoords[E_SIZE][2][2] = {
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
		{{5, 1}, {2, 7}},
	};

	// Generate corner pieces
	for (int i = 0; i < C_SIZE; i++) {
		for (int j = 0; j < 3; j++) {
			generatedMatrix[cornerFaceletCoords[i][j][0]][cornerFaceletCoords[i][j][1]] = cornerColors[c[i].GetId()][(c[i].GetR() + j) % 3];
		}
	}

	// Generate edge pieces
	for (int i = 0; i < E_SIZE; i++) {
		for (int j = 0; j < 2; j++) {
			generatedMatrix[edgeFaceletCoords[i][j][0]][edgeFaceletCoords[i][j][1]] = edgeColors[e[i].GetId()][(e[i].GetR() + j) % 2];
		}
	}

	// Generate middle pieces
	for (int i = 0; i < FACES; i++) {
		generatedMatrix[i][4] = faceColors[i];
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

void Cube3::Scramble(short int amount, bool log)
{
	// Initilize random seed using current time
	srand(time(NULL));

	for (int i = 0; i < amount; i++) {

		short int move = rand() % FACES; // Random move, 0 to (including) 5
		short int pow = 1 + rand() % 3; // Random power of that move, 1 to (inluding) 3

		Rotate(move, pow);

		if (log) {
			std::cout << move << "*" << pow << " : ";
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
}

void Cube3::ConsoleRender()
{
	// Rendering 3D cube in 2D with following convention
	//		B
	//	O	W	R
	//		G	 
	//		Y
	// -1 = blank, rest follows index from faceColors
	const signed short showLayout[4][3] = {
		{-1, 4, -1},
		{ 1, 0, 3 },
		{-1, 2, -1},
		{-1, 5, -1}
	};

	// Update cube state
	GenerateMatrix();

	// Show
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 3; k++) {
				if (showLayout[i][k] == -1) {
					std::cout << "      ";
				}
				else {
					for (int l = 0; l < 3; l++) {
						std::cout << generatedMatrix[showLayout[i][k]][j * 3 + l] << " ";
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

void IterativeDeepening(Cube3 position, short int maxDepth, short int solveState, std::vector<short int>& moves)
{
	moves.resize(maxDepth);
	for (int i = 0; i < maxDepth; i++) {
		moves[i] = -1;
	}
	for (int i = 0; i < maxDepth; i++) {
		std::cout << "Current depth: " << i << std::endl;
		position = Treesearch(position, i, i, solveState, moves);

		if (position.IsSolved(solveState)) {
			position.ConsolePrint();
			position.ConsoleRender();
			break;
		}
	}
}

Cube3 Treesearch(Cube3 position, short int maxDepth, short int depth, short int solveState, std::vector<short int>& moves)
{
	if (depth == 0) {
		if (position.IsSolved(solveState)) {
			return position;
		}
	}
	else if (depth > 0){
		if (true && true) { // prunes
			for (int i = 0; i < FACES; i++) {
				for (int j = 1; j < 4; j++) {
					position.Rotate(i, j);
					moves[maxDepth - depth] = i + (j - 1) * FACES;

					Cube3 result = Treesearch(position, maxDepth, depth - 1, solveState, moves);

					if (result.IsSolved(solveState)) {
						return result;
					}
					else {
						position.Rotate(i, 4 - j);
					}
				}
			}
			return position;
		}
		return position;
	}
	return position;
}