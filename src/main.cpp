#include <iostream>
#include <vector>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Cubemap.hpp"

#include "Shader.hpp"
#include "Object.hpp"
#include "Scene.hpp"
#include "Debug.hpp"
#include "ObjectContainer.hpp"
#include "TextureStreamSurface.hpp"

#include "GLFWContext.hpp"


int createGLEW()
{
	glewExperimental = true;
  if (glewInit() != GLEW_OK) {
    std::cerr<<"Failed to initialize GLEW\n";
		getchar();
		glfwTerminate();
    return -1;
  }

	printf("OpenGL Renderer: %s\n", glGetString(GL_RENDERER));
	printf("OpenGL Vendor: %s\n", glGetString(GL_VENDOR));
	printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
}


int create_opengl()
{
	// Dark blue background
  glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

}



int main(int argc, char *argv[])
{
  OpenHmdWrapper hmd= OpenHmdWrapper();

  GLFWContext glfw_context = GLFWContext(hmd.get_hmd_w(),hmd.get_hmd_h());

  createGLEW();
  create_opengl();

  hmd.createShader();

  Scene sce(hmd,glfw_context);

  glm::mat4 d(1.0);
  glm::mat4 x=glm::translate(d,glm::vec3(0.0f, 0.0f, -15.0f));

  glm::mat4 sxx=glm::translate(d,glm::vec3(0.0f, 0.0f, -5.0f));
  glm::mat4 sxx2=glm::translate(d,glm::vec3(3.0f, 0.0f, -5.0f));

  sce.addObject(new Cubemap(glm::mat4(1.0),"obj/uvmap.DDS"));

  /*
  sce.addObject(new Object3D(x,
                                     "obj/untitled.obj",
                                     "obj/uvmap.DDS",
                                     "shader/StandardShading.vertexshader",
                                     "shader/StandardShading.fragmentshader"));

  glm::mat4 xx=glm::translate(d,glm::vec3(-3.0f, 0.0f, -5.0f));
  sce.addObject(new Object3D(xx,
                                     "obj/untitled.obj",
                                     "obj/uvmap.DDS",
                                     "shader/StandardShading.vertexshader",
                                     "shader/StandardShading.fragmentshader"));

  glm::mat4 y=  glm::translate(d,glm::vec3(2.0f, 0.0f, 0.0f));
  sce.addObject(new Object3D(y,
                             "obj/suzanne.obj",
                             "obj/uvmap.DDS",
                             "shader/StandardShading.vertexshader",
                             "shader/StandardShading.fragmentshader"));


  //  auto testStephenStream= new TextureStreamSurface(sxx, 100, 100);
  */
  auto testStephenStream2= new TextureStreamSurface(sxx2, 100, 100);


  //sce.addObject(testStephenStream);
  sce.addObject(testStephenStream2);
  /*glm::mat4 z=  glm::translate(d,glm::vec3(0.0f, 0.0f, 0.0f));
  sce.addObject(new Object3D(z,
                                  "obj/suzanne.obj",
                                  "obj/uvmap.DDS",
                                  "shader/StandardShading.vertexshader",
                                  "shader/StandardShading.fragmentshader"));
*/

  glm::mat4 yy=  glm::translate(d,glm::vec3(2.0f, 0.0f, 0.0f));
  Object3D* suzanne_one =  new Object3D(yy,
                             "obj/suzanne.obj",
                             "obj/uvmap.DDS",
                             "shader/StandardShading.vertexshader",
                             "shader/StandardShading.fragmentshader"); 

  glm::mat4 zz=  glm::translate(d,glm::vec3(-2.0f, 0.0f, 0.0f));
  Object3D* suzanne_two =  new Object3D(zz,
                                  "obj/suzanne.obj",
                                  "obj/uvmap.DDS",
                                  "shader/StandardShading.vertexshader",
                                  "shader/StandardShading.fragmentshader");


  //glm::rotate(d, glm::vec3(90,0,0));
  glm::mat4 m=  glm::translate(d,glm::vec3(0.0f, 0.0f, 0.0f));
  ObjectContainer * container = new ObjectContainer(m);
  container->add_object(suzanne_one);
  container->add_object(suzanne_two);

  sce.addObject(container);


  // update_debug(glfw_context,sce.objects);
  //draw_debug(glfw_context,sce.objects);

  while(sce.update())
    sce.draw();

  for(auto& a : sce.objects )
    {
      delete(a);
    }

  return 0;
}
