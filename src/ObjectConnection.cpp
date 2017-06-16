#include "ObjectConnection.hpp"


ObjectConnection::ObjectConnection(glm::mat4 m):ObjectContainer(m)
{

}

ObjectConnection::ObjectConnection(const ObjectConnection& other)):ObjectContainer()
{
  
}

ObjectConnection& ObjectConnection::operator =(const ObjectConnection& other) {
  ObjectConnection b;

  return b;
}
