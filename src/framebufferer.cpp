#include "graphics.hpp"
#include "framebufferer.hpp"

#define SUCCESS_STR "\x1b[32m" "✓" "\x1b[0m"
#define FAIL_STR    "\x1b[31m" "✗" "\x1b[0m"

Framebufferer::Framebufferer()
{
	CreateTexture();

	CreateRBO();

	BindToFBO();

	const GLfloat screenQuad[] = { -1, -1,  1, -1, -1,  1,  1,  1 };
	glGenBuffers(1, &VBO_for_FBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_for_FBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(screenQuad), screenQuad, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	CreateShaders();

	ppProg = LinkShaders(vertShaderPP, fragShaderPP);
	if (!ppProg)
		exit(2);

	attrib_vCoordPP = BindAttribute("vCoord", ppProg);
	if (attrib_vCoordPP == -1)
		exit(2);

	uniformFBOtext = BindUniform("fboText", ppProg);
	if (uniformFBOtext == -1)
		exit(2);
}

void Framebufferer::CreateTexture()
{
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &textToRenderTo);
	glBindTexture(GL_TEXTURE_2D, textToRenderTo);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGBA, 800, 600, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Framebufferer::CreateRBO()
{
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, 800, 600);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void Framebufferer::BindToFBO()
{
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture2D(
		GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textToRenderTo, 0);
	glFramebufferRenderbuffer(
		GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		puts("oh shit");
		exit(2);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebufferer::CreateShaders()
{
#define GLSL(src) "#version 120\n" #src
	const char *vertPPShaderSrc = GLSL(
		attribute vec2 vCoord;
		varying vec2 ftexCoord;
		uniform sampler2D fboText;

		void main() {
			gl_Position = vec4(vCoord, 0.0, 1.0);
			ftexCoord = (vCoord + 1.0) / 2.0;
		}
	);
	const char *fragPPShaderSrc = GLSL(
		varying vec2 ftexCoord;
		uniform sampler2D fboText;

		void main(void) {
			float dx = 2.0*(1./800);
			float dy = 2.0*(1./600);
			vec2 coord = vec2(dx*floor(ftexCoord.x/dx), dy*floor(ftexCoord.y/dy));
			gl_FragColor = texture2D(fboText, coord);
		}
	);
#undef GLSL

	vertShaderPP = CreateShader(GL_VERTEX_SHADER, vertPPShaderSrc);
	fragShaderPP = CreateShader(GL_FRAGMENT_SHADER, fragPPShaderSrc);
	if (!vertShaderPP || !fragShaderPP)
		exit(2);
}

Framebufferer::~Framebufferer()
{
	glDeleteBuffers(1, &VBO_for_FBO);
	glDeleteShader(vertShaderPP);
	glDeleteShader(fragShaderPP);
	glDeleteProgram(ppProg);
	glDeleteRenderbuffers(1, &RBO);
	glDeleteTextures(1, &textToRenderTo);
	glDeleteFramebuffers(1, &FBO);
}

