#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <functional>

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

  //! Destructor
  virtual ~TextureStreamSurface() noexcept;

  void createObject();

  void createOpengl();


  //! Copy assignment operator
  TextureStreamSurface& operator=(const TextureStreamSurface &other);

  //! Move assignment operator
  TextureStreamSurface& operator=(TextureStreamSurface &&other) noexcept{};

  //! update the pixel grid on a given memory zone
  //  comment to use lambda function
  //  void updatePixels(GLubyte* dst, int size);


  virtual void update(double t);
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

  std::function<void(GLubyte*, int, int)> updatePixels;
  //void (*updatePixels)(GLubyte*, int, int);

  /////////////////////////////////////////////////////////////////////////////
  //                                 3D PART                                 //
  /////////////////////////////////////////////////////////////////////////////
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;


  glm::mat4* projectionMatrix ;
  glm::mat4* viewMatrix;



  int win_id;
  virtual int get_id();

protected:
private:

};
