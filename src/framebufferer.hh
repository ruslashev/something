#ifndef FRAMEBUFFERER_HH
#define FRAMEBUFFERER_HH

#include <GL/glew.h>
#include "graphics.hh"

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

