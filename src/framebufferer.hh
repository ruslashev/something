#ifndef FRAMEBUFFERER_HH
#define FRAMEBUFFERER_HH

class Framebufferer
{
private:
	void CreateTexture();
	void CreateRBO();
	void BindToFBO();
	void CreateShaders();
public:
	GLuint FBO, textToRenderTo, RBO;
	GLuint VBO_for_FBO;
	GLuint ppProg, vertShaderPP, fragShaderPP;
	GLint attrib_vCoordPP, uniformFBOtext;

	Framebufferer();
	~Framebufferer();

};

#endif

