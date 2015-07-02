#include "main.hh"
#include <GL/glew.h>
#include <fstream>
#include "window.hh"

Window::Window(int nwidth, int nheight, const char *title)
{
	printf("Initializing GLFW... ");
	if (!glfwInit()) {
		fail();
		throw;
	} else
		success();

	printf("GLFW v%d.%d.%d\n",
			GLFW_VERSION_MAJOR, GLFW_VERSION_MINOR, GLFW_VERSION_REVISION);

	width = nwidth;
	height = nheight;

	glfwSetErrorCallback(CallbackError);

	printf("Creating window... ");
	win = glfwCreateWindow(width, height, title, NULL, NULL);
	if (!win) {
		fail();
		throw;
	} else
		success();

	glfwSetFramebufferSizeCallback(win, CallbackFBsizeChange);

	glfwMakeContextCurrent(win);

	printf("Initializing GLEW... ");
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		fail();
		printf("%s\n", glewGetErrorString(err));
	} else {
		success();
		printf("GLEW v%s\n", glewGetString(GLEW_VERSION));
	}

	printf("GL Vendor:    %s\n", glGetString(GL_VENDOR));
	printf("GL Renderer:  %s\n", glGetString(GL_RENDERER));
	printf("GL Version:   %s\n", glGetString(GL_VERSION));
	printf("GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
}

void CallbackError(int errorCode, const char *description)
{
	printf("GLFW error %X: \"%s\"\n", errorCode, description);
}
void CallbackFBsizeChange(GLFWwindow*, int width, int height)
{
	printf("Changed viewport to %dx%d\n", width, height);
	glViewport(0, 0, width, height);
}

Window::~Window()
{
	glfwDestroyWindow(win);
	glfwTerminate();
}

