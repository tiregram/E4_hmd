#pragma once
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Object3D.hpp"
#include <openhmd/openhmd.h>
#include "GLFWContext.hpp"

class Scene
{
public:
  //! Default constructor
  Scene(GLFWContext& glfw_context);
/*
  //! Copy constructor
  Scene(const Scene &other);

  //! Move constructor
  Scene(Scene &&other) noexcept;
*/
  //! Destructor
  virtual ~Scene() noexcept;
/*
  //! Copy assignment operator
  Scene& operator=(const Scene &other);

  //! Move assignment operator
  Scene& operator=(Scene &&other) noexcept;
*/
  void create_fbo(int eye_width, int eye_height, GLuint* fbo, GLuint* color_tex, GLuint* depth_tex);

  void  draw();
  bool  update() ;


  std::vector<Object3D*> objects;
protected:
  GLuint right_color_tex;
  GLuint right_depth_tex;
  GLuint right_fbo;
  GLuint buf_vertex_panel_r;
  GLuint buf_UV_panel_r;
  GLuint buf_elem_panel_l;

  GLuint left_color_tex;
  GLuint left_depth_tex;
  GLuint left_fbo;
  GLuint buf_vertex_panel_l;
  GLuint buf_UV_panel_l;
  GLuint buf_elem_panel_r;

private:

  GLuint shaderHMD;
  GLFWContext &glfw_context;

	int hmd_w;
  int hmd_h;

  float ipd;
  ohmd_context* ctx;
  ohmd_device_settings* settings;
  ohmd_device* hmd;

  int eye_w;
  int eye_h;

  float warp_scale;
  float warp_adj;

  float viewport_scale[2];
	float distortion_coeffs[4];
	float aberr_scale[3];
	float sep;
	float left_lens_center[2];
	float right_lens_center[2];

};
