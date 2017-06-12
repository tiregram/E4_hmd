#pragma once

#include <GL/glew.h>

class Scene
{
public:
  //! Default constructor
  Scene();

  //! Copy constructor
  Scene(const Scene &other);

  //! Move constructor
  Scene(Scene &&other) noexcept;

  //! Destructor
  virtual ~Scene() noexcept;

  //! Copy assignment operator
  Scene& operator=(const Scene &other);

  //! Move assignment operator
  Scene& operator=(Scene &&other) noexcept;

  void create_fbo(int eye_width, int eye_height, GLuint* fbo, GLuint* color_tex, GLuint* depth_tex);

  void  draw() const;
  void  update() const;

protected:
  GLuint right_color_tex;
  GLuint right_depth_tex;
  GLuint right_fbo;

  GLuint left_color_tex;
  GLuint left_depth_tex;
  GLuint left_fbo;

private:
  GLuint shaderHMD;
};


