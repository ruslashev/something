#include "graphics.hpp"

void Mesh::FromOBJ(const char *filename)
{
	if (!loadOBJ(filename, vertices, elements))
		exit(2);
}

void Mesh::Upload()
{
	if (vertices.size() > 0) {
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, \
				vertices.size()*sizeof(vertices[0]), \
				vertices.data(), GL_STATIC_DRAW);
	} else
		printf("Warning: Uploading empty vertex data\n");

	if (elements.size() > 0) {
		glGenBuffers(1, &IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, \
				elements.size()*sizeof(elements[0]), \
				elements.data(), GL_STATIC_DRAW);
	} else
		printf("Warning: Uploading empty elements data\n");
}

void Mesh::Draw(GLint &attrib_vCoord)
{
	glEnableVertexAttribArray(attrib_vCoord);
	glVertexAttribPointer(attrib_vCoord, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawElements(GL_TRIANGLES, elements.size(), GL_UNSIGNED_SHORT, 0);
	glDisableVertexAttribArray(attrib_vCoord);
}

Mesh::~Mesh()
{
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &IBO);
}

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
		printf("failed:\n");
		printLog(shader);
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
		printf("failed:\n");
		printLog(glslProgram);
		return 0;
	}

	printf("success\n" "Validating GLSL program\t\t\t");

	GLint validateSuccess = GL_FALSE;
	glValidateProgram(glslProgram);
	glGetProgramiv(glslProgram, GL_VALIDATE_STATUS, &validateSuccess);
	if (!validateSuccess) {
		printf("failed:\n");
		printLog(glslProgram);
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

void printLog(GLuint &shaderOrProg)
{
	GLint logLength = 0;

	if (glIsShader(shaderOrProg))
		glGetShaderiv(shaderOrProg, GL_INFO_LOG_LENGTH, &logLength);
	else
		glGetProgramiv(shaderOrProg, GL_INFO_LOG_LENGTH, &logLength);

	char *log = new (std::nothrow) char[logLength];
	if (!log) {
		printf("Failed to allocate memory for error log.\n");
		return;
	}

	if (glIsShader(shaderOrProg))
		glGetShaderInfoLog(shaderOrProg, logLength, NULL, log);
	else
		glGetProgramInfoLog(shaderOrProg, logLength, NULL, log);

	printf("%s", log);
	delete [] log;
}
