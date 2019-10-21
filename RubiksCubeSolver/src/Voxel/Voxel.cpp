#include "rcspch.h"
#include "Voxel.h"

void Voxel::SetId(const unsigned char& newId)
{
	_id = newId;
}

void Voxel::SetR(const unsigned char& newR)
{
	_r = newR;
}

const unsigned char& Voxel::GetId()
{
	return _id;
}

const unsigned char& Voxel::GetR()
{
	return _r;
}


std::string Voxel::ToString() const
{
	return ("[" + std::to_string(_id) + ", " + std::to_string(_r) + "]");
}