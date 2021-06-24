#pragma once
#include "Object.h"
#include "PhysicsAttribute.h"

typedef struct _GridCell 
{
	sf::Vector2f cell_pos;
	sf::Vector2f cell_size;
	std::vector<Object*> objects;
} GridCell;

/* CollisionDetection
*  Self-explanatory (WIP).
*/
class CollisionDetection : public PhysicsAttribute
{
public:
	CollisionDetection();
	~CollisionDetection();



private:
	std::vector<Object*> scene_objects;
	std::vector <GridCell> grid;

};

