#pragma once

#include "rcspch.h"
#include "Voxel/Voxel.h"

#define FACES 6
#define C_SIZE 8
#define E_SIZE 12
#define C_PER_FACE 4
#define E_PER_FACE 4

/*USES MOVE VALUES*/
#define C_GET_SIDE_INDEX(x) x % FACES * C_PER_FACE
// Equals the actual index for move index in cornerTurnPerm
#define C_GET_MOVE_INDEX(x, i) C_GET_SIDE_INDEX(x) + ((int)(ceil((float)(x + 1) / FACES)) + i) % 4 // Modulo to switch over
#define E_GET_SIDE_INDEX(x) x % FACES * E_PER_FACE
// Equals the actual index for move index in cornerTurnPerm
#define E_GET_MOVE_INDEX(x, i) E_GET_SIDE_INDEX(x) + ((int)(ceil((float)(x + 1) / FACES)) + i) % 4 // Modulo to switch over

/*3X3X3 CUBE CLASS*/
class Cube3
{
private:
	Voxel c[C_SIZE];
	Voxel e[E_SIZE];

	static const unsigned char faces[FACES];
	static const unsigned char cornerTurnPerm[FACES * C_PER_FACE];
	static const unsigned char edgeTurnPerm[FACES * E_PER_FACE];
	/*
	static const unsigned char cornerTwistPerm[];
	static const unsigned char edgeTwistPerm[];
	*/
public:
	Cube3();

	void Rotate(const unsigned char &mov);

	friend std::ostream& operator <<(std::ostream& os, const Cube3& cube);
};