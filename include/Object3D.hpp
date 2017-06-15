#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Object.hpp"

#include <vector>

class Object3D : public Object
{
public:
  //! Default constructor
  Object3D(glm::mat4 m,
           const char * obj_file,
           const char * texture_file,
           const char * vert_shader_file,
           const char * frag_shader_file);

  //! Copy constructor
  Object3D(const Object3D &other);

  //! Move constructor
  Object3D(Object3D &&other) noexcept{};

  //! Destructor
  virtual ~Object3D() noexcept{};

  void createObject(const char * obj_file);

  void createOpengl(const char * vert_shader_file,const char * frag_shader_file,const char * texture_file);

  //! Copy assignment operator
  Object3D& operator=(const Object3D &other);

  //! Move assignment operator
  Object3D& operator=(Object3D &&other) noexcept{};

  virtual void setVPmatrix(glm::mat4* v, glm::mat4* p);

  virtual void update(double delta_time);
  virtual void draw();

  /////////////////////////////////////////////////////////////////////////////
  //                                 OGL PART                                //
  /////////////////////////////////////////////////////////////////////////////
  GLuint VertexArrayID;
  GLuint programID;
  GLuint vertexBufferID;
  GLuint uvBufferID;
  GLuint normalBufferID;
  GLuint elementBufferID;

	GLuint MatrixID;
  GLuint LightID;
	GLuint ViewMatrixID ;
	GLuint ModelMatrixID ;

	GLuint Texture;
  GLuint TextureID;

  /////////////////////////////////////////////////////////////////////////////
  //                                 3D PART                                 //
  /////////////////////////////////////////////////////////////////////////////
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;

  std::vector<unsigned short> indices;
	std::vector<glm::vec3> indexed_vertices;
	std::vector<glm::vec2> indexed_uvs;
	std::vector<glm::vec3> indexed_normals;

  glm::mat4* projectionMatrix ;
  glm::mat4* viewMatrix;
  glm::mat4 modelMatrix;



protected:
private:

};
