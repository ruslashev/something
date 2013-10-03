#include "main.hpp"
#include "window.hpp"
#include "graphics.hpp"

using namespace std;

int main()
{
	Window window(800, 600, "Something");
	glEnable(GL_DEPTH_TEST);

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
		varying vec2 ftexCoord;
		uniform sampler2D text;
		void main() {
			gl_FragColor = texture2D(text, vec2(ftexCoord.x, 1.0-ftexCoord.y));
		}
	);

	Mesh mapMesh;
	mapMesh.FromVXL("level.vxl");
	mapMesh.Upload();

	GLuint vertShader = LoadShader(GL_VERTEX_SHADER, vertShaderSrc);
	GLuint fragShader = LoadShader(GL_FRAGMENT_SHADER, fragShaderSrc);
	if (!vertShader || !fragShader)
		return 2;

	GLuint glslProgram = LinkShaders(vertShader, fragShader);
	if (!glslProgram)
		return 2;
	glUseProgram(glslProgram);

	GLint attrib_vCoord = BindAttribute("vCoord", glslProgram);
	GLint attrib_texCoord = BindAttribute("texCoord", glslProgram);
	if (attrib_vCoord == -1 || attrib_texCoord == -1)
		return 2;

	GLint uniformMVP = BindUniform("MVP", glslProgram);
	if (uniformMVP == -1)
		return 2;

	while (!glfwWindowShouldClose(window.win))
	{
		glClearColor(1, 1, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 anim = glm::rotate(glm::mat4(1), (float)glfwGetTime()*45, glm::vec3(1, 1, 1));
		glm::mat4 modelMat = glm::translate(glm::mat4(1), glm::vec3(0, 0, -4));
		glm::mat4 viewMat = glm::lookAt(glm::vec3(0, 0, -10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		glm::mat4 projectionMat = glm::perspective(60.f, 1.0f*window.width/window.height, 0.1f, 10.0f);
		glm::mat4 MVP = projectionMat * viewMat * modelMat * anim;

		glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, glm::value_ptr(MVP));

		mapMesh.Draw(attrib_vCoord, attrib_texCoord);

		glfwSwapBuffers(window.win);
		glfwPollEvents();
	}

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);
	glDeleteProgram(glslProgram);
}

