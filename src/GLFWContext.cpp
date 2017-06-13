#include "GLFWContext.hpp"


GLFWContext::GLFWContext(const unsigned int w, const unsigned int h)
{

	// Initialise GLFW
  if( !glfwInit() )
    {
      	std::cerr<< "Failed to initialize GLFW"<<"\n";
		throw std::string("Error : "+ GLFW_NOT_INITIALIZED);
    }

  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  this->window = glfwCreateWindow( w, h, "one", NULL, NULL);

  if( window == NULL ){
    std::cerr<< "Failed to open GLFW window. "
             << "If you have an Intel GPU, they are not 3.3 compatible."
             <<" Try the 2.1 version of the tutorials."<<"\n";
    getchar();
    glfwTerminate();
    throw std::string("Error : "+ GLFW_NOT_INITIALIZED);
  }

  glfwMakeContextCurrent(window);

  // Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  // Hide the mouse and enable unlimited mouvement
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // Set the mouse at the center of the screen
  glfwPollEvents();
  glfwSetCursorPos(window, w/2, h/2);

}

GLFWwindow* GLFWContext::getWindow()
{
	if(this->a == NULL)
	{
		this->a = new GLFWContext();
	}
	return this->window;
}