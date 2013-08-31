#include "main.hpp"

using namespace std;

int main()
{
	Window window(800, 600, "Something");

	while (!glfwWindowShouldClose(window.win))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glBegin(GL_TRIANGLES);
			glVertex3f(-0.5f, -0.5f, 0.0f);
			glVertex3f( 0.0f,  0.5f, 0.0f);
			glVertex3f( 0.5f, -0.5f, 0.0f);
		glEnd();

		glfwSwapBuffers(window.win);
		glfwPollEvents();
	}
}

