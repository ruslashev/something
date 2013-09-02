#include "main.hpp"
#include "window.hpp"
#include "graphics.hpp"

using namespace std;

int main()
{
	Window window(800, 600, "Something");

	glEnable(GL_DEPTH_TEST);

	const char *vertShaderSrc = \
		"#version 120"                                    "\n" \
		"uniform mat4 MVP;"                               "\n" \
		"attribute vec3 pos3D;"                           "\n" \
		"void main() {"                                   "\n" \
		"    gl_Position = MVP*vec4(pos3D, 1.0);""\n" \
		"}"                                               "\n";
	const char *fragShaderSrc = \
		"#version 120"                                    "\n" \
		"void main() {"                                   "\n" \
		"    gl_FragColor[0] = 0.0;"                      "\n" \
		"    gl_FragColor[1] = 0.3;"                      "\n" \
		"    gl_FragColor[2] = 1.0;"                      "\n" \
		"}"                                               "\n";

	float cubeVertices[] = {
		// front
		-1.0, -1.0,  1.0,
		 1.0, -1.0,  1.0,
		 1.0,  1.0,  1.0,
		-1.0,  1.0,  1.0,
		// back
		-1.0, -1.0, -1.0,
		 1.0, -1.0, -1.0,
		 1.0,  1.0, -1.0,
		-1.0,  1.0, -1.0,
	};
	GLuint VBOcube;
	glGenBuffers(1, &VBOcube);
	glBindBuffer(GL_ARRAY_BUFFER, VBOcube);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

	GLushort cubeElements[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// top
		3, 2, 6,
		6, 7, 3,
		// back
		7, 6, 5,
		5, 4, 7,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// left
		4, 0, 3,
		3, 7, 4,
		// right
		1, 5, 6,
		6, 2, 1,
	};
	GLuint IBOcube;
	glGenBuffers(1, &IBOcube);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOcube);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeElements), cubeElements, GL_STATIC_DRAW);

	GLuint vertShader = LoadShader(GL_VERTEX_SHADER, vertShaderSrc);
	GLuint fragShader = LoadShader(GL_FRAGMENT_SHADER, fragShaderSrc);
	if (!vertShader || !fragShader)
		return 2;

	GLuint glslProgram = LinkShaders(vertShader, fragShader);
	if (!glslProgram)
		return 2;

	GLint attrib_pos3D = BindAttribute("pos3D", glslProgram);
	if (attrib_pos3D == -1)
		return 2;

	GLint uniformMVP = BindUniform("MVP", glslProgram);
	if (uniformMVP == -1)
		return 2;

	while (!glfwWindowShouldClose(window.win))
	{
		glm::mat4 anim = glm::rotate(glm::mat4(1), (float)glfwGetTime()*45, glm::vec3(0, 1, 0));
		// shift model -> world
		glm::mat4 modelMat = glm::translate(glm::mat4(1), glm::vec3(0.0, 0.0, -4.0));
		// world -> view with lookAt(from, to, upVector)
		glm::mat4 viewMat = glm::lookAt(glm::vec3(0.0, 2.0, 0.0), glm::vec3(0.0, 0.0, -4.0), glm::vec3(0.0, 1.0, 0.0));
		// view -> projection (screen)
		glm::mat4 projectionMat = glm::perspective(60.f, 1.0f*window.width/window.height, 0.1f, 10.0f);
		glm::mat4 MVP = projectionMat * viewMat * modelMat * anim;

		glClearColor(1.0, 1.0, 1.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, glm::value_ptr(MVP));

		glUseProgram(glslProgram);

		glEnableVertexAttribArray(attrib_pos3D);
		glVertexAttribPointer(attrib_pos3D, 3, GL_FLOAT, GL_FALSE, 0, 0);

		int size;
		glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
		glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

		glDisableVertexAttribArray(attrib_pos3D);

		glfwSwapBuffers(window.win);
		glfwPollEvents();
	}

	glDeleteProgram(glslProgram);
	glDeleteBuffers(1, &VBOcube);
	glDeleteBuffers(1, &IBOcube);
}

