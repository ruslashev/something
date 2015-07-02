#ifndef GRAPHICS_HH
#define GRAPHICS_HH

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
	GLuint VBO, IBO, VBO_tex, textureID;
public:
	Mesh() : VBO(0), IBO(0), VBO_tex(0) {};
	~Mesh();

	GLint textUnif;

	std::vector<glm::vec4> vertices;
	std::vector<GLushort> elements;
	std::vector<glm::vec2> texCoords;

	bool FromOBJ(const char *filename);
	void Upload();
	void Draw(GLint &attrib_vCoord, GLint &attrib_texCoord);
};

class VertexBuffer
{
	GLuint id;
public:
	void Construct();
	void Bind();
	void Unbind();
};

GLuint CreateShader(GLenum type, const char *src);
GLuint LinkShadersToProgram(GLuint &vertShader, GLuint &fragShader);
GLint BindAttribute(const char *name, GLuint &glslProgram);
GLint BindUniform(const char *name, GLuint &glslProgram);
void printLog(GLuint &shaderOrProg);
void printStatus(bool success);

#endif

