#pragma once
#include "Object3D.hpp"



class Object3DPBO:Object3D
{

public:

  Object3DPBO(glm::mat4 m,
              const char * obj_file,
              const char * texture_file,
              const char * vert_shader_file,
              const char * frag_shader_file,
              );

  Object3DPBO(const Object3DPBO &other);

  virtual ~Object3D() noexcept{};

  void createObjectPBO(const char * obj_file);


  // Specific PBO Part
  GLuint* pboIds;







}
