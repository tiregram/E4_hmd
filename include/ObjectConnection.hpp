#pragma once

#include "ObjectContainer.hpp"
#include "X/XDisplay.hpp"
#include "X/XWindow.hpp"


class ObjectConnection : ObjectContainer
{
public:
  //! Default constructor
  ObjectConnection();

  //! Copy constructor
  ObjectConnection(const ObjectConnection &other);

  //! Destructor
  virtual ~ObjectConnection() noexcept;

  //! Copy assignment operator
  ObjectConnection& operator=(const ObjectConnection &other);

  void update(double t)
  {
    ObjectContainer::update(t);
  }

protected:

private:

  std::shared_ptr<G::Display> di;

};
