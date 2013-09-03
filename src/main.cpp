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
		"attribute vec4 vCoord;"                          "\n" \
		"void main() {"                                   "\n" \
		"    gl_Position = MVP*vCoord;"                   "\n" \
		"}"                                               "\n";
	const char *fragShaderSrc = \
		"#version 120"                                    "\n" \
		"void main() {"                                   "\n" \
		"    gl_FragColor[0] = 0.0;"                      "\n" \
		"    gl_FragColor[1] = 0.3;"                      "\n" \
		"    gl_FragColor[2] = 1.0;"                      "\n" \
		"}"                                               "\n";

	std::vector<glm::vec4> vertices;
	std::vector<GLushort> elements;
	loadOBJ("bat.obj", vertices, elements);

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(vertices[0]), vertices.data(), GL_STATIC_DRAW);

	GLuint IBO;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size()*sizeof(elements[0]), elements.data(), GL_STATIC_DRAW);

	GLuint vertShader = LoadShader(GL_VERTEX_SHADER, vertShaderSrc);
	GLuint fragShader = LoadShader(GL_FRAGMENT_SHADER, fragShaderSrc);
	if (!vertShader || !fragShader)
		return 2;

	GLuint glslProgram = LinkShaders(vertShader, fragShader);
	if (!glslProgram)
		return 2;

	GLint attrib_vCoord = BindAttribute("vCoord", glslProgram);
	if (attrib_vCoord == -1)
		return 2;

	GLint uniformMVP = BindUniform("MVP", glslProgram);
	if (uniformMVP == -1)
		return 2;

	while (!glfwWindowShouldClose(window.win))
	{
		glm::mat4 anim = glm::rotate(glm::mat4(1), (float)glfwGetTime()*45, glm::vec3(0, 0, 1));
		glm::mat4 modelMat = glm::translate(glm::mat4(1), glm::vec3(0.0, 0.0, -4.0));
		glm::mat4 viewMat = glm::lookAt(glm::vec3(0.0, 0.0, -10.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
		glm::mat4 projectionMat = glm::perspective(60.f, 1.0f*window.width/window.height, 0.1f, 10.0f);
		glm::mat4 MVP = projectionMat * viewMat * modelMat * anim;

		glClearColor(1.0, 1.0, 1.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, glm::value_ptr(MVP));

		glUseProgram(glslProgram);

		glEnableVertexAttribArray(attrib_vCoord);
		glVertexAttribPointer(attrib_vCoord, 4, GL_FLOAT, GL_FALSE, 0, 0);

		int size;
		glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
		glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

		glDisableVertexAttribArray(attrib_vCoord);

		glfwSwapBuffers(window.win);
		glfwPollEvents();
	}

	glDeleteProgram(glslProgram);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &IBO);
}

