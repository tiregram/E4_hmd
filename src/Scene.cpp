#include <openhmd/openhmd.h>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Scene.hpp"
#include "shader.hpp"
#include "controls.hpp"

#define OVERSAMPLE_SCALE 2.0


Scene::Scene() {
	int hmd_w, hmd_h;

	ohmd_context* ctx = ohmd_ctx_create();
	int num_devices = ohmd_ctx_probe(ctx);
	if(num_devices < 0){
		std::cout <<"failed to probe devices: %s\n"<< ohmd_ctx_get_error(ctx)  << "\n";
  }

	ohmd_device_settings* settings = ohmd_device_settings_create(ctx);
  int auto_update = 1;

	ohmd_device_settings_seti(settings, OHMD_IDS_AUTOMATIC_UPDATE, &auto_update);
	ohmd_device* hmd = ohmd_list_open_device_s(ctx, 0, settings);

	ohmd_device_geti(hmd, OHMD_SCREEN_HORIZONTAL_RESOLUTION, &hmd_w);
	ohmd_device_geti(hmd, OHMD_SCREEN_VERTICAL_RESOLUTION, &hmd_h);


  float ipd;
	ohmd_device_getf(hmd, OHMD_EYE_IPD, &ipd);
	float viewport_scale[2];
	float distortion_coeffs[4];
	float aberr_scale[3];
	float sep;
	float left_lens_center[2];
	float right_lens_center[2];
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
	left_lens_center[0] = viewport_scale[0] - sep/2.0f;
	right_lens_center[0] = sep/2.0f;
	//asume calibration was for lens view to which ever edge of screen is further away from lens center
	float warp_scale = (left_lens_center[0] > right_lens_center[0]) ? left_lens_center[0] : right_lens_center[0];
	float warp_adj = 1.0f;

	ohmd_device_settings_destroy(settings);



	if(!hmd){
		std::cout <<"failed to open device:" << ohmd_ctx_get_error(ctx) << "\n";
  }


	const char* vertex_HMD;
	ohmd_gets(OHMD_GLSL_DISTORTION_VERT_SRC, &vertex_HMD);
	const char* fragment_HMD;
	ohmd_gets(OHMD_GLSL_DISTORTION_FRAG_SRC, &fragment_HMD);

	shaderHMD = LoadShaders(vertex_HMD, fragment_HMD);
	glUseProgram(shaderHMD);
	glUniform1i(glGetUniformLocation(shaderHMD, "warpTexture"), 0);
	glUniform2fv(glGetUniformLocation(shaderHMD, "ViewportScale"), 1, viewport_scale);
	glUniform3fv(glGetUniformLocation(shaderHMD, "aberr"), 1, aberr_scale);
	glUseProgram(0);

	int eye_w = hmd_w/2*OVERSAMPLE_SCALE;
	int eye_h = hmd_h*OVERSAMPLE_SCALE;

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
  do{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int i = 0;

    computeMatricesFromInputs();

    for(auto& one_obj : objects){
      one_obj->draw();
    }

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

  }while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
          glfwWindowShouldClose(window) == 0 );


}

void  Scene::update()
{

}


void Scene::create_fbo(int eye_width, int eye_height, GLuint* fbo, GLuint* color_tex, GLuint* depth_tex)
{
	glGenTextures(1, color_tex);
	glGenTextures(1, depth_tex);
	glGenFramebuffers(1, fbo);

	glBindTexture(GL_TEXTURE_2D, *color_tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, eye_width, eye_height, 0, GL_RGBA, GL_UNSIGNED_INT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, *depth_tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, eye_width, eye_height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, *fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *color_tex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, *depth_tex, 0);

	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);

	if(status != GL_FRAMEBUFFER_COMPLETE_EXT){
		printf("failed to create fbo %x\n", status);
	}
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}
