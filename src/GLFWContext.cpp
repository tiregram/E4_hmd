#include "GLFWContext.hpp"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

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

  glfwMakeContextCurrent(this->window);

  // Ensure we can capture the escape key being pressed below
	glfwSetInputMode(this->window, GLFW_STICKY_KEYS, GL_TRUE);

  // Hide the mouse and enable unlimited mouvement
  glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // Set the mouse at the center of the screen
  glfwPollEvents();
  glfwSetCursorPos(this->window, w/2, h/2);

}

void GLFWContext::computeMatricesFromInputs(){


	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	// Get mouse position
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	// Reset mouse position for next frame
	glfwSetCursorPos(this->window, 1920/2, 1080/2);

	// Compute new orientation
	// this->horizontalAngle += this->mouseSpeed * float(1920/2 - xpos );
	// this->verticalAngle   += this->mouseSpeed * float( 1080/2 - ypos );

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(this->verticalAngle) * sin(this->horizontalAngle),
		sin(this->verticalAngle),
		cos(this->verticalAngle) * cos(this->horizontalAngle)
	);

	// Right vector
	glm::vec3 right = glm::vec3(
		sin(this->horizontalAngle - 3.14f/2.0f),
		0,
		cos(this->horizontalAngle - 3.14f/2.0f)
	);

	// Up vector
	glm::vec3 up = glm::cross( right, direction );

	// Move forward
	if (glfwGetKey( this->window, GLFW_KEY_UP ) == GLFW_PRESS){
		this->position += direction * deltaTime * this->speed;
	}
	// Move backward
	if (glfwGetKey( this->window, GLFW_KEY_DOWN ) == GLFW_PRESS){
		this->position -= direction * deltaTime * this->speed;
	}
	// Strafe right
	if (glfwGetKey( this->window, GLFW_KEY_RIGHT ) == GLFW_PRESS){
		this->position += right * deltaTime * this->speed;
	}
	// Strafe left
	if (glfwGetKey( this->window, GLFW_KEY_LEFT ) == GLFW_PRESS){
		this->position -= right * deltaTime * this->speed;
	}

	float FoV = this->initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

	// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	ViewMatrix       = glm::lookAt(
								this->position,           // Camera is here
								this->position+direction, // and looks here : at the same position, plus "direction"
								up                  // Head is up (set to 0,-1,0 to look upside-down)
						   );

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}

GLFWwindow* GLFWContext::getWindow()
{
	return this->window;
}

glm::mat4 GLFWContext::getViewMatrix()
{
	return this->ViewMatrix;
}

glm::mat4 GLFWContext::getProjectionMatrix()
{
	return this->ProjectionMatrix;
}

void GLFWContext::swapBuffers()
{
	// Swap buffers
  glfwSwapBuffers(this->window);
  glfwPollEvents();
}

bool GLFWContext::getKey()
{
	return glfwGetKey(this->window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(this->window) == 0 ;
}
