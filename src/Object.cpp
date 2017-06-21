#include "Object.hpp"



Object::Object(glm::mat4 m):modelMatrix(m),father(NULL)
{

}

glm::mat4 Object::get_model_matrix()
{
	return this->get_father_model_matrix() * this->modelMatrix;
}

void Object::set_father(Object* father)
{
	this->father = father;
}

Object* Object::get_father() const
{
	return this->father;
}


glm::mat4 Object::get_father_model_matrix()
{
	if(this->father == NULL) 
	{
		return glm::mat4(1.0);
	}
	else
	{
		return this->father->get_model_matrix();
	}
}
