#pragma once

#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

class GLFWContext
{
public:

  GLFWContext(const unsigned int w, const unsigned int h);
  GLFWwindow* getWindow();

  void computeMatricesFromInputs();
  glm::mat4 getViewMatrix();
  glm::mat4 getProjectionMatrix();


private:

	GLFWwindow* window;
  glm::mat4 ViewMatrix;
  glm::mat4 ProjectionMatrix;

  // Initial position : on +Z
  glm::vec3 position = glm::vec3( 0, 0, 5 );
  // Initial horizontal angle : toward -Z
  float horizontalAngle = 3.14f;
  // Initial vertical angle : none
  float verticalAngle = 0.0f;
  // Initial Field of View
  float initialFoV = 45.0f;
  float speed = 3.0f; // 3 units / second
  float mouseSpeed = 0.005f;

};
