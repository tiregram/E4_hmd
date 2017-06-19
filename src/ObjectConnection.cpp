#include "ObjectConnection.hpp"
#include <glm/gtc/matrix_transform.hpp>

ObjectConnection::ObjectConnection(glm::mat4 m):ObjectContainer(m)
{
  std::string name = "server";
  std::string addr = ":98";

  this->connection = std::make_shared<G::XDisplay>(name,addr);
  glm::mat4 position(1.0f);


  for(auto a : this->connection->get_windows_list())
    {

      position = glm::translate(position,glm::vec3(3.0f, 0.0f, 0.0f));
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
