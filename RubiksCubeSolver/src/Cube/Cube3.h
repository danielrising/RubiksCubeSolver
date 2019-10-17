#pragma once

#define C_SIZE 8
#define E_SIZE 12
#define FACES 6
#define E_PER_FACE 4
#define C_PER_FACE 4

class Cube3
{
private:
	struct voxel
	{
	private:
		unsigned char id, r;

	public:
		inline void SetId(const unsigned char& newId);
		inline void SetR(const unsigned char& id);

		inline const unsigned char& GetId();
		inline const unsigned char& GetR();
	};

	voxel c[C_SIZE];
	voxel e[E_SIZE];

	static const unsigned char faces[FACES];
	static const unsigned char edgeTurnPerm[FACES][E_PER_FACE];
	static const unsigned char cornerTurnPerm[FACES][C_PER_FACE];

public:
	Cube3();

	void MoveEdges(int mov);

};