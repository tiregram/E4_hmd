#include "Cubemap.hpp"
#include "Shader.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include <string>

#include "Vboindexer.hpp"
#include "Texture.hpp"


Cubemap::Cubemap(glm::mat4 m,
                 const char * texture_file,
                 const char * vert_shader_file,
                 const char * frag_shader_file)
{



  this->createOpengl(vert_shader_file, frag_shader_file, texture_file);
}


void Cubemap::setVPmatrix(glm::mat4* v, glm::mat4* p) {
  this->viewMatrix= v;
  this->projectionMatrix=p;
}

Cubemap::Cubemap(const Cubemap& other):Object(other.modelMatrix){
  std::cout <<"Cubemap create b"  << "\n";
  this->set_father(other.get_father());
}

Cubemap& Cubemap::operator =(const Cubemap& other) {
  std::cout <<"Cubemap create c"  << "\n";

  return *this;
}

void Cubemap::update(double delta_time) {

}

void Cubemap::draw()
{
  glDepthMask(GL_FALSE);
  glUseProgram(programID);

  glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &(*viewMatrix)[0][0]);
  glUniformMatrix4fv(ProjectionMatrixID, 1, GL_FALSE, &(*projectionMatrix)[0][0]);


  glBindVertexArray(skyboxVAO);
  glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glDepthMask(GL_TRUE);

}

void Cubemap::createOpengl(const char* vert_shader_file, const char* frag_shader_file)
{
  // create ShaderProgram
	programID = LoadShadersFromPathFile( vert_shader_file, frag_shader_file );

  ProjectionMatrixID = glGetUniformLocation(programID, "projection");
	ViewMatrixID  = glGetUniformLocation(programID, "view");

  Texture = loadDDS(texture_file);

  glGenVertexArrays(1, &skyboxVAO);

}


void setupCubeMap(GLuint& texture) {
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_CUBE_MAP);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void setupCubeMapBig(GLuint& texture) {
	setupCubeMap(texture);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, xpos->w, xpos->h, 0,  GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, xpos->pixels);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, xneg->w, xneg->h, 0,  GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, xneg->pixels);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, ypos->w, ypos->h, 0,  GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, ypos->pixels);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, yneg->w, yneg->h, 0,  GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, yneg->pixels);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, zpos->w, zpos->h, 0,  GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, zpos->pixels);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, zneg->w, zneg->h, 0,  GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, zneg->pixels);
}
