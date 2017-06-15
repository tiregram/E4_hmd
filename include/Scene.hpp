#pragma once
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Object3D.hpp"

#include "Side.hpp"
#include "OpenHmdWrapper.hpp"
#include "GLFWContext.hpp"


class Scene
{
public:
  //! Default constructor

Scene(OpenHmdWrapper& ophmd,GLFWContext& glfw_context);

  //! Destructor
  virtual ~Scene() noexcept;

  void  draw();
  bool  update() ;

  void addObject(Object* obj);

  std::vector<Object*> objects;

protected:
  OpenHmdWrapper& hmd;
  Side left;
  Side right;

  glm::mat4 projectionMatrix ;
  GLFWContext &glfw_context;

  glm::mat4 viewMatrix;

};
