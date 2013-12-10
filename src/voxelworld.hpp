#ifndef VOXELWORLD_HPP
#define VOXELWORLD_HPP

#include <iostream>
#include <sstream>

#include "graphics.hpp"

struct AABB { int x, y, z; unsigned int w, h, d; };

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

