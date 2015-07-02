#ifndef WINDOW_HH
#define WINDOW_HH

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>

void CallbackError(int errorCode, const char *description);
void CallbackFBsizeChange(GLFWwindow*, int width, int height);

class Window
{
public:
	GLFWwindow *win;
	int width, height;

	Window(int nwidth, int nheight, const char *title);
	~Window();
};

#endif

