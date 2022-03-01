#include "PropertyNode.h"
unsigned int PropertyNode::instance_counter = 0;

PropertyNode::PropertyNode() { instance_counter++; }

PropertyNode::~PropertyNode() { instance_counter--; }

bool PropertyNode::connectTo(Object* parent)
{
    if (this->parent_object != nullptr || parent == nullptr) return false;
    this->parent_object = parent;
    this->parent_object->addPropertyNode(this);
    return true;
}

bool PropertyNode::disconnectFrom(Object* parent)
{
    if (this->parent_object == nullptr) return false;
    this->parent_object = nullptr;
    return true;
}

