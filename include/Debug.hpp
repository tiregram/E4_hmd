#pragma once

#include <GL/glew.h>
#include "GLFWContext.hpp"

#include <vector>
#include "Object.hpp"


int draw_debug(GLFWContext& glfw_context,std::vector<Object*>& a);
int update_debug(std::vector<Object*>& a);
