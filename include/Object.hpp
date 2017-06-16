#pragma once

#include <glm/glm.hpp>

class Object
{
public:

	Object(glm::mat4 m):modelMatrix(m),father(NULL){};
	virtual void setVPmatrix(glm::mat4* v, glm::mat4* p)=0;
	virtual void draw()= 0;
	virtual void update(double t)=0;
	virtual glm::mat4 get_model_matrix()
	{
		return this->get_father_model_matrix() * this->modelMatrix;
	}
	virtual void set_father(Object* father)
	{
		this->father = father;
	}
	virtual Object* get_father() const
	{
		return this->father;
	}
	
//	virtual void rotate(float x, float y, float z)=0;
//	virtual void translate(glm::vec3* vec)=0;


private: 
	Object* father;
	
protected: 

	glm::mat4 modelMatrix;


	virtual glm::mat4 get_father_model_matrix()
	{
		if(this->father == NULL) 
			return glm::mat4(1.0);
		else
		{
			return this->father->get_model_matrix();
		}

	}


};

