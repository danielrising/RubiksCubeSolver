#pragma once

struct Voxel
{
private:
	unsigned char _id, _r;

public:
	void SetId(const unsigned char& newId);
	void SetR(const unsigned char& newId);

	const unsigned char& GetId();
	const unsigned char& GetR();

	std::string ToString() const;
};