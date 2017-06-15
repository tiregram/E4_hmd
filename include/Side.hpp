#pragma once
#include <GL/glew.h>
#include "Object.hpp"

class Scene;
class OpenHmdWrapper;

enum SIDE{
  LEFT,
  RIGHT
};

class Side : public  Object
{
public:
  //! Default constructor
  Side(SIDE side, OpenHmdWrapper& ophmd,Scene& sce);

  //! Copy constructor
  Side(const Side &other) = delete;

  //! Move constructor
  Side(Side &&other) noexcept = delete;

  //! Destructor
  virtual ~Side() noexcept;

  //! Copy assignment operator
  Side& operator=(const Side &other);

  //! Move assignment operator
  Side& operator=(Side &&other) noexcept;

  void create_fbo();

  void draw();
  void drawSceneInEye();

  void update(double t);

  virtual void setVPmatrix(glm::mat4* v, glm::mat4* p);

protected:
private:
  GLuint fbo;
  GLuint color_tex;
  GLuint depth_tex;

  SIDE s;
  GLuint buf_vertex_panel;
  GLuint buf_UV_panel;
  GLuint buf_elem_panel;

  glm::mat4* projectionMatrix ;
  glm::mat4* viewMatrix;

  OpenHmdWrapper& hmd;
  Scene& sce;
};

