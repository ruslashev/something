#include "graphics.hpp"
#include "main.hpp"
#include "window.hpp"
#include "camera.hpp"

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

	Camera cam(&window);
	cam.position = glm::vec3(0, 0, -10);
	cam.pitch = 0;
	cam.yaw = M_PI;

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

	double simulationTime = 0;
	double realTime = 0;
	while (!glfwWindowShouldClose(window.win))
	{
		realTime = glfwGetTime();

		glm::mat4 anim;

		while (simulationTime < realTime) {
			// Update(time, dt)
			simulationTime += 0.0016;
			anim = glm::mat4(1);
			//glm::rotate(glm::mat4(1), (float)simulationTime*45, glm::vec3(0, 1, 0));
			cam.Update(&window, 0.0016);
		}

		glm::mat4 modelMat = glm::translate(glm::mat4(1), glm::vec3(0, 0, -4));
		glm::mat4 viewMat = cam.LookAtMat();
		const glm::mat4 projectionMat = glm::perspective(60.f,
				1.0f*window.width/window.height, 0.1f, 50.0f);
		glm::mat4 MVP = projectionMat * viewMat * modelMat * anim;

		glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, glm::value_ptr(MVP));

		glClearColor(1, 1, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mapMesh.Draw(attrib_vCoord, attrib_texCoord);

		// That was too easy not to do ----------
		GLfloat gridSize = 10.0f;
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

		glfwSwapBuffers(window.win);
		glfwPollEvents();
	}

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);
	glDeleteProgram(glslProgram);
}

