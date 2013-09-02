#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <GL/glew.h>
#include <fstream>
#include <new>

GLuint LoadShader(GLenum type, const char *src);
GLuint LinkShaders(GLuint &vertShader, GLuint &fragShader);
GLint BindAttribute(const char *name, GLuint &glslProgram);
GLint BindUniform(const char *name, GLuint &glslProgram);

#endif

