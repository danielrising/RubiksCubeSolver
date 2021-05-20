#include "rcspch.h"

#include "Voxel.h"

Voxel::Voxel()
{
	voxel = 0;
}

void Voxel::SetId(uint8_t newId)
{
	voxel = (newId << 4) | GetR();
}

void Voxel::SetR(uint8_t newR)
{
	voxel = (GetId() << 4) | newR;
}

uint8_t Voxel::GetId()
{
	return voxel >> 4;
}

uint8_t Voxel::GetR()
{
	return (voxel & 0b00001111);
}