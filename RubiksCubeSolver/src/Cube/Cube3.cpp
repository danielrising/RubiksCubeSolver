#include "Cube3.h"
#include "cmath"

inline void Cube3::voxel::SetId(const unsigned char& newId)
{
	id = newId;
}

inline void Cube3::voxel::SetR(const unsigned char& newR)
{
	r = newR;
}

inline const unsigned char& Cube3::voxel::GetId()
{
	return id;
}

inline const unsigned char& Cube3::voxel::GetR()
{
	return r;
}

const unsigned char Cube3::faces[FACES] = { 'U', 'F', 'R', 'D', 'B', 'L' };

const unsigned char Cube3::edgeTurnPerm[FACES * E_PER_FACE] = { 
	0, 2, 3, 1,		// U
	3, 7, 11, 6,	// F
	2, 5, 10, 7,	// R
	9, 11, 10, 8,	// D
	0, 4, 8, 5,		// B
	1, 6, 9, 4		// L
};

const unsigned char Cube3::cornerTurnPerm[FACES * C_PER_FACE] = { 
	0, 1, 3, 2,		// U
	2, 3, 7, 6,		// F
	3, 1, 5, 7,		// R
	4, 6, 7, 5,		// D
	1, 0, 4, 5,		// B
	0, 2, 6, 4		// L
};

Cube3::Cube3()
{
	for (int i = 0; i < C_SIZE; i++)
	{
		c[i].SetId(i);
		c[i].SetR(0);
	}

	for (int i = 0; i < E_SIZE; i++)
	{
		e[i].SetId(i);
		e[i].SetR(0);
	}
}

// mov 0 = U, mov 1 = F ... mov 6 = U2 ... mov 12 = U3 (U')
void Cube3::Rotate(const unsigned char &mov)
{
	// edges
	voxel eTemp = e[cornerTurnPerm[E_GET_MOVE_INDEX(mov, 0)]];
	e[cornerTurnPerm[E_GET_SIDE_INDEX(mov)] + 0] = e[cornerTurnPerm[E_GET_MOVE_INDEX(mov, 1)]];
	e[cornerTurnPerm[E_GET_SIDE_INDEX(mov)] + 1] = e[cornerTurnPerm[E_GET_MOVE_INDEX(mov, 2)]];
	e[cornerTurnPerm[E_GET_SIDE_INDEX(mov)] + 2] = e[cornerTurnPerm[E_GET_MOVE_INDEX(mov, 3)]];
	e[cornerTurnPerm[E_GET_SIDE_INDEX(mov)] + 3] = eTemp;

	// corners
	voxel cTemp = c[edgeTurnPerm[C_GET_MOVE_INDEX(mov, 0)]];
	c[cornerTurnPerm[C_GET_SIDE_INDEX(mov)] + 0] = c[cornerTurnPerm[C_GET_MOVE_INDEX(mov, 1)]];
	c[cornerTurnPerm[C_GET_SIDE_INDEX(mov)] + 1] = c[cornerTurnPerm[C_GET_MOVE_INDEX(mov, 2)]];
	c[cornerTurnPerm[C_GET_SIDE_INDEX(mov)] + 2] = c[cornerTurnPerm[C_GET_MOVE_INDEX(mov, 3)]];
	c[cornerTurnPerm[C_GET_SIDE_INDEX(mov)] + 3] = cTemp;

}