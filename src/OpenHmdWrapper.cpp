
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

}

void OpenHmdWrapper::getViewMatrix(SIDE s, glm::mat4& view) {

  if(s == LEFT)
    ohmd_device_getf(hmd, OHMD_LEFT_EYE_GL_MODELVIEW_MATRIX, &view[0][0]);
  else if(s == RIGHT)
    ohmd_device_getf(hmd, OHMD_RIGHT_EYE_GL_MODELVIEW_MATRIX, &view[0][0]);

}

void OpenHmdWrapper::getProjMatrix(SIDE s, glm::mat4& view) {

  if(s == LEFT)
    ohmd_device_getf(hmd, OHMD_LEFT_EYE_GL_PROJECTION_MATRIX, &view[0][0]);
  else if(s == RIGHT)
    ohmd_device_getf(hmd, OHMD_RIGHT_EYE_GL_PROJECTION_MATRIX, &view[0][0]);

}


OpenHmdWrapper::OpenHmdWrapper(const OpenHmdWrapper& other) {

}


void OpenHmdWrapper::bindShader() {
  glUseProgram(shaderHMD);
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

	shaderHMD = LoadShadersFromPathFile("shader/vert.glsl","shader/frag.glsl" );
	// glUseProgram(shaderHMD);
	// glUniform1i(glGetUniformLocation(shaderHMD, "warpTexture"), 0);
	// glUniform2fv(glGetUniformLocation(shaderHMD, "ViewportScale"), 1, viewport_scale);
	// glUniform3fv(glGetUniformLocation(shaderHMD, "aberr"), 1, aberr_scale);
	// glUseProgram(0);
}



//! Destructor
OpenHmdWrapper::~OpenHmdWrapper() noexcept{}
