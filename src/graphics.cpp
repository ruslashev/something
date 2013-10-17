#include "graphics.hpp"
#include "image.hpp"

#define INFO_COLOR  "\x1b[36m"
#define CLEAR_COLOR "\x1b[0m"

#define SUCCESS_STR "\x1b[32m" "✓" CLEAR_COLOR
#define FAIL_STR    "\x1b[31m" "✗" CLEAR_COLOR

void Mesh::FromOBJ(const char *filename)
{
	// TODO
	if (!loadOBJ(filename, vertices, elements))
		exit(2);
}

void Mesh::FromVXL(const char *filename)
{
	printf("Loading VXL model %s%-12s%s\t\t\t", INFO_COLOR, filename, CLEAR_COLOR);

	std::ifstream ifs(filename, std::ifstream::in);
	if (!ifs) {
		printf(FAIL_STR " failbit: %d badbit: %d\n", ifs.fail(), ifs.bad());
		exit(2);
	}

	std::string line;
	while (getline(ifs, line)) {
		if (line.substr(0,2) == "v ") {
			std::istringstream s(line.substr(2));
			int x, y, z;
			s >> x; s >> y; s >> z;
			vertices.push_back(glm::vec4(x  , y+1, z  , 1));
			vertices.push_back(glm::vec4(x  , y  , z  , 1));
			vertices.push_back(glm::vec4(x+1, y  , z  , 1));
			vertices.push_back(glm::vec4(x  , y+1, z  , 1));
			vertices.push_back(glm::vec4(x+1, y+1, z  , 1));
			vertices.push_back(glm::vec4(x+1, y  , z  , 1));
			vertices.push_back(glm::vec4(x  , y+1, z+1, 1));
			vertices.push_back(glm::vec4(x  , y  , z+1, 1));
			vertices.push_back(glm::vec4(x+1, y  , z+1, 1));
			vertices.push_back(glm::vec4(x  , y+1, z+1, 1));
			vertices.push_back(glm::vec4(x+1, y+1, z+1, 1));
			vertices.push_back(glm::vec4(x+1, y  , z+1, 1));

			vertices.push_back(glm::vec4(x  , y+1, z  , 1));
			vertices.push_back(glm::vec4(x  , y  , z  , 1));
			vertices.push_back(glm::vec4(x  , y  , z+1, 1));
			vertices.push_back(glm::vec4(x  , y+1, z  , 1));
			vertices.push_back(glm::vec4(x  , y+1, z+1, 1));
			vertices.push_back(glm::vec4(x  , y  , z+1, 1));
			vertices.push_back(glm::vec4(x+1, y+1, z  , 1));
			vertices.push_back(glm::vec4(x+1, y  , z  , 1));
			vertices.push_back(glm::vec4(x+1, y  , z+1, 1));
			vertices.push_back(glm::vec4(x+1, y+1, z  , 1));
			vertices.push_back(glm::vec4(x+1, y+1, z+1, 1));
			vertices.push_back(glm::vec4(x+1, y  , z+1, 1));

			vertices.push_back(glm::vec4(x  , y  , z+1, 1));
			vertices.push_back(glm::vec4(x  , y  , z  , 1));
			vertices.push_back(glm::vec4(x+1, y  , z  , 1));
			vertices.push_back(glm::vec4(x  , y  , z+1, 1));
			vertices.push_back(glm::vec4(x+1, y  , z+1, 1));
			vertices.push_back(glm::vec4(x+1, y  , z  , 1));
			vertices.push_back(glm::vec4(x  , y+1, z+1, 1));
			vertices.push_back(glm::vec4(x  , y+1, z  , 1));
			vertices.push_back(glm::vec4(x+1, y+1, z  , 1));
			vertices.push_back(glm::vec4(x  , y+1, z+1, 1));
			vertices.push_back(glm::vec4(x+1, y+1, z+1, 1));
			vertices.push_back(glm::vec4(x+1, y+1, z  , 1));

			for (int i = 1; i <= 6; i++) {
				texCoords.push_back(glm::vec2(0, 1));
				texCoords.push_back(glm::vec2(0, 0));
				texCoords.push_back(glm::vec2(1, 0));
				texCoords.push_back(glm::vec2(0, 1));
				texCoords.push_back(glm::vec2(1, 1));
				texCoords.push_back(glm::vec2(1, 0));
			}
		} else { /* ignoring this line */ }
	}

	printf(SUCCESS_STR "\n");
}

