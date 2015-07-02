#include "graphics.hh"
#include "main.hh"
#include "window.hh"
#include "interaction.hh"
#include "framebufferer.hh"
#include "voxelworld.hh"

using namespace std;

int main()
{
	Window window(800, 600, "Something");
	glEnable(GL_DEPTH_TEST);
	glLineWidth(2);

	VoxelWorld vw;
	vw.fromFile("level.vxl");
	Mesh mapMesh = vw.voxelMesh;
	mapMesh.Upload();

	Mesh bat;
	bat.FromOBJ("bat.obj");
	bat.Upload();

	Player ply(&window);
	ply.pos = glm::vec3(-10, 1, 0);
	ply.pitch = ply.yaw = 0;

#define GLSL(src) "#version 120\n" #src
	const char *vertShaderSrc = GLSL(
		uniform mat4 MVP;
		attribute vec4 vCoord;
		attribute vec2 texCoord;
		varying vec2 ftexCoord;

		void main() {
			ftexCoord = texCoord;
			gl_Position = MVP*vCoord;
		}
	);
	const char *fragShaderSrc = GLSL(
		uniform sampler2D tex0;
		varying vec2 ftexCoord;
		void main() {
			vec2 inverseCoord = vec2(ftexCoord.x, 1.0-ftexCoord.y);
			gl_FragColor = texture2D(tex0, inverseCoord);
		}
	);
#undef GLSL

	GLuint vertShader = CreateShader(GL_VERTEX_SHADER, vertShaderSrc);
	GLuint fragShader = CreateShader(GL_FRAGMENT_SHADER, fragShaderSrc);

	GLuint glslProgram = LinkShadersToProgram(vertShader, fragShader);

	GLint attrib_vCoord = BindAttribute("vCoord", glslProgram);
	GLint attrib_texCoord = BindAttribute("texCoord", glslProgram);

	GLint uniformMVP = BindUniform("MVP", glslProgram);
	mapMesh.textUnif = BindUniform("tex0", glslProgram);

	Framebufferer fbm;

	double realTime, simulationTime = 0;
	const glm::mat4 projectionMat = glm::perspective(
			60.f, (float)window.width/window.height, 0.1f, 50.0f);
	while (!glfwWindowShouldClose(window.win)) {
		realTime = glfwGetTime();

		while (simulationTime < realTime) {
			simulationTime += 0.0016;
			// Update(time, dt)
			{
				const glm::vec3 oldPos = ply.pos;
				ply.Update(&window, 0.0016);
				AABB plyAABB = AABB { (int)ply.pos.x, (int)ply.pos.y, (int)ply.pos.z, 0.5, 1.5, 0.5 };
				for (AABB &box : vw.voxels)
					if (aabbsCollide(box, plyAABB)) {
						ply.pos = oldPos;
						break;
					}
			}
		}

		const glm::mat4 viewMat = ply.LookAtMat();
		const glm::mat4 MVP = projectionMat * viewMat;

		glUseProgram(glslProgram);
		glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, glm::value_ptr(MVP));

		// Draw -------------------------------------------------------
		{
			glBindFramebuffer(GL_FRAMEBUFFER, fbm.FBO);
			// Draw to Framebuffer

			glClearColor(1, 1, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			mapMesh.Draw(attrib_vCoord, attrib_texCoord);
			// bat.Draw(attrib_vCoord, attrib_texCoord);

			// That was too easy not to do ----------
			GLfloat gridSize = 11.0f;
			glBegin(GL_LINES);
			for (GLint i = -gridSize; i <= gridSize; i++)
			{
				glVertex3f(i, 0.f,  gridSize);
				glVertex3f(i, 0.f, -gridSize);
				glVertex3f(-gridSize, 0.f, i);
				glVertex3f( gridSize, 0.f, i);
			}
			glEnd();
			// --------------------------------------

			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			glClearColor(0, 0, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glUseProgram(fbm.ppProg);
			glBindTexture(GL_TEXTURE_2D, fbm.textToRenderTo);
			glUniform1i(fbm.uniformFBOtext, GL_TEXTURE0);
			glEnableVertexAttribArray(fbm.attrib_vCoordPP);

			glBindBuffer(GL_ARRAY_BUFFER, fbm.VBO_for_FBO);
			glVertexAttribPointer(fbm.attrib_vCoordPP, 2, GL_FLOAT, GL_FALSE, 0, 0);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			glDisableVertexAttribArray(fbm.attrib_vCoordPP);
		}

		glfwSwapBuffers(window.win);
		glfwPollEvents();
		// double frameTime = glfwGetTime() - realTime;
		// printf("%.7f\n", frameTime);
	}

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);
	glDeleteProgram(glslProgram);
}

void success()
{
	const char *green = "\x1b[32m";
	const char *reset = "\x1b[0m";
	printf("%s" "✓" "%s\n", green, reset);
}
void fail()
{
	const char *red   = "\x1b[31m";
	const char *reset = "\x1b[0m";
	printf("%s" "✗" "%s\n", red, reset);
}
void info(const char *msg)
{
	const char *blue  = "\x1b[36m";
	const char *reset = "\x1b[0m";
	printf("%s" "%s" "%s", blue, msg, reset);
}
void warn(const char *msg)
{
	const char *yellow  = "\x1b[33m";
	const char *reset = "\x1b[0m";
	printf("%s" "%s" "%s\n", yellow, msg, reset);
}

