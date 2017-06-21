#include "ObjectConnection.hpp"
#include <glm/gtc/matrix_transform.hpp>

ObjectConnection::ObjectConnection(glm::mat4 m):ObjectContainer(m),
                                                table( glm::translate(m,glm::vec3(0.0f, -0.3f, 0.0f)),
                                                       "obj/table.obj",
                                                       "obj/uvmap.DDS",
                                                       "shader/StandardShading.vertexshader",
                                                       "shader/StandardShading.fragmentshader"),
                                                grnd(glm::translate(m,glm::vec3(0.0f, -0.6f, 0.0f)),
                                                     "obj/grnd.obj",
                                                     "obj/uvmap.DDS",
                                                     "shader/StandardShading.vertexshader",
                                                     "shader/StandardShading.fragmentshader")
{
  std::string name = "server";
  std::string addr = ":99";

  this->connection = std::make_shared<G::XDisplay>(name,addr);
  glm::mat4 position(1.0f);
  position = glm::scale(position,glm::vec3(0.2f, 0.2f, 0.2f));
  //  position = glm::translate(position,glm::vec3(0.0f, 0.0f, -1.0f));
  glm::mat4 angle(1.0f);

  for(auto a : this->connection->get_windows_list())
    {

      angle = glm::rotate(angle, 20.0f, glm::vec3(0.0f,1.0f,0.0f));
      position = glm::translate(angle,glm::vec3(0.0f, 0.0f, -2.0f));

      auto& gm = a->get_geo_manager();
      std::cout << "size"<<gm.get_width()<<gm.get_height() << "\n";
      TextureStreamSurface* tss =new TextureStreamSurface(position, gm.get_height(),gm.get_width());
      tss->updatePixels=[a](GLubyte* dp, int b, int c){
        a->get_screen(dp);
        };
      this->add_object(tss);
    }
}

ObjectConnection::~ObjectConnection() {

}


void ObjectConnection::setVPmatrix(glm::mat4* v, glm::mat4* p)
{
  ObjectContainer::setVPmatrix(v,p);
  table.setVPmatrix(v,p);
  grnd.setVPmatrix(v,p);
}
