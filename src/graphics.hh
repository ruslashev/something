#ifndef GRAPHICS_HH
#define GRAPHICS_HH

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <new>
#include <vector>
#include <string>
#include <sstream>

template <class Vec>
class ArrayBuffer
{
	GLuint type;
public:
	GLuint id;

	void Construct(GLenum ntype)
	{
		type = ntype;
		id = 0;
		glGenBuffers(1, &id);
	}
	void Upload(std::vector<Vec> *data)
	{
		Bind();
		glBufferData(type,
				data->size()*sizeof(data->at(0)),
				data->data(),
				GL_STATIC_DRAW);
		glBindBuffer(type, 0);
	}
	void Bind() const
	{
		glBindBuffer(type, id);
	}
	~ArrayBuffer()
	{
		if (id)
			glDeleteBuffers(1, &id);
	}
};

class Mesh
{
	ArrayBuffer<glm::vec4> vbo;
	ArrayBuffer<GLushort> ebo;
	ArrayBuffer<glm::vec2> tbo;
	GLuint textureID;
public:
	~Mesh();

	GLint textUnif;

	std::vector<glm::vec4> vertices;
	std::vector<GLushort> elements;
	std::vector<glm::vec2> texCoords;

	bool FromOBJ(const char *filename);
	void Upload();
	void Draw(GLint &attrib_vCoord, GLint &attrib_texCoord);
};

GLuint CreateShader(GLenum type, const char *src);
GLuint LinkShadersToProgram(GLuint &vertShader, GLuint &fragShader);
GLint BindAttribute(const char *name, GLuint &glslProgram);
GLint BindUniform(const char *name, GLuint &glslProgram);
void printLog(GLuint &shaderOrProg);
void printStatus(bool success);

#endif

