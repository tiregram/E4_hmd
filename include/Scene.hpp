#pragma once
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Object3D.hpp"
#include "Side.hpp"
#include "OpenHmdWrapper.hpp"

class Scene
{
public:
  //! Default constructor
  Scene(OpenHmdWrapper& ophmd);


  //! Destructor
  virtual ~Scene() noexcept;

  //! Copy assignment operator
  Scene& operator=(const Scene &other);

  //! Move assignment operator
  Scene& operator=(Scene &&other) noexcept;

  void  draw();
  bool  update() ;

  GLFWwindow* window;
  std::vector<Object3D*> objects;
protected:
  OpenHmdWrapper& hmd;
  Side left;
  Side right;
};
