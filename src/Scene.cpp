#include <openhmd/openhmd.h>

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <unistd.h>
#include "Scene.hpp"

#include "Shader.hpp"



//#define OVERSAMPLE_SCALE 2.0

Scene::Scene(OpenHmdWrapper& ophmd,GLFWContext& glfw_context):left(LEFT,ophmd,*this),right(RIGHT,ophmd,*this),hmd(ophmd),glfw_context(glfw_context) {
  this->left.setVPmatrix(&viewMatrix,&projectionMatrix);
  this->right.setVPmatrix(&viewMatrix,&projectionMatrix);

}



void Scene::addObject(Object* obj) {
  obj->setVPmatrix(&this->viewMatrix,&this->projectionMatrix);
  this->objects.push_back(obj);
}


//! Destructor
Scene::~Scene() noexcept
{

}

void  Scene::draw()
{

  glfw_context.computeMatricesFromInputs();


  viewMatrix =   glfw_context.getViewMatrix();

  this->left.drawSceneInEye();
  this->right.drawSceneInEye();

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(0,0,this->hmd.get_hmd_w(),this->hmd.get_eye_h());
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(0.0, 0.2, 0.0, 1.0);

  this->right.draw();
  this->left.draw();

  // // Clean up state.
  glBindTexture(GL_TEXTURE_2D, 0);
  glDisable(GL_TEXTURE_2D);
  glUseProgram(0);
  glfw_context.swapBuffers();

}

bool Scene::update()
{

  float time_for_limit = 1.0 / FPS_LIMIT;

  float new_time = glfwGetTime();
  float time_enlapsed = new_time - this->old_time;
  this->old_time = new_time;
  std::cout << "\r FPS : " << 1/time_enlapsed;

/*
  float delta = time_for_limit - time_enlapsed;
  delta *= 1000;
  std::cout << " \r" <<delta;
  if(delta > 0)
    usleep(100);
*/
  for(auto& one_obj : objects){
    one_obj->update(time_enlapsed);
  }

  //return  glfwGetKey(this->glfw_context.getWindow(), GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(this->glfw_context.getWindow()) == 0 ;
  return glfw_context.getKey();
}
