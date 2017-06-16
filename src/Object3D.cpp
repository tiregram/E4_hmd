#include "Object3D.hpp"
#include "Shader.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "Objloader.hpp"
#include "Vboindexer.hpp"
#include "Texture.hpp"

Object3D::Object3D(glm::mat4 m,
                   const char * obj_file,
                   const char * texture_file,
                   const char * vert_shader_file,
                   const char * frag_shader_fil):Object(m)
{
  std::cout <<"Object create A"  << "\n";
  this->createObject(obj_file);
  this->createOpengl(vert_shader_file, frag_shader_fil, texture_file);
}

void Object3D::setVPmatrix(glm::mat4* v, glm::mat4* p) {
  this->viewMatrix= v;
  this->projectionMatrix=p;
}


Object3D::Object3D(const Object3D& other):Object(other.modelMatrix){
  std::cout <<"Object create b"  << "\n";
  this->set_father(other.get_father());
}

Object3D& Object3D::operator =(const Object3D& other) {
  std::cout <<"Object create c"  << "\n";

  return *this;
}

void Object3D::update(double delta_time) {

}

void Object3D::draw() {

  glUseProgram(programID);

  glm::mat4 tmp_model_matrix = this->get_model_matrix();
  glm::vec3 lightPos = glm::vec3(4,4,4);
  glm::mat4 MVP = *this->projectionMatrix * *this->viewMatrix * tmp_model_matrix;


  glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);
  glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &(*viewMatrix)[0][0]);

  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

  glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &tmp_model_matrix[0][0]);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, Texture);
  glUniform1i(TextureID, 0);

  /////////////////////////////////////////////////////////////////////////////
  //                             load obj                             //
  /////////////////////////////////////////////////////////////////////////////
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, uvBufferID);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

  glEnableVertexAttribArray(2);
  glBindBuffer(GL_ARRAY_BUFFER, normalBufferID);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferID);

  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, (void*)0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);
}


void Object3D::createObject(const char* obj_file) {
	bool res = loadAssImp(obj_file,indices, vertices, uvs, normals);

  if(!res)
    {
      std::cout <<"error read .obj:"  <<obj_file<< "\n";
    }

  indexVBO(vertices, uvs, normals, indices, indexed_vertices, indexed_uvs, indexed_normals);

  return;
}

void Object3D::createOpengl(const char* vert_shader_file, const char* frag_shader_file, const char* texture_file ) {

  /////////////////////////////////////////////////////////////////////////////
  //                                  Shader                                 //
  /////////////////////////////////////////////////////////////////////////////
	programID = LoadShadersFromPathFile( vert_shader_file, frag_shader_file );

	MatrixID      = glGetUniformLocation(programID, "MVP");
	ViewMatrixID  = glGetUniformLocation(programID, "V");
	ModelMatrixID = glGetUniformLocation(programID, "M");

  Texture       = loadDDS(texture_file);
	TextureID     = glGetUniformLocation(programID, "myTextureSampler");
	LightID       = glGetUniformLocation(programID, "LightPosition_worldspace");

  /////////////////////////////////////////////////////////////////////////////
  //                                  buffer                                 //
  /////////////////////////////////////////////////////////////////////////////
  std::cout <<indexed_vertices.size()<<","
            <<indexed_uvs.size()<<","
            <<indexed_normals.size()<<","
            <<indices.size()<<","
            << "\n";
  // vertex ///////////////////////////////////////////////////////////////////
  glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

  // uv //////////////////////////////////////////////////////////////////////
	glGenBuffers(1, &uvBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, uvBufferID);
	glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

  // normal ///////////////////////////////////////////////////////////////////
	glGenBuffers(1, &normalBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferID);
	glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);

  // element vertex id ////////////////////////////////////////////////////////
	glGenBuffers(1, &elementBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0] , GL_STATIC_DRAW);

  return;
}
