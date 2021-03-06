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
#include <SOIL/SOIL.h>

const float SIZE_CUBE_MAP = 300.0f;

Cubemap::Cubemap(glm::mat4 m,std::vector<std::string>& texture_files):Object(m)
{
  this->createOpengl(texture_files);
}


void Cubemap::setVPmatrix(glm::mat4* v, glm::mat4* p) {
  this->viewMatrix= v;
  this->projectionMatrix=p;
}

Cubemap::Cubemap(const Cubemap& other):Object(other.modelMatrix){
  this->set_father(other.get_father());
}

Cubemap& Cubemap::operator =(const Cubemap& other) {
  return *this;
}

void Cubemap::update(double delta_time) {

}

void Cubemap::draw()
{
  glDepthMask(GL_FALSE);
  glUseProgram(programID);

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

  glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &(*viewMatrix)[0][0]);
  glUniformMatrix4fv(ProjectionMatrixID, 1, GL_FALSE, &(*projectionMatrix)[0][0]);


  glBindTexture(GL_TEXTURE_CUBE_MAP, Texture);
  glDrawArrays(GL_TRIANGLES, 0, 36);

  glDepthMask(GL_TRUE);
}

void Cubemap::createOpengl(std::vector<std::string>& texture_files)
{
	programID = LoadShadersFromPathFile( "shader/vertCUBEMAP.glsl", "shader/fragCUBEMAP.glsl" );

  ProjectionMatrixID = glGetUniformLocation(programID, "projection");
	ViewMatrixID  = glGetUniformLocation(programID, "view");

  // TODO put in .obj
  float skyboxVertices[] = {
    -SIZE_CUBE_MAP,  SIZE_CUBE_MAP, -SIZE_CUBE_MAP,
    -SIZE_CUBE_MAP, -SIZE_CUBE_MAP, -SIZE_CUBE_MAP,
    SIZE_CUBE_MAP, -SIZE_CUBE_MAP, -SIZE_CUBE_MAP,
    SIZE_CUBE_MAP, -SIZE_CUBE_MAP, -SIZE_CUBE_MAP,
    SIZE_CUBE_MAP,  SIZE_CUBE_MAP, -SIZE_CUBE_MAP,
    -SIZE_CUBE_MAP,  SIZE_CUBE_MAP, -SIZE_CUBE_MAP,

    -SIZE_CUBE_MAP, -SIZE_CUBE_MAP,  SIZE_CUBE_MAP,
    -SIZE_CUBE_MAP, -SIZE_CUBE_MAP, -SIZE_CUBE_MAP,
    -SIZE_CUBE_MAP,  SIZE_CUBE_MAP, -SIZE_CUBE_MAP,
    -SIZE_CUBE_MAP,  SIZE_CUBE_MAP, -SIZE_CUBE_MAP,
    -SIZE_CUBE_MAP,  SIZE_CUBE_MAP,  SIZE_CUBE_MAP,
    -SIZE_CUBE_MAP, -SIZE_CUBE_MAP,  SIZE_CUBE_MAP,

    SIZE_CUBE_MAP, -SIZE_CUBE_MAP, -SIZE_CUBE_MAP,
    SIZE_CUBE_MAP, -SIZE_CUBE_MAP,  SIZE_CUBE_MAP,
    SIZE_CUBE_MAP,  SIZE_CUBE_MAP,  SIZE_CUBE_MAP,
    SIZE_CUBE_MAP,  SIZE_CUBE_MAP,  SIZE_CUBE_MAP,
    SIZE_CUBE_MAP,  SIZE_CUBE_MAP, -SIZE_CUBE_MAP,
    SIZE_CUBE_MAP, -SIZE_CUBE_MAP, -SIZE_CUBE_MAP,

    -SIZE_CUBE_MAP, -SIZE_CUBE_MAP,  SIZE_CUBE_MAP,
    -SIZE_CUBE_MAP,  SIZE_CUBE_MAP,  SIZE_CUBE_MAP,
    SIZE_CUBE_MAP,  SIZE_CUBE_MAP,  SIZE_CUBE_MAP,
    SIZE_CUBE_MAP,  SIZE_CUBE_MAP,  SIZE_CUBE_MAP,
    SIZE_CUBE_MAP, -SIZE_CUBE_MAP,  SIZE_CUBE_MAP,
    -SIZE_CUBE_MAP, -SIZE_CUBE_MAP,  SIZE_CUBE_MAP,

    -SIZE_CUBE_MAP,  SIZE_CUBE_MAP, -SIZE_CUBE_MAP,
    SIZE_CUBE_MAP,  SIZE_CUBE_MAP, -SIZE_CUBE_MAP,
    SIZE_CUBE_MAP,  SIZE_CUBE_MAP,  SIZE_CUBE_MAP,
    SIZE_CUBE_MAP,  SIZE_CUBE_MAP,  SIZE_CUBE_MAP,
    -SIZE_CUBE_MAP,  SIZE_CUBE_MAP,  SIZE_CUBE_MAP,
    -SIZE_CUBE_MAP,  SIZE_CUBE_MAP, -SIZE_CUBE_MAP,

    -SIZE_CUBE_MAP, -SIZE_CUBE_MAP, -SIZE_CUBE_MAP,
    -SIZE_CUBE_MAP, -SIZE_CUBE_MAP,  SIZE_CUBE_MAP,
    SIZE_CUBE_MAP, -SIZE_CUBE_MAP, -SIZE_CUBE_MAP,
    SIZE_CUBE_MAP, -SIZE_CUBE_MAP, -SIZE_CUBE_MAP,
    -SIZE_CUBE_MAP, -SIZE_CUBE_MAP,  SIZE_CUBE_MAP,
    SIZE_CUBE_MAP, -SIZE_CUBE_MAP,  SIZE_CUBE_MAP
  };

  glGenBuffers(1, &vertexBufferID);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID );
	glBufferData(GL_ARRAY_BUFFER, 36 * sizeof(glm::vec3), &skyboxVertices[0], GL_STATIC_DRAW);

  this->setupCubeMap(texture_files);
}


void Cubemap::setupCubeMap(std::vector<std::string>& texture_files) {

  // TODO search why SOIL load doesn't work

  int width, height, channels;
  unsigned char *ht_map;

  std::vector<unsigned char*> map_charged;
  for(auto one_texture_face : texture_files){

    // add a check on file existence

    ht_map == NULL;
    ht_map= SOIL_load_image
      (
       one_texture_face.c_str(),
       &width, &height, &channels,
       SOIL_LOAD_RGB
       );

    /* check for an error during the load process */
    if( NULL == ht_map)
      {
        printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
      }
    map_charged.push_back(ht_map);
  }

  // // unsigned char * data_image = loadBMP_custon_2("obj/MorningSky.bmp");
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_CUBE_MAP);
	glGenTextures(1, &Texture);


	glBindTexture(GL_TEXTURE_CUBE_MAP, Texture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, 512, 512, 0,  GL_RGB, GL_UNSIGNED_BYTE, map_charged[0]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, 512, 512, 0,  GL_RGB, GL_UNSIGNED_BYTE, map_charged[1]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, 512, 512, 0,  GL_RGB, GL_UNSIGNED_BYTE, map_charged[2]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, 512, 512, 0,  GL_RGB, GL_UNSIGNED_BYTE, map_charged[3]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, 512, 512, 0,  GL_RGB, GL_UNSIGNED_BYTE, map_charged[4]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, 512, 512, 0,  GL_RGB, GL_UNSIGNED_BYTE, map_charged[5]);

  for (auto a : map_charged) {
    SOIL_free_image_data( a );
  }
}
