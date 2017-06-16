#pragma once

#include "Object.hpp"
#include <vector>

class ObjectContainer : public Object
{
public:

	ObjectContainer(glm::mat4 m);

	~ObjectContainer() noexcept{};

	std::vector<Object *> get_objects();

	bool remove_object(Object * object_to_remove);


	void add_object(Object * object_to_add);

	void rotate_container(float x, float y, float z);


	// FROM Object.
	virtual void update(double delta_time);
	virtual void draw();
	virtual void setVPmatrix(glm::mat4* v, glm::mat4* p);


private: 
	
	std::vector<Object*> objects_contained;
  	glm::mat4* projectionMatrix ;
  	glm::mat4* viewMatrix;


};

