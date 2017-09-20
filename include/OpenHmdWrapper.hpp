#pragma once
#include <openhmd/openhmd.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Side.hpp"


class OpenHmdWrapper
{
public:
  const GLuint get_shaderHMD() const {
    return shaderHMD;
  }

  void set_shaderHMD(const GLuint shaderHMD) {
    this->shaderHMD = shaderHMD;
  }

  int get_hmd_h() const {
    return hmd_h;
  }

  void set_hmd_h(const int hmd_h) {
    this->hmd_h = hmd_h;
  }

  int get_hmd_w() const {
    return hmd_w;
  }

  void set_hmd_w(const int hmd_w) {
    this->hmd_w = hmd_w;
  }

  int get_eye_h() const {
    return eye_h;
  }

  void set_eye_h(const int eye_h) {
    this->eye_h = eye_h;
  }

  int get_eye_w() const {
    return eye_w;
  }

  void set_eye_w(const int eye_w) {
    this->eye_w = eye_w;
  }


  void getViewMatrix(SIDE s, glm::mat4& view);
  void getProjMatrix(SIDE s, glm::mat4& view);

  //! Default constructor
  OpenHmdWrapper();

  //! Copy constructor
  OpenHmdWrapper(const OpenHmdWrapper &other);

  //! Move constructor
  OpenHmdWrapper(OpenHmdWrapper &&other) noexcept;

  //! Destructor
  virtual ~OpenHmdWrapper() noexcept;

  //! Copy assignment operator
  OpenHmdWrapper& operator=(const OpenHmdWrapper &other);

  //! Move assignment operator
  OpenHmdWrapper& operator=(OpenHmdWrapper &&other) noexcept;

  void  createShader();
  void  bindShader(SIDE s);
  void  unbindShader();;

  void update();
  
protected:
private:
  ohmd_context* ctx;

  ohmd_device* hmd;

  float ipd;

	int hmd_w;
  int hmd_h;

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

  GLuint shaderHMD;
};
