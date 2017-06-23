#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Object.hpp"

#include <vector>
#include <string>

class Cubemap : public Object
{
public:
  //! Default constructor
  Cubemap(glm::mat4 m, std::vector<std::string>& texture_files);

  //! Copy constructor
  Cubemap(const Cubemap &other);

  //! Destructor
  virtual ~Cubemap() noexcept{};

  //! Copy assignment operator
  Cubemap& operator=(const Cubemap &other);

  //! Move assignment operator
  Cubemap& operator=(Cubemap &&other) noexcept{};

  void createOpengl(std::vector<std::string>& texture_files);
  void setupCubeMap(std::vector<std::string>& texture_files);
  virtual void update(double delta_time);
  virtual void draw();
  virtual void setVPmatrix(glm::mat4* v, glm::mat4* p);

  //custom SKYBOX

  /////////////////////////////////////////////////////////////////////////////
  //                                 OGL PART                                //
  /////////////////////////////////////////////////////////////////////////////
  GLuint programID;
  GLuint vertexBufferID;

	GLuint ViewMatrixID ;
  GLuint ProjectionMatrixID;
	GLuint Texture;
  GLuint TextureID;

  /////////////////////////////////////////////////////////////////////////////
  //                                 3D PART                                 //
  /////////////////////////////////////////////////////////////////////////////
  glm::mat4* projectionMatrix ;
  glm::mat4* viewMatrix;

protected:
private:

};
