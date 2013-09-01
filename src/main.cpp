#include "main.hpp"
#include "window.hpp"
#include "graphics.hpp"

using namespace std;

int main()
{
	Window window(800, 600, "Something");

	const char *vertShaderSrc = \
		"#version 120"                                "\n" \
		"attribute vec2 pos2D;"                       "\n" \
		"void main() {"                               "\n" \
		"    gl_Position = vec4(pos2D, 0.0, 1.0);"    "\n" \
		"}"                                           "\n";
	const char *fragShaderSrc = \
		"#version 120"                                "\n" \
		"void main() {"                               "\n" \
		"    gl_FragColor[0] = 0.0;"                  "\n" \
		"    gl_FragColor[1] = 0.3;"                  "\n" \
		"    gl_FragColor[2] = 1.0;"                  "\n" \
		"}"                                           "\n";

	GLuint vertShader = LoadShader(GL_VERTEX_SHADER, vertShaderSrc);
	GLuint fragShader = LoadShader(GL_FRAGMENT_SHADER, fragShaderSrc);
	if (!vertShader || !fragShader)
		return 2;

	GLuint glslProgram = LinkShaders(vertShader, fragShader);
	if (!glslProgram)
		return 2;

	GLint attrib_pos2D = BindAttribute("pos2D", glslProgram);
	if (attrib_pos2D == -1)
		return 2;

	float triVertices[] = {
		 0.0,  0.5,
		-0.5, -0.5,
		 0.5, -0.5
	};

	while (!glfwWindowShouldClose(window.win))
	{
		glClearColor(1.0, 1.0, 1.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(glslProgram);
		glEnableVertexAttribArray(attrib_pos2D);

		// Describe our vertices array to OpenGL (it can't guess its format automatically)
		glVertexAttribPointer(
			attrib_pos2D,      // attribute
			2,                 // number of elements per vertex, here (x,y)
			GL_FLOAT,          // the type of each element
			GL_FALSE,          // take our values as-is
			0,                 // no extra data between each position
			triVertices        // pointer to the C array
		);

		// Push each element in buffer_vertices to the vertex shader
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glDisableVertexAttribArray(attrib_pos2D);

		glfwSwapBuffers(window.win);
		glfwPollEvents();
	}

	glDeleteProgram(glslProgram);
}

