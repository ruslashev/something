#include "window.hpp"

Window::Window(int newWidth, int newHeight, const char *title)
{
	printf("Initializing GLFW\t\t\t");
	if (!glfwInit()) {
		printf("failed\n");
	}
	printf("success, v %d.%d\n", GLFW_VERSION_MAJOR, GLFW_VERSION_MINOR);

	width = newWidth;
	height = newHeight;

	glfwSetErrorCallback(CallbackError);

	printf("Creating a window\t\t\t");
	win = glfwCreateWindow(newWidth, newHeight, title, NULL, NULL);
	if (!win) {
		printf("failed\n");
	}
	printf("success\n");

	glfwSetFramebufferSizeCallback(win, CallbackFBsizeChange);

	glfwMakeContextCurrent(win);

	printf("Initializing GLEW\t\t\t");
	GLenum err = glewInit();
	if (err != GLEW_OK)
		printf("failed: %s\n", glewGetErrorString(err));
	printf("success, v %s\n", glewGetString(GLEW_VERSION));
}

void CallbackError(int errorCode, const char *description)
{
	printf("GLFW error %X: \"%s\"\n", errorCode, description);
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

