#include <Side.hpp>

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "Scene.hpp"

Side::Side(SIDE side, OpenHmdWrapper& ophmd,Scene& sce):Object(glm::mat4(1.0)),s(side),hmd(ophmd),sce(sce) {



  unsigned short element[6] = {0,1,2,2,3,0};
  glGenBuffers(1, &buf_elem_panel);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buf_elem_panel);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned short), element, GL_STATIC_DRAW);

  std::vector<glm::vec3> vertices(4);

  if (side == LEFT)
    {
      std::cout <<"LEFT"  << "\n";
      vertices[0] = glm::vec3(0,1,0);
      vertices[1] = glm::vec3(-1,1,0);
      vertices[2] = glm::vec3(-1,-1,0);
      vertices[3] = glm::vec3(0,-1,0);
    }
  else if (side == RIGHT)
    {
      std::cout << "RIGHT" << "\n";
      vertices[0] = glm::vec3(1,1,0);
      vertices[1] = glm::vec3(0,1,0);
      vertices[2] = glm::vec3(0,-1,0);
      vertices[3] = glm::vec3(1,-1,0);
    }

  glGenBuffers(1, &buf_vertex_panel);
	glBindBuffer(GL_ARRAY_BUFFER, buf_vertex_panel);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

  std::vector<glm::vec2> uv(4);
  uv[0] = glm::vec2(1,1);
  uv[1] = glm::vec2(0,1);
  uv[2] = glm::vec2(0,0);
  uv[3] = glm::vec2(1,0);

  glGenBuffers(1, &buf_UV_panel);
	glBindBuffer(GL_ARRAY_BUFFER, buf_UV_panel);
	glBufferData(GL_ARRAY_BUFFER, uv.size() * sizeof(glm::vec2), &uv[0], GL_STATIC_DRAW);

	create_fbo();
}

void Side::update(double t) {

}

void Side::setVPmatrix(glm::mat4* v, glm::mat4* p) {
  this->projectionMatrix = p;
  this->viewMatrix = v;
}



void Side::create_fbo() {
  
  glGenFramebuffers(1, &fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);

  glGenTextures(1, &color_tex);
  glBindTexture(GL_TEXTURE_2D, color_tex);
  glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, this->hmd.get_eye_w(), this->hmd.get_eye_h(), 0,GL_RGB, GL_UNSIGNED_BYTE, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glGenRenderbuffers(1, &depth_tex);
  glBindRenderbuffer(GL_RENDERBUFFER, depth_tex);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, this->hmd.get_eye_w(), this->hmd.get_eye_h());
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_tex);

  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, color_tex, 0);

  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    std::cout <<"error FBO"  << "\n";

}

void Side::draw() {

  glUseProgram(this->hmd.get_shaderHMD());

  //Stephen Added Code
  this->hmd.bindShader(s == LEFT);

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, buf_vertex_panel);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, buf_UV_panel);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, color_tex);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,buf_elem_panel);

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)0);


  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);

  glBindTexture(GL_TEXTURE_2D, 0);
  glDisable(GL_TEXTURE_2D);
  glUseProgram(0);

}


void Side::drawSceneInEye() {

  glm::mat4 save = *this->viewMatrix;

  glm::mat4 mat_v;
  glm::mat4 mat_p;

  hmd.getProjMatrix(s,*this->projectionMatrix);
  hmd.getViewMatrix(s,mat_v);

  *this->viewMatrix = mat_v**this->viewMatrix;

  glEnable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);

  // Draw scene into framebuffer.
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glViewport(0,0,this->hmd.get_eye_w(),this->hmd.get_eye_h());
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // if(s)
  //   glClearColor(0.3, 0.0, 0.0, 1.0);
  // else
  //   glClearColor(0.3, 0.0, 0.0, 1.0);

  for(auto& one_obj : sce.objects){
    one_obj->draw();
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  *this->viewMatrix= save;

}


//! Destructor
Side::~Side() noexcept{}
