#include <openhmd/openhmd.h>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Scene.hpp"
#include "shader.hpp"
#include "controls.hpp"

//#define OVERSAMPLE_SCALE 2.0

Scene::Scene() {

	ctx = ohmd_ctx_create();
	int num_devices = ohmd_ctx_probe(ctx);
	if(num_devices < 0){
		std::cout <<"failed to probe devices: %s\n"<< ohmd_ctx_get_error(ctx)  << "\n";
  }

	settings = ohmd_device_settings_create(ctx);
  int auto_update = 1;

	ohmd_device_settings_seti(settings, OHMD_IDS_AUTOMATIC_UPDATE, &auto_update);
	hmd = ohmd_list_open_device_s(ctx, 0, settings);

	ohmd_device_geti(hmd, OHMD_SCREEN_HORIZONTAL_RESOLUTION, &hmd_w);
	ohmd_device_geti(hmd, OHMD_SCREEN_VERTICAL_RESOLUTION, &hmd_h);

	ohmd_device_getf(hmd, OHMD_EYE_IPD, &ipd);
	//viewport is half the screen
	ohmd_device_getf(hmd, OHMD_SCREEN_HORIZONTAL_SIZE, &(viewport_scale[0]));
	viewport_scale[0] /= 2.0f;
	ohmd_device_getf(hmd, OHMD_SCREEN_VERTICAL_SIZE, &(viewport_scale[1]));
	//distortion coefficients
	ohmd_device_getf(hmd, OHMD_UNIVERSAL_DISTORTION_K, &(distortion_coeffs[0]));
	ohmd_device_getf(hmd, OHMD_UNIVERSAL_ABERRATION_K, &(aberr_scale[0]));
	//calculate lens centers (assuming the eye separation is the distance betweenteh lense centers)
	ohmd_device_getf(hmd, OHMD_LENS_HORIZONTAL_SEPARATION, &sep);
	ohmd_device_getf(hmd, OHMD_LENS_VERTICAL_POSITION, &(left_lens_center[1]));
	ohmd_device_getf(hmd, OHMD_LENS_VERTICAL_POSITION, &(right_lens_center[1]));

  std::cout << "screen"<<this->hmd_h <<"x"<< this->hmd_w<< "\n";

	left_lens_center[0] = viewport_scale[0] - sep/2.0f;
	right_lens_center[0] = sep/2.0f;
	//asume calibration was for lens view to which ever edge of screen is further away from lens center
	warp_scale = (left_lens_center[0] > right_lens_center[0]) ? left_lens_center[0] : right_lens_center[0];
	warp_adj = 1.0f;

	ohmd_device_settings_destroy(settings);



	if(!hmd){
		std::cout <<"failed to open device:" << ohmd_ctx_get_error(ctx) << "\n";
  }

	// const char* vertex_HMD;
	// ohmd_gets(OHMD_GLSL_DISTORTION_VERT_SRC, &vertex_HMD);
	// const char* fragment_HMD;
	// ohmd_gets(OHMD_GLSL_DISTORTION_FRAG_SRC, &fragment_HMD);

	shaderHMD = LoadShadersFromPathFile("shader/vert.glsl","shader/frag.glsl" );
	// glUseProgram(shaderHMD);
	// glUniform1i(glGetUniformLocation(shaderHMD, "warpTexture"), 0);
	// glUniform2fv(glGetUniformLocation(shaderHMD, "ViewportScale"), 1, viewport_scale);
	// glUniform3fv(glGetUniformLocation(shaderHMD, "aberr"), 1, aberr_scale);
	// glUseProgram(0);

	eye_w = hmd_w/2;//OVERSAMPLE_SCALE;
	eye_h = hmd_h;//*OVERSAMPLE_SCALE;


  // vertex left //////////////////////////////////////////////////////////////
  unsigned short element_left[6] = {0,1,2,2,3,0};

  glGenBuffers(1, &buf_elem_panel_l);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buf_elem_panel_l);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned short), element_left, GL_STATIC_DRAW);

  std::vector<glm::vec3> vertices_left(4);
  vertices_left[0] = glm::vec3(0,1,0);
  vertices_left[1] = glm::vec3(-1,1,0);
  vertices_left[2] = glm::vec3(-1,-1,0);
  vertices_left[3] = glm::vec3(0,-1,0);

  glGenBuffers(1, &buf_vertex_panel_l);
	glBindBuffer(GL_ARRAY_BUFFER, buf_vertex_panel_l);
	glBufferData(GL_ARRAY_BUFFER, vertices_left.size() * sizeof(glm::vec3), &vertices_left[0], GL_STATIC_DRAW);

  std::vector<glm::vec2> uv_left(4);
  uv_left[0] = glm::vec2(1,1);
  uv_left[1] = glm::vec2(0,1);
  uv_left[2] = glm::vec2(0,0);
  uv_left[3] = glm::vec2(1,0);

  glGenBuffers(1, &buf_UV_panel_l);
	glBindBuffer(GL_ARRAY_BUFFER, buf_UV_panel_l);
	glBufferData(GL_ARRAY_BUFFER, uv_left.size() * sizeof(glm::vec2), &uv_left[0], GL_STATIC_DRAW);

  // vertex right ////////////////////////////////////////////////////////////////
  unsigned short element_right[6] = {0,1,2,2,3,0};

  glGenBuffers(1, &buf_elem_panel_r);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buf_elem_panel_r);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned short), element_right, GL_STATIC_DRAW);

  std::vector<glm::vec3> vertices_right(4);
  vertices_right[0] = glm::vec3(1,1,0);
  vertices_right[1] = glm::vec3(0,1,0);
  vertices_right[2] = glm::vec3(0,-1,0);
  vertices_right[3] = glm::vec3(1,-1,0);


  glGenBuffers(1, &buf_vertex_panel_r);
	glBindBuffer(GL_ARRAY_BUFFER, buf_vertex_panel_r);
	glBufferData(GL_ARRAY_BUFFER, vertices_right.size() * sizeof(glm::vec3), &vertices_right[0], GL_STATIC_DRAW);

  std::vector<glm::vec2>  uv_right(4);
  uv_right[0] = glm::vec2(1,1);
  uv_right[1] = glm::vec2(0,1);
  uv_right[2] = glm::vec2(0,0);
  uv_right[3] = glm::vec2(1,0);

  glGenBuffers(1, &buf_UV_panel_r);
	glBindBuffer(GL_ARRAY_BUFFER, buf_UV_panel_r);
	glBufferData(GL_ARRAY_BUFFER, uv_right.size() * sizeof(glm::vec2), &uv_right[0], GL_STATIC_DRAW);

  std::cout << eye_w<<"x"<< eye_h << "\n";
	create_fbo(eye_w, eye_h, &left_fbo, &left_color_tex, &left_depth_tex);
	create_fbo(eye_w, eye_h, &right_fbo, &right_color_tex, &right_depth_tex);
}

