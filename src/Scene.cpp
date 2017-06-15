#include <openhmd/openhmd.h>

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Scene.hpp"
#include "Shader.hpp"
#include "Controls.hpp"



void Scene::addObject(Object* obj) {
  obj->setVPmatrix(&this->viewMatrix,&this->projectionMatrix);
  this->objects.push_back(obj);
}


Scene::Scene(OpenHmdWrapper& ophmd):left(LEFT,ophmd,*this),right(RIGHT,ophmd,*this),hmd(ophmd) {

}


//! Destructor
Scene::~Scene() noexcept
{

}

void  Scene::draw()
{
  projectionMatrix = getProjectionMatrix();
  viewMatrix = getViewMatrix();

  computeMatricesFromInputs();

  this->left.drawSceneInEye();
  this->right.drawSceneInEye();


  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(0,0,this->hmd.get_hmd_w(),this->hmd.get_eye_h());
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(0.0, 0.2, 0.0, 1.0);

  this->right.drawEyeInGlobal();
  this->left.drawEyeInGlobal();

  // // Clean up state.
  // glBindTexture(GL_TEXTURE_2D, 0);
  // glDisable(GL_TEXTURE_2D);
  // glUseProgram(0);

  // // Swap buffers
  glfwSwapBuffers(window);
  glfwPollEvents();
}

bool
Scene::update()
{
  double t=0;

  for(auto& one_obj : objects){
    one_obj->update(t);
  }

  return  glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0 ;
}
