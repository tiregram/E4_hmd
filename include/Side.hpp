#pragma once
#include <GL/glew.h>
#include "OpenHmdWrapper.hpp"

class Scene;

enum SIDE{
  LEFT,
  RIGHT
};

class Side
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

  void drawEyeInGlobal();
  void drawSceneInEye();


protected:
private:
  GLuint fbo;
  GLuint color_tex;
  GLuint depth_tex;

  SIDE s;
  GLuint buf_vertex_panel;
  GLuint buf_UV_panel;
  GLuint buf_elem_panel;

  OpenHmdWrapper& hmd;
  Scene& sce;
};

