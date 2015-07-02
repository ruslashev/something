#include "graphics.hh"
#include "main.hh"
#include "voxelworld.hh"

bool aabbsCollide(AABB a, AABB b)
{
	return (a.x + a.w >= b.x) &&
           (b.x + b.w >= a.x) &&
           (a.y + a.h >= b.y) &&
           (b.y + b.h >= a.y) &&
           (a.z + a.d >= b.z) &&
           (b.z + b.d >= a.z);
}

void VoxelWorld::fromFile(const char *filename)
{
	if (!readVoxels(filename))
		exit(1);
	createMesh();
}

bool VoxelWorld::readVoxels(const char *filename)
{
	printf("Loading VXL model ");
	info(filename);
	printf("... ");

	std::ifstream ifs(filename, std::ifstream::in);
	if (!ifs) {
		fail();
		printf("failed to open file. fail: %d bad: %d\n",
				ifs.fail(), ifs.bad());
		throw;
	}

	std::string line;
	while (getline(ifs, line)) {
		if (line.substr(0, 2) == "v ") {
			std::istringstream s(line.substr(2));
			int x, y, z;
			s >> x; s >> y; s >> z;
			voxels.push_back(AABB { x, y, z, 1., 1., 1. });
		}
	}

	success();
	return true;
}

void VoxelWorld::createMesh()
{
	for (AABB &box : voxels) {
		const int x = box.x, y = box.y, z = box.z;
		std::vector<glm::vec4> *verts = &voxelMesh.vertices;
		// alias, to not to write voxelMesh.vertices.push_back(..

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

