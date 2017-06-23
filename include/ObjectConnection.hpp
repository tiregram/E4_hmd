#pragma once

#include "ObjectContainer.hpp"
#include "X/XDisplay.hpp"
#include "X/XWindow.hpp"
#include "TextureStreamSurface.hpp"
#include "Object3D.hpp"
class ObjectConnection : public ObjectContainer
{
public:
  // construction and destruction /////////////////////////////////////////////
  ObjectConnection(glm::mat4 m);
  virtual ~ObjectConnection() noexcept;

  // virtualfunction for Object  //////////////////////////////////////////////
  virtual void setVPmatrix(glm::mat4* v, glm::mat4* p);
  virtual void update(double t);
  virtual void draw();

  // function for manage window ///////////////////////////////////////////////
  void addWindowTexture(std::shared_ptr<G::Window>& a);
  bool haveChange();

private:
  Object3D table;
  Object3D grnd;

  std::shared_ptr<G::Display> connection;
  std::vector<TextureStreamSurface> SurfaceToDraw;
};
