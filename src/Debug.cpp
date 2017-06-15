#include "Debug.hpp"



#include <glm/glm.hpp>


int draw_debug(GLFWContext& glfw_context,std::vector<Object*>& a)
{

  glBindFramebuffer(GL_FRAMEBUFFER,0);
  do{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.1, 0.0, 0.0, 1.0);

    int i = 0;

    glfw_context.computeMatricesFromInputs();

    for(auto& one_obj : a ){
      // std::cout << "draw " <<i++<< "\n";
      one_obj->draw();
    }

  glfw_context.swapBuffers();

  }while(glfw_context.getKey());

}


int update_debug(std::vector<Object*>& a)
{
  for(auto& one_obj : a)
    {
      double t = 0.1;
      one_obj->update(t);
    }
}
