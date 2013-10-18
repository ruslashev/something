#include "graphics.hpp"
#include "main.hpp"
#include "window.hpp"
#include "camera.hpp"

using namespace std;

int main()
{
	Window window(800, 600, "Something");
	glEnable(GL_DEPTH_TEST);
	glLineWidth(2);

#define GLSL(src) "#version 120\n" #src
	const char *vertShaderSrc = GLSL(
		uniform mat4 MVP;
		attribute vec4 vCoord;
		attribute vec2 texCoord;
		varying vec2 ftexCoord;

		void main() {
			ftexCoord = texCoord;
			gl_Position = MVP*vCoord;
		}
	);
	const char *fragShaderSrc = GLSL(
		uniform sampler2D tex0;
		varying vec2 ftexCoord;
		void main() {
			vec2 inverseCoord = vec2(ftexCoord.x, 1.0-ftexCoord.y);
			gl_FragColor = texture2D(tex0, inverseCoord);
		}
	);

	Mesh mapMesh;
	mapMesh.FromVXL("level.vxl");
	mapMesh.Upload();

	Mesh bat;
	bat.FromOBJ("bat.obj");
	bat.Upload();

	Camera cam;
	cam.position = glm::vec3(-10, 0, 0);
	cam.pitch = cam.yaw = 0;

	GLuint vertShader = CreateShader(GL_VERTEX_SHADER, vertShaderSrc);
	GLuint fragShader = CreateShader(GL_FRAGMENT_SHADER, fragShaderSrc);
	if (!vertShader || !fragShader)
		return 2;

	GLuint glslProgram = LinkShaders(vertShader, fragShader);
	if (!glslProgram)
		return 2;

	GLint attrib_vCoord = BindAttribute("vCoord", glslProgram);
	GLint attrib_texCoord = BindAttribute("texCoord", glslProgram);
	if (attrib_vCoord == -1 || attrib_texCoord == -1)
		return 2;

	GLint uniformMVP = BindUniform("MVP", glslProgram);
	if (uniformMVP == -1)
		return 2;
	mapMesh.textUnif = BindUniform("tex0", glslProgram);
	if (mapMesh.textUnif == -1)
		return 2;

	// Framebuffer setting up -----------------------------------------------------
	GLuint FBO, textToRenderTo, RBO;
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &textToRenderTo);
	glBindTexture(GL_TEXTURE_2D, textToRenderTo);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 800, 600, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, 800, 600);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
			textToRenderTo, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER,
			RBO);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		puts("oh shit\n");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	GLuint VBO_for_FBO;
	GLfloat screenQuad[] = { -1, -1,  1, -1, -1,  1,  1,  1 };
	glGenBuffers(1, &VBO_for_FBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_for_FBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(screenQuad), screenQuad, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLuint ppProg;

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

	GLuint vertShaderPP = CreateShader(GL_VERTEX_SHADER, vertPPShaderSrc);
	GLuint fragShaderPP = CreateShader(GL_FRAGMENT_SHADER, fragPPShaderSrc);
	if (!vertShaderPP || !fragShaderPP)
		return 2;

	ppProg = LinkShaders(vertShaderPP, fragShaderPP);
	if (!ppProg)
		return 2;

	GLint attrib_vCoordPP = BindAttribute("vCoord", ppProg);
	if (attrib_vCoordPP == -1)
		return 2;

	GLint uniformFBOtext = BindUniform("fboText", ppProg);
	if (uniformFBOtext == -1)
		return 2;
	// ----------------------------------------------------------------------------

	double realTime, simulationTime = 0;
	while (!glfwWindowShouldClose(window.win))
	{
		realTime = glfwGetTime();

		glm::mat4 anim;

		while (simulationTime < realTime) {
			simulationTime += 0.0016;
			// Update(time, dt)
			{
				anim = glm::mat4(1);
				//glm::rotate(glm::mat4(1), (float)simulationTime*45, glm::vec3(0, 1, 0));
				cam.Update(&window, 0.0016);
			}
		}

		glm::mat4 modelMat = glm::mat4(1);//glm::translate(glm::mat4(1), glm::vec3(0, 0, -4));
		glm::mat4 viewMat = cam.LookAtMat();
		const glm::mat4 projectionMat = glm::perspective(60.f,
				1.0f*window.width/window.height, 0.1f, 50.0f);
		glm::mat4 MVP = projectionMat * viewMat * modelMat * anim;

		glUseProgram(glslProgram);
		glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, glm::value_ptr(MVP));

		// Draw -------------------------------------------------------
		{
			glBindFramebuffer(GL_FRAMEBUFFER, FBO);
			// Draw to Framebuffer

			glClearColor(1, 1, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			mapMesh.Draw(attrib_vCoord, attrib_texCoord);
			// bat.Draw(attrib_vCoord, attrib_texCoord);

			// That was too easy not to do ----------
			GLfloat gridSize = 11.0f;
			glBegin(GL_LINES);
			for (GLint i = -gridSize; i <= gridSize; i++)
			{
				glVertex3f(i, 0.f,  gridSize);
				glVertex3f(i, 0.f, -gridSize);
				glVertex3f(-gridSize, 0.f, i);
				glVertex3f( gridSize, 0.f, i);
			}
			glEnd();
			// --------------------------------------

			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			glClearColor(0, 0, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glUseProgram(ppProg);
			glBindTexture(GL_TEXTURE_2D, textToRenderTo);
			glUniform1i(uniformFBOtext, GL_TEXTURE0);
			glEnableVertexAttribArray(attrib_vCoordPP);

			glBindBuffer(GL_ARRAY_BUFFER, VBO_for_FBO);
			glVertexAttribPointer(attrib_vCoordPP, 2, GL_FLOAT, GL_FALSE, 0, 0);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			glDisableVertexAttribArray(attrib_vCoordPP);
		}

		glfwSwapBuffers(window.win);
		glfwPollEvents();
	}

	// FBO --
	glDeleteBuffers(1, &VBO_for_FBO);
	glDeleteProgram(ppProg);
	glDeleteRenderbuffers(1, &RBO);
	glDeleteTextures(1, &textToRenderTo);
	glDeleteFramebuffers(1, &FBO);
	// ------

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);
	glDeleteProgram(glslProgram);
}

