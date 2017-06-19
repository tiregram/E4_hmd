
#include "OpenHmdWrapper.hpp"

#include <iostream>
#include "Shader.hpp"



OpenHmdWrapper::OpenHmdWrapper() {

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

  std::cout << "screen"<<this->hmd_w <<"x"<< this->hmd_h<< "\n";

	left_lens_center[0] = viewport_scale[0] - sep/2.0f;
	right_lens_center[0] = sep/2.0f;
	//asume calibration was for lens view to which ever edge of screen is further away from lens center
	warp_scale = (left_lens_center[0] > right_lens_center[0]) ? left_lens_center[0] : right_lens_center[0];
	warp_adj = 1.0f;

	eye_w = hmd_w/2;
	eye_h = hmd_h;

	ohmd_device_settings_destroy(settings);

	if(!hmd){
		std::cout <<"failed to open device:" << ohmd_ctx_get_error(ctx) << "\n";
  }

  ////////////////////////// DEBUG WITHOUT OCULUS /////////////////////////////////////////////:
  distortion_coeffs[0] = 0.098;
  distortion_coeffs[1] = 0.324;
  distortion_coeffs[2] = -0.241;
  distortion_coeffs[3] = 0.819;

  aberr_scale[0] = 0.9952420;
  aberr_scale[1] = 1.0;
  aberr_scale[2] = 1.0008074;

  sep = 0.054;
  //////////////////////////////////////////////////////////////////////////////////////////////////



}

void OpenHmdWrapper::getViewMatrix(SIDE s, glm::mat4& view) {

  if(s == LEFT)
    ohmd_device_getf(hmd, OHMD_LEFT_EYE_GL_MODELVIEW_MATRIX, &view[0][0]);
  else if(s == RIGHT)
    ohmd_device_getf(hmd, OHMD_RIGHT_EYE_GL_MODELVIEW_MATRIX, &view[0][0]);

}

void OpenHmdWrapper::update() {
  ohmd_ctx_update(ctx);
}

void OpenHmdWrapper::getProjMatrix(SIDE s, glm::mat4& view) {

  if(s == LEFT)
    ohmd_device_getf(hmd, OHMD_LEFT_EYE_GL_PROJECTION_MATRIX, &view[0][0]);
  else if(s == RIGHT)
    ohmd_device_getf(hmd, OHMD_RIGHT_EYE_GL_PROJECTION_MATRIX, &view[0][0]);

}


OpenHmdWrapper::OpenHmdWrapper(const OpenHmdWrapper& other) {

}


void OpenHmdWrapper::bindShader(SIDE s) {
  glUseProgram(shaderHMD);

  // getting IDs for each uniform variable
  GLuint warpTextureID = glGetUniformLocation(shaderHMD, "warpTexture");
  GLuint LensCenterID = glGetUniformLocation(shaderHMD, "LensCenter");
  GLuint ViewportScaleID = glGetUniformLocation(shaderHMD, "ViewportScale");
  GLuint WarpScaleID = glGetUniformLocation(shaderHMD, "WarpScale");
  GLuint HmdWarpParamID = glGetUniformLocation(shaderHMD, "HmdWarpParam");
  GLuint aberrID = glGetUniformLocation(shaderHMD, "aberr");

  glUniform1i(warpTextureID, 0);

  
  glUniform2fv(ViewportScaleID,1,viewport_scale);
  glUniform1f(WarpScaleID, warp_scale*warp_adj);
  glUniform4fv(HmdWarpParamID,1, distortion_coeffs);
  glUniform3fv(aberrID, 1, aberr_scale);


  /*
  printf("viewport_scale: [%0.4f, %0.4f]\n", viewport_scale[0], viewport_scale[1]);
  printf("lens separation: %04f\n", sep);
  printf("IPD: %0.4f\n", ipd);
  printf("warp_scale: %0.4f\r\n", warp_scale);
  printf("distoriton coeffs: [%0.4f, %0.4f, %0.4f, %0.4f]\n", distortion_coeffs[0], distortion_coeffs[1], distortion_coeffs[2], distortion_coeffs[3]);
  printf("aberration coeffs: [%0.4f, %0.4f, %0.4f]\n", aberr_scale[0], aberr_scale[1], aberr_scale[2]);
  printf("left_lens_center: [%0.4f, %0.4f]\n", left_lens_center[0], left_lens_center[1]);
  printf("right_lens_center: [%0.4f, %0.4f]\n", right_lens_center[0], right_lens_center[1]);*/


  if(s == LEFT)
    {
      glUniform2fv(LensCenterID, 1, left_lens_center);
    }

  else
    {
      glUniform2fv(LensCenterID, 1, right_lens_center);
    }
  

  //std::cout << "BINDINGSHADER\n";

}

OpenHmdWrapper& OpenHmdWrapper::operator =(const OpenHmdWrapper& other) {
  OpenHmdWrapper b;

  return b;
}

void OpenHmdWrapper::createShader() {

  
	// const char* vertex_HMD;
	// ohmd_gets(OHMD_GLSL_DISTORTION_VERT_SRC, &vertex_HMD);
	// const char* fragment_HMD;
	// ohmd_gets(OHMD_GLSL_DISTORTION_FRAG_SRC, &fragment_HMD);

	shaderHMD = LoadShadersFromPathFile("shader/vertHMD.glsl","shader/fragHMD.glsl" );
  
	// glUseProgram(shaderHMD);
	// glUniform1i(glGetUniformLocation(shaderHMD, "warpTexture"), 0);
	// glUniform2fv(glGetUniformLocation(shaderHMD, "ViewportScale"), 1, viewport_scale);
	// glUniform3fv(glGetUniformLocation(shaderHMD, "aberr"), 1, aberr_scale);
	// glUseProgram(0);
}



//! Destructor
OpenHmdWrapper::~OpenHmdWrapper() noexcept{}
