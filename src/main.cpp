#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "shader.hpp"
#include "Object3D.hpp"
#include "controls.hpp"

GLFWwindow* window;


int createGLFWContext()
{
  // Initialise GLFW
  if( !glfwInit() )
    {
      std::cerr<< "Failed to initialize GLFW"<<"\n";
      return -1;
    }

  glfwWindowHint(GLFW_SAMPLES, 4); 
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow( 1920, 1080, "Tutorial 01", NULL, NULL);

  if( window == NULL ){
    std::cerr<< "Failed to open GLFW window. "
             << "If you have an Intel GPU, they are not 3.3 compatible."
             <<" Try the 2.1 version of the tutorials."<<"\n";
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  // Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  // Hide the mouse and enable unlimited mouvement
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // Set the mouse at the center of the screen
  glfwPollEvents();
  glfwSetCursorPos(window, 1920/2, 1080/2);

}


int createGLEW()
{
  if (glewInit() != GLEW_OK) {
    std::cerr<<"Failed to initialize GLEW\n";
    return -1;
  }

}

int draw(std::vector<Object3D*>& a)
{

  do{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int i = 0;

    computeMatricesFromInputs();
    a[0]->projectionMatrix = getProjectionMatrix();
    a[0]->viewMatrix = getViewMatrix();

    for(auto& one_obj : a ){
        // std::cout << "draw " <<i++<< "\n";
        one_obj->draw();
      }

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

  }while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
         glfwWindowShouldClose(window) == 0 );
}


int main(int argc, char *argv[])
{
  createGLFWContext();
  createGLEW();

  glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); 
	glEnable(GL_CULL_FACE);

  std::vector<Object3D*> objs;

  objs.push_back(new Object3D("obj/suzanne.obj",
                              "obj/uvmap.DDS",
                              "shader/StandardShading.vertexshader",
                              "shader/StandardShading.fragmentshader"));

  draw(objs);

  for(auto& a : objs )
    {
      delete(a);
    }
  return 0;
}
