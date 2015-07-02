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

	~ArrayBuffer();

	void Construct(GLenum ntype);
	void Upload(std::vector<Vec> *data);
	void Bind() const;
	void Unbind() const;
};

template <class Vec>
void ArrayBuffer<Vec>::Construct(GLenum ntype)
{
	type = ntype;
	id = 0;
	glGenBuffers(1, &id);
}
template <class Vec>
ArrayBuffer<Vec>::~ArrayBuffer()
{
	if (id)
		glDeleteBuffers(1, &id);
}
template <class Vec>
void ArrayBuffer<Vec>::Upload(std::vector<Vec> *data)
{
	Bind();
	glBufferData(type,
			data->size()*sizeof(data->at(0)),
			data->data(),
			GL_STATIC_DRAW);
	Unbind();
}
template <class Vec>
void ArrayBuffer<Vec>::Bind() const
{
	glBindBuffer(type, id);
}
template <class Vec>
void ArrayBuffer<Vec>::Unbind() const
{
	glBindBuffer(type, 0);
}


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

