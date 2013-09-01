#include "main.hpp"

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

	printf("Compiling vertex shader\n");

	GLint compileStatus = GL_FALSE;
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertShaderSrc, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus == false) {
		fprintf(stderr, "Error in vertex shader\n");
		return 1;
	}

	printf("Compiling fragment shader\n");

	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &fragShaderSrc, NULL);
	glCompileShader(fragShader);
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus == false) {
		fprintf(stderr, "Error in fragment shader\n");
		return 1;
	}

	printf("Linking shaders\n");

	GLint linkStatus = GL_FALSE;
	GLuint glslProgram = glCreateProgram();
	glAttachShader(glslProgram, vertexShader);
	glAttachShader(glslProgram, fragShader);
	glLinkProgram(glslProgram);
	glGetProgramiv(glslProgram, GL_LINK_STATUS, &linkStatus);
	if (!linkStatus) {
		fprintf(stderr, "Failed to link shaders\n");
		return 1;
	}

	printf("Binding attribute\n");

	GLint attrib_pos2D;
	const char *attributeName = "pos2D";
	attrib_pos2D = glGetAttribLocation(glslProgram, attributeName);
	if (attrib_pos2D == -1) {
		fprintf(stderr, "Could not bind attribute %s\n", attributeName);
		return 1;
	}

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

