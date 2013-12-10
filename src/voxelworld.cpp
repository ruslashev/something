#include "voxelworld.hpp"

#define INFO_COLOR  "\x1b[36m"
#define CLEAR_COLOR "\x1b[0m"

#define SUCCESS_STR "\x1b[32m" "✓" CLEAR_COLOR
#define FAIL_STR    "\x1b[31m" "✗" CLEAR_COLOR

void VoxelWorld::fromFile(const char *filename)
{
	if (!readVoxels(filename))
		exit(1);
	createMesh();
}

bool VoxelWorld::readVoxels(const char *filename)
{
	printf("Loading VXL model %s%-12s%s\t\t\t", INFO_COLOR, filename, CLEAR_COLOR);

	std::ifstream ifs(filename, std::ifstream::in);
	if (!ifs) {
		printf(FAIL_STR " fail: %d bad: %d\n", ifs.fail(), ifs.bad());
		return false;
	}

	std::string line;
	while (getline(ifs, line)) {
		if (line.substr(0, 2) == "v ") {
			std::istringstream s(line.substr(2));
			int x, y, z;
			s >> x; s >> y; s >> z;
			voxels.push_back(AABB { x, y, z, 1, 1, 1 });
		}
	}

	puts(SUCCESS_STR);
	return true;
}

void VoxelWorld::createMesh()
{
	for (AABB &box : voxels) {
		const int x = box.x, y = box.y, z = box.z;
		std::vector<glm::vec4> *verts = &voxelMesh.vertices;
		// sort of alias, to not to write voxelMesh.vertices.push_back(..

		verts->push_back(glm::vec4(x  , y+1, z  , 1));
		verts->push_back(glm::vec4(x  , y  , z  , 1));
		verts->push_back(glm::vec4(x+1, y  , z  , 1));
		verts->push_back(glm::vec4(x  , y+1, z  , 1));
		verts->push_back(glm::vec4(x+1, y+1, z  , 1));
		verts->push_back(glm::vec4(x+1, y  , z  , 1));
		verts->push_back(glm::vec4(x  , y+1, z+1, 1));
		verts->push_back(glm::vec4(x  , y  , z+1, 1));
		verts->push_back(glm::vec4(x+1, y  , z+1, 1));
		verts->push_back(glm::vec4(x  , y+1, z+1, 1));
		verts->push_back(glm::vec4(x+1, y+1, z+1, 1));
		verts->push_back(glm::vec4(x+1, y  , z+1, 1));

		verts->push_back(glm::vec4(x  , y+1, z  , 1));
		verts->push_back(glm::vec4(x  , y  , z  , 1));
		verts->push_back(glm::vec4(x  , y  , z+1, 1));
		verts->push_back(glm::vec4(x  , y+1, z  , 1));
		verts->push_back(glm::vec4(x  , y+1, z+1, 1));
		verts->push_back(glm::vec4(x  , y  , z+1, 1));
		verts->push_back(glm::vec4(x+1, y+1, z  , 1));
		verts->push_back(glm::vec4(x+1, y  , z  , 1));
		verts->push_back(glm::vec4(x+1, y  , z+1, 1));
		verts->push_back(glm::vec4(x+1, y+1, z  , 1));
		verts->push_back(glm::vec4(x+1, y+1, z+1, 1));
		verts->push_back(glm::vec4(x+1, y  , z+1, 1));

		verts->push_back(glm::vec4(x  , y  , z+1, 1));
		verts->push_back(glm::vec4(x  , y  , z  , 1));
		verts->push_back(glm::vec4(x+1, y  , z  , 1));
		verts->push_back(glm::vec4(x  , y  , z+1, 1));
		verts->push_back(glm::vec4(x+1, y  , z+1, 1));
		verts->push_back(glm::vec4(x+1, y  , z  , 1));
		verts->push_back(glm::vec4(x  , y+1, z+1, 1));
		verts->push_back(glm::vec4(x  , y+1, z  , 1));
		verts->push_back(glm::vec4(x+1, y+1, z  , 1));
		verts->push_back(glm::vec4(x  , y+1, z+1, 1));
		verts->push_back(glm::vec4(x+1, y+1, z+1, 1));
		verts->push_back(glm::vec4(x+1, y+1, z  , 1));

		for (int i = 1; i <= 6; i++) {
			voxelMesh.texCoords.push_back(glm::vec2(0, 1));
			voxelMesh.texCoords.push_back(glm::vec2(0, 0));
			voxelMesh.texCoords.push_back(glm::vec2(1, 0));
			voxelMesh.texCoords.push_back(glm::vec2(0, 1));
			voxelMesh.texCoords.push_back(glm::vec2(1, 1));
			voxelMesh.texCoords.push_back(glm::vec2(1, 0));
		}
	}
}
