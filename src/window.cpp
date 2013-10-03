#include "window.hpp"

#define SUCCESS_STR "\x1b[32m" "✓" "\x1b[0m"
#define FAIL_STR    "\x1b[31m" "✗" "\x1b[0m"

Window::Window(int newWidth, int newHeight, const char *title)
{
	printf("Initializing GLFW\t\t\t\t");
	if (!glfwInit())
		printf(FAIL_STR);

	printf(SUCCESS_STR " v%d.%d.%d\n", GLFW_VERSION_MAJOR, GLFW_VERSION_MINOR,
			GLFW_VERSION_REVISION);

	width = newWidth;
	height = newHeight;

	glfwSetErrorCallback(CallbackError);

	printf("Creating a window\t\t\t\t");
	win = glfwCreateWindow(newWidth, newHeight, title, NULL, NULL);
	if (!win)
		printf(FAIL_STR);

	printf(SUCCESS_STR "\n");

	glfwSetFramebufferSizeCallback(win, CallbackFBsizeChange);

	glfwMakeContextCurrent(win);

	printf("Initializing GLEW\t\t\t\t");
	GLenum err = glewInit();
	if (err != GLEW_OK)
		printf(FAIL_STR ": %s\n", glewGetErrorString(err));
	printf(SUCCESS_STR " v%s\n", glewGetString(GLEW_VERSION));
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

