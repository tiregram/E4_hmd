#pragma once

#include <GLFW/glfw3.h>

class GLFWContext
{
public:	

  GLFWContext(const unsigned int w, const unsigned int h);
  GLFWwindow* getWindow();

private: 

	GLFWwindow* window;

};