Scene::Scene(const Scene& other) {

}

Scene& Scene::operator =(const Scene& other) {
  Scene b;

  return b;
}

Scene::Scene(Scene &&other) noexcept
{

}


Scene& Scene::operator=(Scene &&other) noexcept
{

}

//! Destructor
Scene::~Scene() noexcept
{

}

void  Scene::draw()
{
  computeMatricesFromInputs();

  // left eyes //////////////////////////////////////////////////////////////
  // Common scene state
  glEnable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);
  float matrix[16];

  // set hmd rotation, for left eye.
  ohmd_device_getf(hmd, OHMD_LEFT_EYE_GL_PROJECTION_MATRIX, matrix);
  ohmd_device_getf(hmd, OHMD_LEFT_EYE_GL_MODELVIEW_MATRIX, matrix);

  // Draw scene into framebuffer.
  glBindFramebuffer(GL_FRAMEBUFFER, left_fbo);
  glViewport(0,0,this->eye_w,this->eye_h);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(0.3, 0.0, 0.0, 1.0);

  for(auto& one_obj : objects){
    one_obj->draw();
  }

  // right eyes //////////////////////////////////////////////////////////////
  // set hmd rotation, for right eye.

  ohmd_device_getf(hmd, OHMD_RIGHT_EYE_GL_PROJECTION_MATRIX, matrix);
  ohmd_device_getf(hmd, OHMD_RIGHT_EYE_GL_MODELVIEW_MATRIX, matrix);


  // Draw scene into framebuffer.
  glBindFramebuffer(GL_FRAMEBUFFER, right_fbo);
  glViewport(0,0,this->eye_w,this->eye_h);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(0.0, 0.0, 0.3, 1.0);

  for(auto& one_obj : objects){
    one_obj->draw();
  }

  // both ///////////////////////////////////////////////////////////////////
  // Clean up common draw state
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(0,0,1000,1000);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(0.0, 0.0, 0.2, 1.0);

  glDisable(GL_BLEND);
  glDisable(GL_DEPTH_TEST);

  glUseProgram(shaderHMD);

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, buf_vertex_panel_l);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, buf_UV_panel_l);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, left_color_tex);
  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,buf_elem_panel_l);

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);



  glUseProgram(shaderHMD);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, buf_vertex_panel_r);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, buf_UV_panel_r);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, right_color_tex);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,buf_elem_panel_r);

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)0);


  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);

  // // Setup ortho state.
  // glUseProgram(shaderHMD);
  // // glUniform1f(glGetUniformLocation(shaderHMD, "WarpScale"), warp_scale*warp_adj);
  // // glUniform4fv(glGetUniformLocation(shaderHMD, "HmdWarpParam"), 1, distortion_coeffs);
  // // glViewport(0, 0, hmd_w, hmd_h);
  // // glEnable(GL_TEXTURE_2D);
  // // glColor4d(1, 1, 1, 1);

  // // // Setup simple render state
  // // glMatrixMode(GL_PROJECTION);
  // // glLoadIdentity();

  // // glMatrixMode(GL_MODELVIEW);
  // // glLoadIdentity();

  // // // Draw left eye
  // //  glUniform2fv(glGetUniformLocation(shaderHMD, "LensCenter"), 1, left_lens_center);

  // //  glBindTexture(GL_TEXTURE_2D, left_color_tex);
  // glBegin(GL_QUADS);
  // glTexCoord2d( 0,  0);
  // glVertex3d(  -1, -1, 0);
  // glTexCoord2d( 1,  0);
  // glVertex3d(   0, -1, 0);
  // glTexCoord2d( 1,  1);
  // glVertex3d(   0,  1, 0);
  // glTexCoord2d( 0,  1);
  // glVertex3d(  -1,  1, 0);
  // glEnd();

  // // // Draw right eye
  // // glUniform2fv(glGetUniformLocation(shaderHMD, "LensCenter"), 1, right_lens_center);
  // // glBindTexture(GL_TEXTURE_2D, right_color_tex);
  // // glBegin(GL_QUADS);
  // // glTexCoord2d( 0,  0);
  // // glVertex3d(   0, -1, 0);
  // // glTexCoord2d( 1,  0);
  // // glVertex3d(   1, -1, 0);
  // // glTexCoord2d( 1,  1);
  // // glVertex3d(   1,  1, 0);
  // // glTexCoord2d( 0,  1);
  // // glVertex3d(   0,  1, 0);
  // // glEnd();

  // // Clean up state.
  glBindTexture(GL_TEXTURE_2D, 0);
  glDisable(GL_TEXTURE_2D);
  glUseProgram(0);

  // Swap buffers
  glfwSwapBuffers(window);
  glfwPollEvents();

}

