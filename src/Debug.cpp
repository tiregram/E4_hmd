#include "Debug.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include "controls.hpp"
extern GLFWwindow* window;

int draw_debug(std::vector<Object*>& a)
{

  glBindFramebuffer(GL_FRAMEBUFFER,0);
  do{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.1, 0.0, 0.0, 1.0);

    int i = 0;

    computeMatricesFromInputs();

    for(auto& one_obj : a ){
      // std::cout << "draw " <<i++<< "\n";
      one_obj->draw();
    }

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
  }while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
          glfwWindowShouldClose(window) == 0 );

}


int update_debug(std::vector<Object*>& a)
{
  for(auto& one_obj : a)
    {
      double t = 0.1;
      one_obj->update(t);
    }
}
