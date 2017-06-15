#include "TextureStreamSurface.hpp"
#include "Shader.hpp"

#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <cstring>

// For testing purposes
#include <time.h> 

TextureStreamSurface::TextureStreamSurface(glm::mat4 m, GLuint width, GLuint height):modelMatrix(m){
  widthPBO=width;
  heightPBO=height;
  dataSize=widthPBO*heightPBO*4;


  // Buffer utilise par le PBO
  imageData = 0;
  imageData = new GLubyte[dataSize];
  memset(imageData, 0, dataSize);

  std::cout << "PBO Plane width and height set to : " << widthPBO << " x " << heightPBO << "\n";

	// Partie Textures
  glGenTextures(1, &TextureID);
  glBindTexture(GL_TEXTURE_2D, TextureID);

  std::cout << "PBO Bind Texture\n";


  //parametrage texture 2D
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  //  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, IMAGE_WIDTH, IMAGE_HEIGHT, 0, PIXEL_FORMAT, GL_UNSIGNED_BYTE, (GLvoid*)imageData);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, widthPBO,heightPBO, 0, GL_BGRA, GL_UNSIGNED_BYTE, (GLvoid*)imageData);
  glBindTexture(GL_TEXTURE_2D, 0);

  std::cout << "PBO : Preparing to load Shaders\n";	
  // Partie Shader
	shaderID=LoadShadersFromPathFile("shader/vertPBO.glsl","shader/fragPBO.glsl" );


//Partie PBO
	glGenBuffersARB(1, &pboID);

	glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, pboID);
	glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB, dataSize,0, GL_STREAM_READ_ARB);
	glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB,0);




//Partie Element
  unsigned short element_left[6] = {0,1,2,2,3,0};

  glGenBuffers(1, &elementBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned short), element_left, GL_STATIC_DRAW);


//Partie Vertex
  std::vector<glm::vec3> vertices_left(4);
  /*  vertices_left[0] = glm::vec3(0,1,0);
  vertices_left[1] = glm::vec3(-1,1,0);
  vertices_left[2] = glm::vec3(-1,-1,0);
  vertices_left[3] = glm::vec3(0,-1,0);*/

  vertices_left[0] = glm::vec3(1 ,1,0);
  vertices_left[1] = glm::vec3(-1,1,0);
  vertices_left[2] = glm::vec3(-1,-1,0);
  vertices_left[3] = glm::vec3(1,-1,0);


  glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, vertices_left.size() * sizeof(glm::vec3), &vertices_left[0], GL_STATIC_DRAW);

//partie UV
  std::vector<glm::vec2> uv_left(4);
  uv_left[0] = glm::vec2(1,1);
  uv_left[1] = glm::vec2(0,1);
  uv_left[2] = glm::vec2(0,0);
  uv_left[3] = glm::vec2(1,0);

  glGenBuffers(1, &uvBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, uvBufferID);
	glBufferData(GL_ARRAY_BUFFER, uv_left.size() * sizeof(glm::vec2), &uv_left[0], GL_STATIC_DRAW);

	std::cout << "PBO : Finished constructing Object\n";

}

TextureStreamSurface::TextureStreamSurface(const TextureStreamSurface& other) {
  
}

void TextureStreamSurface::setVPmatrix(glm::mat4* v, glm::mat4* p) {
  this->viewMatrix = v;
  this->projectionMatrix =p;
}

void TextureStreamSurface::createObject() {
  
}

void TextureStreamSurface::createOpengl() {
  
}

TextureStreamSurface& TextureStreamSurface::operator =(const TextureStreamSurface& other) {
  //TextureStreamSurface b;


  return *this;
}

void TextureStreamSurface::update(double delta_time) {
  
}

void TextureStreamSurface::draw() {



  glBindTexture(GL_TEXTURE_2D, TextureID);
  glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, pboID);
  //

  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, widthPBO, heightPBO, GL_BGRA, GL_UNSIGNED_BYTE, 0);


  // bind PBO to update pixel values
  glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, pboID);

  // map the buffer object into client's memory
  glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB, dataSize, 0, GL_STREAM_DRAW_ARB);
  GLubyte* ptr = (GLubyte*)glMapBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, GL_WRITE_ONLY_ARB);
  if(ptr)
    {
      // update data directly on the mapped buffer
      updatePixels(ptr, dataSize);
      glUnmapBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB); // release pointer to mapping buffer
    }


  glUseProgram(shaderID);


  ///////////////////////////////////////////////////////////
  //////////         Set Uniform Values          ////////////
  ///////////////////////////////////////////////////////////

  glm::mat4 MVP = *this->projectionMatrix * *this->viewMatrix * this->modelMatrix;

  ModelMatrixID = glGetUniformLocation(shaderID, "M");
  ViewMatrixID  = glGetUniformLocation(shaderID, "V");
  ProjectionMatrixID = glGetUniformLocation(shaderID, "P");

  MatrixID = glGetUniformLocation(shaderID, "MVP");



  glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &modelMatrix[0][0]);
  glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &(*viewMatrix)[0][0]);
  glUniformMatrix4fv(ProjectionMatrixID, 1, GL_FALSE, &(*projectionMatrix)[0][0]);

  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);



  
  /////////////////////////////////////////////////////////////////////////////
  //                             load obj                                    //
  /////////////////////////////////////////////////////////////////////////////

  // Binding Vertex Array 
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);


  // Binding UVs
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, uvBufferID);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

  glActiveTexture(GL_TEXTURE0);

  glBindTexture(GL_TEXTURE_2D, TextureID);
  glUniform1i(TextureID, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferID);

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);

  glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);

  //Updating pixels
  updatePixels(imageData, dataSize);



}


void TextureStreamSurface::updatePixels(GLubyte* dst, int size)
{
  static int unsigned v=0;
  // for testing purposes
  static int unsigned randGeneratedInt =0;
  srand (v++);
  randGeneratedInt = (rand()%(256*256*256)<<8)+256;


  if(!dst)
    return;

  int* ptr = (int*)dst;

  // copy 4 bytes at once
  for(int i = 0; i < heightPBO; ++i)
    {
      for(int j = 0; j < widthPBO; ++j)
        {
          randGeneratedInt++;
          *ptr = (rand()%(256*256*256)<<8)+256;
          ++ptr;

        }
    }
}


