#ifndef FRAMEBUFFERER_HH
#define FRAMEBUFFERER_HH

class Framebufferer
{
	void createTexture();
	void createRBO();
	void bindToFBO();
	void createShaders();
public:
	GLuint FBO, textToRenderTo, RBO;
	GLuint VBO_for_FBO;
	GLuint ppProg, vertShaderPP, fragShaderPP;
	GLint attrib_vCoordPP, uniformFBOtext;

	Framebufferer();
	~Framebufferer();

};

#endif

