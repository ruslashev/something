#ifndef VOXELWORLD_HH
#define VOXELWORLD_HH

#include <iostream>
#include <sstream>

#include "graphics.hh"

struct AABB { int x, y, z; double w, h, d; };

bool aabbsCollide(AABB a, AABB b);

class VoxelWorld
{
private:
	bool readVoxels(const char *filename);
	void createMesh();
public:
	std::vector<AABB> voxels;
	Mesh voxelMesh;

	void fromFile(const char *filename);
};

#endif

