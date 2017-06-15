#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.hpp"
#include "Object3D.hpp"
#include "controls.hpp"
#include "Scene.hpp"

//pour Debug PBO
#include "TextureStreamSurface.hpp"


#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <cstring>

#define GLOB_WIDTH 1000
#define GLOB_HEIGHT 1000

#define IMAGE_WIDTH 100
#define IMAGE_HEIGHT 100
#define PIXEL_FORMAT GL_BGRA
#define DATA_SIZE 40000

GLFWwindow* window;





int createGLFWContext()
{
  // Initialise GLFW
  if( !glfwInit() )
    {
      std::cerr<< "Failed to initialize GLFW"<<"\n";
      return -1;
    }

  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow( GLOB_WIDTH, GLOB_HEIGHT, "one", NULL, NULL);

  if( window == NULL ){
    std::cerr<< "Failed to open GLFW window. "
             << "If you have an Intel GPU, they are not 3.3 compatible."
             <<" Try the 2.1 version of the tutorials."<<"\n";
    getchar();
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  // Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  // Hide the mouse and enable unlimited mouvement
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // Set the mouse at the center of the screen
  glfwPollEvents();
  glfwSetCursorPos(window, GLOB_WIDTH/2, GLOB_HEIGHT/2);

}


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

int update(std::vector<Object3D*>& a)
{
  for(auto& one_obj : a)
    {
      double t = 0.1;
      one_obj->update(t);
    }
}

int draw(std::vector<Object3D*>& a, std::vector<TextureStreamSurface*>& b )
{

  do{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int i = 0;

    computeMatricesFromInputs();

    for(auto& one_obj : a ){
        // std::cout << "draw " <<i++<< "\n";
        one_obj->draw();
      }
  	//callBackStephen(textureId, pboId, imageData, shaderId, uvId, elementId, vertId);

    
    for(auto& test : b)
      {
        test->draw();
      }
    // Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

  }while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
         glfwWindowShouldClose(window) == 0 );


}


int main(int argc, char *argv[])
{
  createGLFWContext();
  createGLEW();
  create_opengl();

  //Scene sce;
  std::vector<Object3D*> objs;
  glm::mat4 d(1.0);
  glm::mat4 x=glm::translate(d,glm::vec3(0.0f, 0.0f, -15.0f));

  glm::mat4 xx=glm::translate(d,glm::vec3(0.0f, 0.0f, -5.0f));
  glm::mat4 xx2=glm::translate(d,glm::vec3(3.0f, 0.0f, -5.0f));

  //
  GLuint pboId;
  GLuint shaderId;
  GLuint vertId;
  GLuint uvId;
  GLuint elementId;
  GLuint textureId;

  GLubyte* imageData = 0;  
  imageData = new GLubyte[DATA_SIZE];
  memset(imageData, 0, DATA_SIZE);

  TextureStreamSurface testStephenStream(xx, 200, 200);
  TextureStreamSurface testStephenStream2(xx2, 200, 200);

  std::vector<TextureStreamSurface*> texturesStephen;

  for(int i=0;i<4;i++)
    {
      for(int j =0; j<4;j++)
        {
          texturesStephen.push_back(new TextureStreamSurface(glm::translate(d, glm::vec3(3 *i,3 * j,-5.0f))));          
        }
    }



  //initPBO(&shaderId, &pboId, &vertId, &uvId, &elementId, &textureId, imageData);

  objs.push_back(new Object3D(x,
                                  "obj/untitled.obj",
                                  "obj/uvmap.DDS",
                                  "shader/StandardShading.vertexshader",
                                  "shader/StandardShading.fragmentshader"));

  // glm::mat4 y=  glm::translate(d,glm::vec3(2.0f, 0.0f, 0.0f));
  // sce.objects.push_back(new Object3D(y,
  //                                 "obj/suzanne.obj",
  //                                 "obj/uvmap.DDS",
  //                                 "shader/StandardShading.vertexshader",
  //                                 "shader/StandardShading.fragmentshader"));

  // glm::mat4 z=  glm::translate(d,glm::vec3(-2.0f, 0.0f, 0.0f));
  // sce.objects.push_back(new Object3D(z,
  //                                 "obj/suzanne.obj",
  //                                 "obj/uvmap.DDS",
  //                                 "shader/StandardShading.vertexshader",
  //                                 "shader/StandardShading.fragmentshader"));

  // update(objs);
  //draw(objs, &textureId, &pboId,imageData, &shaderId, &uvId, &elementId, &vertId);
  draw(objs, texturesStephen);


  //sce.window = window;


  //while(sce.update())
    //sce.draw();



  for(auto& a : objs )
    {
      delete(a);
    }

  return 0;
}
