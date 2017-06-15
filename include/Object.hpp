#pragma once

#include <glm/glm.hpp>

class Object
{
public:
  virtual void setVPmatrix(glm::mat4* v, glm::mat4* p)=0;
  virtual void draw()= 0;
  virtual void update(double t)=0;
};

