#pragma once

class Cube
{

public:

	struct voxel {

		unsigned char id;
		unsigned char rotation;

	};

	Cube();

private:

	voxel corners[8];
	voxel edges[12];

};