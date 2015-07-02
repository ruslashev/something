#include "graphics.hh"
#include "image.hh"
#include "main.hh"

bool Mesh::FromOBJ(const char *filename)
{
	printf("Loading OBJ model ");
	info(filename);
	printf("... ");

	std::ifstream ifs(filename, std::ifstream::in);
	if (!ifs) {
		fail();
		printf("failed to open file. fail: %d bad: %d\n",
				ifs.fail(), ifs.bad());
		throw;
	}

	std::string line;
	while (getline(ifs, line)) {
		if (line.substr(0, 2) == "v ") {
			std::istringstream s(line.substr(2));
			glm::vec4 v;
			s >> v.x; s >> v.y; s >> v.z; v.w = 1.0f;
			vertices.push_back(v);
		} else if (line.substr(0, 2) == "f ") {
			std::istringstream s(line.substr(2));
			GLushort a, b, c;
			s >> a; s >> b; s >> c;
			a--; b--; c--;
			elements.push_back(a); elements.push_back(b); elements.push_back(c);
		}
	}

	success();
	return true;
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
		warn("Warning: Uploading empty vertex data");

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
		warn("Warning: Uploading empty texture coords");
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
	if (elements.size() > 0)
		glDrawElements(GL_TRIANGLES, elements.size(), GL_UNSIGNED_SHORT, 0);
	else
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	glDisableVertexAttribArray(attrib_vCoord);
	glDisableVertexAttribArray(attrib_texCoord);
}

Mesh::~Mesh()
{
	glDeleteBuffers(1, &VBO_tex);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &IBO);
	glDeleteTextures(1, &textureID);
}

GLuint CreateShader(GLenum type, const char *src)
{
	printf("Compiling ");
	info(type == GL_VERTEX_SHADER ? "vertex" : "fragment");
	printf(" shader... ");

	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &src, NULL);
	glCompileShader(shader);

	GLint compileSuccess = GL_FALSE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileSuccess);
	if (!compileSuccess) {
		fail();
		printLog(shader);
		throw;
	} else
		success();

	return shader;
}

GLuint LinkShadersToProgram(GLuint &vertShader, GLuint &fragShader)
{
	printf("Linking shaders... ");

	GLuint glslProgram = glCreateProgram();

	glAttachShader(glslProgram, vertShader);
	glAttachShader(glslProgram, fragShader);

	glLinkProgram(glslProgram);

	GLint linkSuccess = GL_FALSE;
	glGetProgramiv(glslProgram, GL_LINK_STATUS, &linkSuccess);
	if (!linkSuccess) {
		fail();
		printLog(glslProgram);
		throw;
	} else
		success();

	printf("Validating GLSL program... ");

	glValidateProgram(glslProgram);

	GLint validateSuccess = GL_FALSE;
	glGetProgramiv(glslProgram, GL_VALIDATE_STATUS, &validateSuccess);
	if (!validateSuccess) {
		fail();
		printLog(glslProgram);
		throw;
	} else
		success();

	return glslProgram;
}

GLint BindAttribute(const char *name, GLuint &glslProgram)
{
	printf("Binding attribute ");
	info(name);
	printf("... ");

	GLint attribute = glGetAttribLocation(glslProgram, name);
	if (attribute == -1) {
		fail();
		throw;
	} else
		success();

	return attribute;
}

GLint BindUniform(const char *name, GLuint &glslProgram)
{
	printf("Binding uniform ");
	info(name);
	printf("... ");

	GLint uniform = glGetUniformLocation(glslProgram, name);
	if (uniform == -1) {
		fail();
		throw;
	} else
		success();

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
		puts("Failed to allocate memory for error log.");
		return;
	}

	if (glIsShader(shaderOrProg))
		glGetShaderInfoLog(shaderOrProg, logLength, NULL, log);
	else
		glGetProgramInfoLog(shaderOrProg, logLength, NULL, log);

	puts(log);

	delete [] log;
}

