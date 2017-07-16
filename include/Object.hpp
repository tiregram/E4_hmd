#pragma once

#include <glm/glm.hpp>

class Object
{
public:

	Object(glm::mat4 m);
  virtual ~Object(){};

	virtual void setVPmatrix(glm::mat4* v, glm::mat4* p)=0;
	virtual void draw()= 0;
	virtual void update(double t)=0;
	virtual glm::mat4 get_model_matrix();
	virtual void set_father(Object* father);
	virtual Object* get_father() const;

	virtual int get_id(){return 0;}
//	virtual void rotate(float x, float y, float z)=0;
//	virtual void translate(glm::vec3* vec)=0;


private: 

	Object* father;

protected: 

	glm::mat4 modelMatrix;
	virtual glm::mat4 get_father_model_matrix();


};

