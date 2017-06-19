#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Object.hpp"

#include <vector>
#include <string>
#include <stb_image.h>

class Cubemap : public Object
{
public:
  //! Default constructor
  Cubemap(glm::mat4 m,
          const char * texture_file,
           const char * vert_shader_file,
           const char * frag_shader_file);

  //! Copy constructor
  Cubemap(const Cubemap &other);

  //! Destructor
  virtual ~Cubemap() noexcept{};

  void createOpengl(const char * vert_shader_file,const char * frag_shader_file, const char * texture_file);

  //! Copy assignment operator
  Cubemap& operator=(const Cubemap &other);

  //! Move assignment operator
  Cubemap& operator=(Cubemap &&other) noexcept{};

  virtual void update(double delta_time);
  virtual void draw();
  virtual void setVPmatrix(glm::mat4* v, glm::mat4* p);

  //custom SKYBOX


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
  GLuint ProjectionMatrixID;
	GLuint Texture;
  GLuint TextureID;

  //skybox specific
  GLuint cubeMapTexture;
  std::vector<std::string> faces;
  GLuint skyboxVAO;

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


protected:
private:

};
