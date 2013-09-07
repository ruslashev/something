#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <GL/glew.h>
#include <fstream>

#include <new>

#include <vector>
#include <glm/glm.hpp>
#include <string>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>

class Mesh
{
private:
	GLuint VBO, IBO;
	std::vector<glm::vec4> vertices;
	std::vector<GLushort> elements;

public:
	Mesh() : VBO(0), IBO(0) {};
	~Mesh();

	void FromOBJ(const char *filename);
	void Upload();
	void Draw(GLint &attrib_vCoord);
};

bool loadOBJ(const char* filename, std::vector<glm::vec4> &vertices, std::vector<GLushort> &elements);
GLuint LoadShader(GLenum type, const char *src);
GLuint LinkShaders(GLuint &vertShader, GLuint &fragShader);
GLint BindAttribute(const char *name, GLuint &glslProgram);
GLint BindUniform(const char *name, GLuint &glslProgram);
void printLog(GLuint &shaderOrProg);

#endif

