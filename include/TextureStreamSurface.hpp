#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>


#include <vector>
#include <iostream>

#include "Object.hpp"

class TextureStreamSurface : public Object
{
public:
  //! Default constructor
  TextureStreamSurface(glm::mat4 m, GLuint width=100,GLuint height =100);

  //! Copy constructor
  TextureStreamSurface(const TextureStreamSurface &other);

  //! Move constructor
  TextureStreamSurface(TextureStreamSurface &&other) noexcept{};

  //! Destructor
  virtual ~TextureStreamSurface() noexcept{};

  void createObject();

  void createOpengl();


  //! Copy assignment operator
  TextureStreamSurface& operator=(const TextureStreamSurface &other);

  //! Move assignment operator
  TextureStreamSurface& operator=(TextureStreamSurface &&other) noexcept{};

  //! update the pixel grid on a given memory zone
  void updatePixels(GLubyte* dst, int size);


  virtual void update(double delta_time);
  virtual void draw();

  virtual void setVPmatrix(glm::mat4* v, glm::mat4* p);

  /////////////////////////////////////////////////////////////////////////////
  //                                 OGL PART                                //
  /////////////////////////////////////////////////////////////////////////////
  GLuint shaderID;
  GLuint vertexBufferID;
  GLuint uvBufferID;
  GLuint elementBufferID;

  GLuint MatrixID;
	GLuint ViewMatrixID ;
	GLuint ModelMatrixID ;
  GLuint ProjectionMatrixID;

	GLuint Texture;
  GLuint TextureID;

  GLubyte* imageData;

  GLuint pboID;
  GLuint widthPBO;
  GLuint heightPBO;

  //taille du buffer
  GLuint dataSize;


  /////////////////////////////////////////////////////////////////////////////
  //                                 3D PART                                 //
  /////////////////////////////////////////////////////////////////////////////
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;


  glm::mat4* projectionMatrix ;
  glm::mat4* viewMatrix;
  glm::mat4 modelMatrix;

protected:
private:

};
