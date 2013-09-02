#include "main.hpp"
#include "window.hpp"
#include "graphics.hpp"

using namespace std;

int main()
{
	Window window(800, 600, "Something");

	const char *vertShaderSrc = \
		"#version 120"                                    "\n" \
		"uniform mat4 transformMat;"                      "\n" \
		"attribute vec3 pos3D;"                           "\n" \
		"void main() {"                                   "\n" \
		"    gl_Position = transformMat*vec4(pos3D, 1.0);""\n" \
		"}"                                               "\n";
	const char *fragShaderSrc = \
		"#version 120"                                    "\n" \
		"void main() {"                                   "\n" \
		"    gl_FragColor[0] = 0.0;"                      "\n" \
		"    gl_FragColor[1] = 0.3;"                      "\n" \
		"    gl_FragColor[2] = 1.0;"                      "\n" \
		"}"                                               "\n";

	float triVertices[] = {
		 0.0,  0.5, 0.0,
		-0.5, -0.5, 0.0,
		 0.5, -0.5, 0.0
	};
	GLuint VBOtriangle;
	glGenBuffers(1, &VBOtriangle);
	glBindBuffer(GL_ARRAY_BUFFER, VBOtriangle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triVertices), triVertices, GL_STATIC_DRAW);

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

	GLint uniformTransf = BindUniform("transformMat", glslProgram);

	while (!glfwWindowShouldClose(window.win))
	{
		float move = sinf(glfwGetTime() * (2*3.14) / 5); // -1<->+1 every 5 seconds
		float angle = glfwGetTime() * 45;
		glm::mat4 transfMatrix = glm::translate(
			glm::mat4(1),
			glm::vec3(move, 0.0, 0.0)
		) * glm::rotate(
			glm::mat4(1), angle, glm::vec3(0, 0, 1)
		);

		glUniformMatrix4fv(uniformTransf, 1, GL_FALSE, glm::value_ptr(transfMatrix));

		glClearColor(1.0, 1.0, 1.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(glslProgram);
		glEnableVertexAttribArray(attrib_pos3D);
		glVertexAttribPointer(attrib_pos3D, 3, GL_FLOAT, GL_FALSE, 0, 0);

		// Push each element in buffer_vertices to the vertex shader
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glDisableVertexAttribArray(attrib_pos3D);

		glfwSwapBuffers(window.win);
		glfwPollEvents();
	}

	glDeleteProgram(glslProgram);
	glDeleteBuffers(1, &VBOtriangle);
}

