#include "graphics.hpp"

GLuint LoadShader(GLenum type, const char *src)
{
	printf("Compiling %s shader \t\t", \
			type == GL_VERTEX_SHADER ? "vertex" : "fragment");

	GLint compileSuccess = GL_FALSE;
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &src, NULL);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileSuccess);
	if (!compileSuccess) {
		GLint logLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

		char *log = new (std::nothrow) char[logLength];
		if (!log) {
			printf("failed\n"\
					"And even failed to allocate memory for compile log.\n");
			return 0;
		}
		glGetShaderInfoLog(shader, logLength, NULL, log);

		printf("failed:\n%s", log);

		delete [] log;
		return 0;
	}

	printf("success\n");
	return shader;
}

GLuint LinkShaders(GLuint &vertShader, GLuint &fragShader)
{
	printf("Linking shaders\t\t\t\t");

	GLint linkSuccess = GL_FALSE;
	GLuint glslProgram = glCreateProgram();

	glAttachShader(glslProgram, vertShader);
	glAttachShader(glslProgram, fragShader);

	glLinkProgram(glslProgram);

	glGetProgramiv(glslProgram, GL_LINK_STATUS, &linkSuccess);
	if (!linkSuccess) {
		GLint logLength = 0;
		glGetProgramiv(glslProgram, GL_INFO_LOG_LENGTH, &logLength);

		char *log = new (std::nothrow) char[logLength];
		if (!log) {
			printf("failed\n"\
					"And even failed to allocate memory for link info log.\n");
			return 0;
		}
		glGetProgramInfoLog(glslProgram, logLength, NULL, log);

		printf("failed:\n%s", log);

		delete [] log;
		return 0;
	}

	printf("success\n");
	return glslProgram;
}

GLint BindAttribute(const char *name, GLuint &glslProgram)
{
	printf("Binding attribute\t\t\t");
	GLint attribute = glGetAttribLocation(glslProgram, name);
	if (attribute == -1) {
		printf("failed\n");
		return -1;
	}
	printf("success\n");
	return attribute;
}

GLint BindUniform(const char *name, GLuint &glslProgram)
{
	printf("Binding uniform\t\t\t");
	GLint uniform = glGetUniformLocation(glslProgram, name);
	if (uniform  == -1) {
		printf("failed\n");
		return -1;
	}
	printf("success\n");
	return uniform;
}
