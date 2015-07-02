#ifndef FRAMEBUFFERER_HH
#define FRAMEBUFFERER_HH

#include "graphics.hh"

#include <GL/glew.h>

class Framebufferer
{
	void createTexture();
	void createRBO();
	void bind();
	void createShaders();
public:
	GLuint FBO, textToRenderTo, RBO;
	ArrayBuffer<GLfloat> vbo;
	GLuint ppProg, vertShaderPP, fragShaderPP;
	GLint attrib_vCoordPP, uniformFBOtext;

	Framebufferer();
	~Framebufferer();
};

#endif

