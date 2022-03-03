#pragma once
#include "Object.h"

class Object;

/* PropertyNode
*  An abstract base class for a set of classes that are meant to connect
*  Objects to different properties such as rendering, controls (?), actions (?),
*  animations (?), physics attributes etc. New classes can be derived from
*  PropertyNode to extend the opportunities of adding data to Objects.
*/
class PropertyNode
{
public:
	PropertyNode();
	virtual ~PropertyNode();
	// Getters.
	Object* getParentObject() { return this->parent_object; };
	sf::String getPropertyName() { return this->property_name; };
	// Setters.
	void setPropertyName(sf::String p) { this->property_name = p; };
	// Others.
	virtual bool connectTo(Object* parent);
	virtual bool disconnectFrom(Object* parent);

	static unsigned int instanceCount() { return instance_counter; };
protected:
	sf::String property_name = "";
	Object* parent_object = nullptr;
private:
	static unsigned int instance_counter;
};

