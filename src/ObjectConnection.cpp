#include "ObjectConnection.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

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
      addWindowTexture(a);

    }
}

void ObjectConnection::addWindowTexture(std::shared_ptr<G::Window>& a)
{
  int si = this->objects_contained.size();
  glm::mat4 model(1.0f);


  int w = 600;
  int h = 600;

  int x = 600;


  auto& gm = a->get_geo_manager();
  gm.set_x(x*si);
  gm.set_y(0);
  gm.set_width(w);
  gm.set_height(h);
  gm.apply();



  model = glm::rotate(model, si * -3.141595f/3.0f, glm::vec3(0.0f,1.0f,0.0f));

  model = glm::translate(model,glm::vec3(0.0f, 0.0f, -2.0f));

  model = glm::rotate(model, -3.141592f/2.0f, glm::vec3(0.0f,0.0f,1.0f));

  TextureStreamSurface* tss =new TextureStreamSurface(model, 600,600);
  tss->win_id = a->get_id();

  tss->updatePixels=
    [this,a,tss](GLubyte* dp, int b, int c)
    {

      auto windl = this->connection->get_windows_list();
      auto result = std::find_if(windl.begin(),windl.end(),
                                 [a](std::shared_ptr<G::Window>& w){return a->get_id() == w->get_id();}
                                 );
      if(result != windl.end())
        {

          a->get_screen(dp);
        }
      else
        {
          std::cout << "c"  << "\n";
          this->remove_object(tss);
        }
    };

  this->add_object(tss);

}



ObjectConnection::~ObjectConnection() {

}


void ObjectConnection::setVPmatrix(glm::mat4* v, glm::mat4* p)
{
  ObjectContainer::setVPmatrix(v,p);
  table.setVPmatrix(v,p);
  grnd.setVPmatrix(v,p);
}