void Mesh::Upload()
{
	if (vertices.size() > 0) {
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER,
				vertices.size()*sizeof(vertices[0]),
				vertices.data(), GL_STATIC_DRAW);
	} else
		printf("Warning: Uploading empty vertex data\n");

	if (elements.size() > 0) {
		glGenBuffers(1, &IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
				elements.size()*sizeof(elements[0]),
				elements.data(), GL_STATIC_DRAW);
	}

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 128, 128, 0, GL_RGB, GL_UNSIGNED_BYTE,
			gimp_image.pixel_data);
	if (texCoords.size() > 0) {
		glGenBuffers(1, &VBO_tex);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_tex);
		glBufferData(GL_ARRAY_BUFFER,
				texCoords.size()*sizeof(texCoords[0]),
				texCoords.data(), GL_STATIC_DRAW);
	} else
		printf("Warning: Uploading empty texCoords data\n");
}

void Mesh::Draw(GLint &attrib_vCoord, GLint &attrib_texCoord)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glUniform1i(textUnif, GL_TEXTURE0);

	glEnableVertexAttribArray(attrib_vCoord);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(attrib_vCoord, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(attrib_texCoord);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_tex);
	glVertexAttribPointer(attrib_texCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);
	// glDrawElements(GL_TRIANGLES, elements.size(), GL_UNSIGNED_SHORT, 0);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	glDisableVertexAttribArray(attrib_vCoord);
}

Mesh::~Mesh()
{
	glDeleteBuffers(1, &VBO_tex);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &IBO);
	glDeleteTextures(1, &textureID);
}

bool loadOBJ(const char* filename, std::vector<glm::vec4> &vertices, std::vector<GLushort> &elements)
{
	printf("Loading OBJ model %s%-12s%s\t\t\t", INFO_COLOR, filename, CLEAR_COLOR);

	std::ifstream ifs(filename, std::ifstream::in);
	if (!ifs) {
		printf(FAIL_STR " failbit: %d badbit: %d\n", ifs.fail(), ifs.bad());
		return false;
	}

	std::string line;
	while (getline(ifs, line)) {
		if (line.substr(0,2) == "v ") {
			std::istringstream s(line.substr(2));
			glm::vec4 v;
			s >> v.x; s >> v.y; s >> v.z; v.w = 1.0f;
			vertices.push_back(v);
		} else if (line.substr(0,2) == "f ") {
			std::istringstream s(line.substr(2));
			GLushort a,b,c;
			s >> a; s >> b; s >> c;
			a--; b--; c--;
			elements.push_back(a); elements.push_back(b); elements.push_back(c);
		} else { /* ignoring this line */ }
	}

	printf(SUCCESS_STR "\n");
	return true;
}

GLuint CreateShader(GLenum type, const char *src)
{
	printf("Compiling %s shader \t\t\t",
			type == GL_VERTEX_SHADER ? "vertex" : "fragment");

	GLint compileSuccess = GL_FALSE;
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &src, NULL);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileSuccess);
	if (!compileSuccess) {
		printf(FAIL_STR "\n");
		printLog(shader);
		return 0;
	}

	printf(SUCCESS_STR "\n");
	return shader;
}

GLuint LinkShaders(GLuint &vertShader, GLuint &fragShader)
{
	printf("Linking shaders\t\t\t\t\t");

	GLint linkSuccess = GL_FALSE;
	GLuint glslProgram = glCreateProgram();

	glAttachShader(glslProgram, vertShader);
	glAttachShader(glslProgram, fragShader);

	glLinkProgram(glslProgram);

	glGetProgramiv(glslProgram, GL_LINK_STATUS, &linkSuccess);
	if (!linkSuccess) {
		printf(FAIL_STR "\n");
		printLog(glslProgram);
		return 0;
	}

	printf(SUCCESS_STR "\n" "Validating GLSL program\t\t\t\t");

	GLint validateSuccess = GL_FALSE;
	glValidateProgram(glslProgram);
	glGetProgramiv(glslProgram, GL_VALIDATE_STATUS, &validateSuccess);
	if (!validateSuccess) {
		printf(FAIL_STR "\n");
		printLog(glslProgram);
		return 0;
	}

	printf(SUCCESS_STR "\n");
	return glslProgram;
}

GLint BindAttribute(const char *name, GLuint &glslProgram)
{
	printf("Binding attribute %s%-12s%s\t\t\t", INFO_COLOR, name, CLEAR_COLOR);
	GLint attribute = glGetAttribLocation(glslProgram, name);
	if (attribute == -1) {
		printf(FAIL_STR "\n");
		return -1;
	}
	printf(SUCCESS_STR "\n");
	return attribute;
}

GLint BindUniform(const char *name, GLuint &glslProgram)
{
	printf("Binding uniform %s%-12s%s\t\t\t", INFO_COLOR, name, CLEAR_COLOR);
	GLint uniform = glGetUniformLocation(glslProgram, name);
	if (uniform  == -1) {
		printf(FAIL_STR "\n");
		return -1;
	}
	printf(SUCCESS_STR "\n");
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

	puts(log);
	delete [] log;
}

