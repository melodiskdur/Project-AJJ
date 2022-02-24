#include "PropertyNode.h"
unsigned int PropertyNode::instance_counter = 0;

PropertyNode::PropertyNode() { instance_counter++; }

PropertyNode::~PropertyNode() { instance_counter--; }

