#pragma once

struct Voxel
{
private:
	uint8_t voxel;

public:
	Voxel();

	void SetId(uint8_t newId);
	void SetR(uint8_t newId);

	uint8_t GetId();
	uint8_t GetR();
};