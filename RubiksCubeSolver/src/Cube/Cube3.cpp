#include "Cube3.h"

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
const unsigned char Cube3::edgeTurnPerm[FACES][4] = { {0, 2, 3, 1}, {3, 7, 11, 6}, {2, 5, 10, 7}, {9, 11, 10, 8}, {0, 4, 8, 5}, {1, 6, 9, 4} };
const unsigned char Cube3::cornerTurnPerm[FACES][4] = { {0, 1, 3, 2}, {2, 3, 7, 6}, {3, 1, 5, 7}, {4, 6, 7, 5}, {1, 0, 4, 5}, {0, 2, 6, 4} };

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