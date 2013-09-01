#include "window.hpp"

Window::Window(int newWidth, int newHeight, const char *title)
{
	if (!glfwInit())
		fprintf(stderr, "Failed to initialize GLFW\n");

	width = newWidth;
	height = newHeight;

	glfwSetErrorCallback(CallbackError);

	win = glfwCreateWindow(newWidth, newHeight, title, NULL, NULL);
	if (!win)
		fprintf(stderr, "Failed to open window\n");

	glfwSetFramebufferSizeCallback(win, CallbackFBsizeChange);

	glfwMakeContextCurrent(win);

	GLenum err = glewInit();
	if (err != GLEW_OK)
		fprintf(stderr, "glewInit failed: %s\n", glewGetErrorString(err));
	fprintf(stdout, "Using GLEW %s\n", glewGetString(GLEW_VERSION));
}

void CallbackError(int errorCode, const char *description)
{
	fprintf(stderr, "GLFW error %X: \"%s\"\n", errorCode, description);
}
void CallbackFBsizeChange(GLFWwindow *window, int width, int height)
{
	printf("Changed viewport to %dx%d\n", width, height);
	glViewport(0, 0, width, height);
}

Window::~Window()
{
	glfwDestroyWindow(win);
	glfwTerminate();
}