bool
Scene::update()
{
  double t=0;

  for(auto& one_obj : objects){
    one_obj->update(t);
  }

  return  glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0 ;
}


void Scene::create_fbo(int eye_width, int eye_height, GLuint* fbo, GLuint* color_tex, GLuint* depth_tex)
{
  glGenFramebuffers(1, fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, *fbo);

  glGenTextures(1, color_tex);
  glBindTexture(GL_TEXTURE_2D, *color_tex);
  glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, eye_width, eye_height, 0,GL_RGB, GL_UNSIGNED_BYTE, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glGenRenderbuffers(1, depth_tex);
  glBindRenderbuffer(GL_RENDERBUFFER, *depth_tex);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, eye_width, eye_height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, *depth_tex);

  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, *color_tex, 0);

  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    std::cout <<"eroor FBO"  << "\n";

	// glGenTextures(1, depth_tex);
	// glGenFramebuffers(1, fbo);

	// glBindTexture(GL_TEXTURE_2D, *color_tex);
	// glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, eye_width, eye_height, 0, GL_RGBA, GL_UNSIGNED_INT, NULL);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// glBindTexture(GL_TEXTURE_2D, *depth_tex);
	// glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, eye_width, eye_height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// glBindTexture(GL_TEXTURE_2D, 0);

	// glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, *fbo);
	// glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *color_tex, 0);
	// glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, *depth_tex, 0);

	// GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);

	// if(status != GL_FRAMEBUFFER_COMPLETE_EXT){
	// 	printf("failed to create fbo %x\n", status);
	// }
	// glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}
