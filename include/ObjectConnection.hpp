#pragma once

#include "ObjectContainer.hpp"
#include "X/XDisplay.hpp"
#include "X/XWindow.hpp"
#include "TextureStreamSurface.hpp"

class ObjectConnection : public ObjectContainer
{
public:
  //! Default constructor
  ObjectConnection(glm::mat4 m);

  //! Destructor
  virtual ~ObjectConnection() noexcept;


  void update(double t)
  {
    ObjectContainer::update(t);
  }

  void draw()
  {
    ObjectContainer::draw();
  }
protected:

private:

  std::shared_ptr<G::Display> connection;
  std::vector<TextureStreamSurface> SurfaceToDraw;

};
