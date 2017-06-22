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

    if(haveChange())
      {

        for (auto w : this->connection->get_windows_list()) {

          int i = 0;
          int size_texture=this->objects_contained.size();
          int have_a_texture = false;

          while(i < size_texture && have_a_texture == false)
            {

              if (this->objects_contained[i]->get_id() == w->get_id() ) {
                have_a_texture = true;
              }
              i++;
            }

          if(!have_a_texture)
            {
              std::cout << "o" << "\n";
              this->addWindowTexture(w);
            }
        }
      }

    ObjectContainer::draw();
    auto wl = this->connection->get_windows_list();
    int new_q = wl.size();

  }


  void addWindowTexture(std::shared_ptr<G::Window>& a);
  bool haveChange()
  {
    auto wl = this->connection->get_windows_list();

    if(this->objects_contained.size()!=wl.size())
        {
          return true;
        }

  }


protected:

private:

  Object3D table;
  int q=0;
  Object3D grnd;

  std::shared_ptr<G::Display> connection;
  std::vector<TextureStreamSurface> SurfaceToDraw;

};
