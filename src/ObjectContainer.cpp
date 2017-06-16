#include "ObjectContainer.hpp"
#include <iostream>




ObjectContainer::ObjectContainer(glm::mat4 m):modelMatrix(m)
{

}

std::vector<Object *> ObjectContainer::get_objects()
{
	return this->objects_contained;
}


bool ObjectContainer::remove_object(Object * object_to_remove)
{
	for( std::vector<Object*>::iterator iter = this->objects_contained.begin(); iter != this->objects_contained.end(); ++iter )
	{
	    if( *iter == object_to_remove )
	    {
	        this->objects_contained.erase( iter );
	        return true;
	    }
	}
	return false;
}

void ObjectContainer::add_object(Object * object_to_add)
{
	this->objects_contained.push_back(object_to_add);
}


void ObjectContainer::update(double delta_time)
{
	for(auto o : this->objects_contained)
		o->update(delta_time);
}

void ObjectContainer::draw()
{
	for(auto o : this->objects_contained)		
		o->draw();	
}

void ObjectContainer::setVPmatrix(glm::mat4* v, glm::mat4* p)
{
	for(auto o : this->objects_contained)
		o->setVPmatrix(v, p);
}


	





