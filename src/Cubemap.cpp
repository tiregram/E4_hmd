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

Cubemap::Cubemap(glm::mat4 m,
                 const char * texture_file):Object(m)
{
  this->createOpengl(texture_file);
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

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

  glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &(*viewMatrix)[0][0]);
  glUniformMatrix4fv(ProjectionMatrixID, 1, GL_FALSE, &(*projectionMatrix)[0][0]);


  glBindTexture(GL_TEXTURE_CUBE_MAP, Texture);
  glDrawArrays(GL_TRIANGLES, 0, 36);

  glDepthMask(GL_TRUE);
}

void Cubemap::createOpengl(const char * texture_file)
{
  // create ShaderProgram
	programID = LoadShadersFromPathFile( "shader/vertCUBEMAP.glsl", "shader/fragCUBEMAP.glsl" );

  ProjectionMatrixID = glGetUniformLocation(programID, "projection");
	ViewMatrixID  = glGetUniformLocation(programID, "view");

  float skyboxVertices[] = {
    -300.0f,  300.0f, -300.0f,
    -300.0f, -300.0f, -300.0f,
    300.0f, -300.0f, -300.0f,
    300.0f, -300.0f, -300.0f,
    300.0f,  300.0f, -300.0f,
    -300.0f,  300.0f, -300.0f,

    -300.0f, -300.0f,  300.0f,
    -300.0f, -300.0f, -300.0f,
    -300.0f,  300.0f, -300.0f,
    -300.0f,  300.0f, -300.0f,
    -300.0f,  300.0f,  300.0f,
    -300.0f, -300.0f,  300.0f,

    300.0f, -300.0f, -300.0f,
    300.0f, -300.0f,  300.0f,
    300.0f,  300.0f,  300.0f,
    300.0f,  300.0f,  300.0f,
    300.0f,  300.0f, -300.0f,
    300.0f, -300.0f, -300.0f,

    -300.0f, -300.0f,  300.0f,
    -300.0f,  300.0f,  300.0f,
    300.0f,  300.0f,  300.0f,
    300.0f,  300.0f,  300.0f,
    300.0f, -300.0f,  300.0f,
    -300.0f, -300.0f,  300.0f,

    -300.0f,  300.0f, -300.0f,
    300.0f,  300.0f, -300.0f,
    300.0f,  300.0f,  300.0f,
    300.0f,  300.0f,  300.0f,
    -300.0f,  300.0f,  300.0f,
    -300.0f,  300.0f, -300.0f,

    -300.0f, -300.0f, -300.0f,
    -300.0f, -300.0f,  300.0f,
    300.0f, -300.0f, -300.0f,
    300.0f, -300.0f, -300.0f,
    -300.0f, -300.0f,  300.0f,
    300.0f, -300.0f,  300.0f
  };

  glGenBuffers(1, &vertexBufferID);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID );
	glBufferData(GL_ARRAY_BUFFER, 36 * sizeof(glm::vec3), &skyboxVertices[0], GL_STATIC_DRAW);

  this->setupCubeMap();
}


void Cubemap::setupCubeMap() {


  std::vector<std::string> map_fil=
    {"obj/mp_totality/totality_ft.tga",
     "obj/mp_totality/totality_bk.tga",
     "obj/mp_totality/totality_up.tga",
     "obj/mp_totality/totality_dn.tga",

     "obj/mp_totality/totality_rt.tga",
     "obj/mp_totality/totality_lf.tga"
    };

  int width, height, channels;
  unsigned char *ht_map;

  std::vector<unsigned char*> map_charged;
  for(auto a : map_fil){
    ht_map == NULL;
    ht_map= SOIL_load_image
      (
       a.c_str(),
       &width, &height, &channels,
       SOIL_LOAD_RGB
       );

    /* check for an error during the load process */
    if( NULL == ht_map)
      {
        printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
      }else
      {
        std::cout << a << " "
                  << width << " "
                  << height << " "
                  << channels << "\n";
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
