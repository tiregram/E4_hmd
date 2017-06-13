#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

GLuint LoadShadersFromPathFile(const char * vertex_file_path, const char * fragment_file_path);

GLuint LoadShadersFromCode(const char * vertex_code, const char * fragment_code);

GLuint loadBMP_custom(const char * imagepath);
