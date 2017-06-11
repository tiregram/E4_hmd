#include "Object3D.hpp"
#include "shader.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "objloader.hpp"
#include "vboindexer.hpp"
#include "texture.hpp"

Object3D::Object3D(const char * obj_file,
                   const char * texture_file,
                   const char * vert_shader_file,
                   const char * frag_shader_fil)
{
  std::cout <<"Object create A"  << "\n";
  this->createObject(obj_file);
  this->createOpengl(vert_shader_file, frag_shader_fil, texture_file);
}




Object3D::Object3D(const Object3D& other){
  std::cout <<"Object create b"  << "\n";
}

Object3D& Object3D::operator =(const Object3D& other) {
  std::cout <<"Object create c"  << "\n";

  return *this;
}

void Object3D::update(double delta_time) {

}

void Object3D::draw() {
  glUseProgram(programID);

  glm::vec3 lightPos = glm::vec3(4,4,4);
  glm::mat4 MVP = projectionMatrix * viewMatrix * modelMatrix;

  this->modelMatrix = glm::mat4(1.0);
  this->modelMatrix = glm::translate(this->modelMatrix, glm::vec3(2.0f, 0.0f, 0.0f));

  glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);
  glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &viewMatrix[0][0]);

  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
  glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &modelMatrix[0][0]);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, Texture);

  glUniform1i(TextureID, 0);

  /////////////////////////////////////////////////////////////////////////////
  //                             separation line                             //
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

  std::cout <<"ind"<<indices.size()  << "\n";
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, (void*)0);

}


void Object3D::createObject(const char* obj_file) {
	bool res = loadOBJ(obj_file, vertices, uvs, normals);

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
	GLuint programID = LoadShaders( vert_shader_file, frag_shader_file );

	GLuint MatrixID      = glGetUniformLocation(programID, "MVP");
	GLuint ViewMatrixID  = glGetUniformLocation(programID, "ViewMatrix");
	GLuint ModelMatrixID = glGetUniformLocation(programID, "ModelMatrix");

  GLuint Texture       = loadDDS(texture_file);
	GLuint TextureID     = glGetUniformLocation(programID, "TextureSampler");
	GLuint LightID       = glGetUniformLocation(programID, "LightPosition");

  /////////////////////////////////////////////////////////////////////////////
  //                                  buffer                                 //
  /////////////////////////////////////////////////////////////////////////////

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
