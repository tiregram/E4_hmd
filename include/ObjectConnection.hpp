#pragma once

#include "ObjectContainer.hpp"
#include "X/XDisplay.hpp"
#include "X/XWindow.hpp"
#include "TextureStreamSurface.hpp"
#include "Object3D.hpp"
class ObjectConnection : public ObjectContainer
{
public:
  //! Default constructor
  ObjectConnection(glm::mat4 m);

  //! Destructor
  virtual ~ObjectConnection() noexcept;

  virtual void setVPmatrix(glm::mat4* v, glm::mat4* p);

  void update(double t)
  {
    table.update(t);
    grnd.update(t);

    ObjectContainer::update(t);
  }

  void draw()
  {
    table.draw();
    grnd.draw();

    ObjectContainer::draw();
  }
protected:

private:


  Object3D table;

  Object3D grnd;

  std::shared_ptr<G::Display> connection;
  std::vector<TextureStreamSurface> SurfaceToDraw;

};
