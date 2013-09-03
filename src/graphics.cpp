#include "graphics.hpp"

bool loadOBJ(const char* filename, std::vector<glm::vec4> &vertices, std::vector<GLushort> &elements)
{
	printf("Loading OBJ model %12s\t\t", filename);

	std::ifstream ifs(filename, std::ifstream::in);
	if (!ifs) {
		printf("failed: failbit: %d badbit: %d\n", ifs.fail(), ifs.bad());
		return false;
	}

	std::string line;
	while (getline(ifs, line)) {
		if (line.substr(0,2) == "v ") {
			std::istringstream s(line.substr(2));
			glm::vec4 v;
			s >> v.x; s >> v.y; s >> v.z; v.w = 1.0f;
			vertices.push_back(v);
		}  else if (line.substr(0,2) == "f ") {
			std::istringstream s(line.substr(2));
			GLushort a,b,c;
			s >> a; s >> b; s >> c;
			a--; b--; c--;
			elements.push_back(a); elements.push_back(b); elements.push_back(c);
		}
		// TODO
		else if (line[0] == '#') { /* ignoring this line */ }
		else { /* ignoring this line */ }
	}

	printf("success\n");
	return true;
}

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

	printf("Validating GLSL program\t\t\t");

	GLint validateSuccess = GL_FALSE;
	glValidateProgram(glslProgram);
	glGetProgramiv(glslProgram, GL_VALIDATE_STATUS, &validateSuccess);
	if (!validateSuccess) {
		printf("failed\n");
		// TODO printLog(..)
		return 0;
	}

	printf("success\n");
	return glslProgram;
}

GLint BindAttribute(const char *name, GLuint &glslProgram)
{
	printf("Binding attribute %12s\t\t", name);
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
	printf("Binding uniform   %12s\t\t", name);
	GLint uniform = glGetUniformLocation(glslProgram, name);
	if (uniform  == -1) {
		printf("failed\n");
		return -1;
	}
	printf("success\n");
	return uniform;
}
