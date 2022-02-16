#include "Hitbox.h"

#define HITBOX_INFINITY 20000000.0f

Hitbox::Hitbox():
	PhysicsAttribute("Hitbox")
{

}

Hitbox::~Hitbox()
{

}
std::vector<ObjectData> Hitbox::separateHitboxes(Object* i, Object* j)
{
	sf::Vector2f wp_i = i->getWorldPosition();
	sf::Vector2f wp_j = j->getWorldPosition();
	sf::Vector2f size_i = i->getSize();
	sf::Vector2f size_j = j->getSize();

	sf::FloatRect r_i = sf::FloatRect(wp_i, size_i);
	sf::FloatRect r_j = sf::FloatRect(wp_j, size_j);

	sf::Vector2f vel_i = i->getVelocity();
	sf::Vector2f vel_j = j->getVelocity();

	// Return values.
	std::vector<ObjectData> wp_and_vel_re;

	// Special case: If no objects are moving.
	if (vel_i.x == 0 && vel_i.y == 0 && vel_j.x == 0 && vel_j.y == 0)
	{
		if (i->getBehaviorType() == OBJECT_BEHAVIOR::STATIC && j->getBehaviorType() == OBJECT_BEHAVIOR::STATIC)
			return wp_and_vel_re;
		else if (i->getBehaviorType() == OBJECT_BEHAVIOR::STATIC && j->getBehaviorType() == OBJECT_BEHAVIOR::DYNAMIC)
			wp_and_vel_re.push_back(Hitbox::singleObjectSeparation(j, i));
		else if (i->getBehaviorType() == OBJECT_BEHAVIOR::DYNAMIC && j->getBehaviorType() == OBJECT_BEHAVIOR::STATIC)
			wp_and_vel_re.push_back(Hitbox::singleObjectSeparation(i, j));
		else if (i->getBehaviorType() == OBJECT_BEHAVIOR::DYNAMIC && j->getBehaviorType() == OBJECT_BEHAVIOR::DYNAMIC)
			wp_and_vel_re = Hitbox::dualObjectSeparation(i, j);
		return wp_and_vel_re;
	}

	sf::Vector2f abs_vel_i = sf::Vector2f(std::abs(i->getVelocity().x), std::abs(i->getVelocity().y));
	sf::Vector2f abs_vel_j = sf::Vector2f(std::abs(j->getVelocity().x), std::abs(j->getVelocity().y));

	// If i is inside j or if j is inside i.
	if (    //i in j.
		(	r_j.contains(wp_i) &&												//Upper left corner.
			r_j.contains(sf::Vector2f(wp_i.x + size_i.x, wp_i.y)) &&			//Upper right corner.
			r_j.contains(sf::Vector2f(wp_i.x, wp_i.y + size_i.y)) &&			//Lower left corner.
			r_j.contains(sf::Vector2f(wp_i.x + size_i.x, wp_i.y + size_i.y)))   //Lower right corner.
		||	//j in i.
		(	r_i.contains(wp_j) &&									            //Upper left corner.
			r_i.contains(sf::Vector2f(wp_j.x + size_j.x, wp_j.y)) &&			//Upper right corner.
			r_i.contains(sf::Vector2f(wp_j.x, wp_j.y + size_j.y)) &&			//Lower left corner.
			r_i.contains(sf::Vector2f(wp_j.x + size_j.x, wp_j.y + size_j.y)))   //Lower right corner.
		)
	{
		// If i is moving and j is not.
		if ((abs_vel_i.x != 0 || abs_vel_i.y != 0)
			&& abs_vel_j.x == 0 && abs_vel_j.y == 0)
		{
			 wp_and_vel_re.push_back(Hitbox::singleObjectSeparation(i, j));
		}
		// If j is moving and i is not.
		else if ((abs_vel_j.x != 0 || abs_vel_j.y != 0)
			&& abs_vel_i.x == 0 && abs_vel_i.y == 0)
		{
			wp_and_vel_re.push_back(Hitbox::singleObjectSeparation(j, i));
		}
		// If both are moving.
		else
		{
			wp_and_vel_re = Hitbox::dualObjectSeparation(j, i);
		}
	}
	// If no object is encapsulated within the other.
	else
		//if i is moving and j is not.
		if ((abs_vel_i.x != 0 || abs_vel_i.y != 0)
			&& abs_vel_j.x == 0 && abs_vel_j.y == 0)
		{
			wp_and_vel_re.push_back(Hitbox::singleObjectSeparation(i, j));
		}
	// If j is moving and i is not.
		else if ((abs_vel_j.x != 0 || abs_vel_j.y != 0)
			&& abs_vel_i.x == 0 && abs_vel_i.y == 0)
		{
			wp_and_vel_re.push_back(Hitbox::singleObjectSeparation(j, i));
		}
	// If both are moving.
		else
		{
			wp_and_vel_re = Hitbox::dualObjectSeparation(i, j);
		}

	// Return the collected data.
	return wp_and_vel_re;
}
ObjectData Hitbox::singleObjectSeparation(Object* moving, Object* other)
{
	// Return value.
	ObjectData data;
	data.m_object = moving;
	data.m_colliding_object = other;
	INTERSECTED_SIDE intersect = INTERSECTED_SIDE::ODATA_NONE;

	sf::Vector2f vel = moving->getVelocity();
	sf::Vector2f prev_pos = moving->getWorldPosition() - vel;
	sf::Vector2f ul = prev_pos;
	sf::Vector2f ll = prev_pos + sf::Vector2f(0.0f, moving->getSize().y);
	sf::Vector2f ur = prev_pos + sf::Vector2f(moving->getSize().x, 0.0f);
	sf::Vector2f lr = prev_pos + moving->getSize();
	sf::Vector2f new_pos = moving->getWorldPosition();
	sf::Vector2f new_vel = moving->getVelocity();
	
	// Axis-aligned velocity.
	if (vel.x == 0)
	{
		new_pos.y = vel.y < 0 ? other->getWorldPosition().y + other->getSize().y
			:                   other->getWorldPosition().y - moving->getSize().y;
		new_vel.y = 0;
		intersect = vel.y < 0 ? INTERSECTED_SIDE::ODATA_TOP : INTERSECTED_SIDE::ODATA_BOTTOM;
	}
	else if (vel.y == 0)
	{
		new_pos.x = vel.x < 0 ? other->getWorldPosition().x + other->getSize().x
			:					other->getWorldPosition().x - moving->getSize().x;
		new_vel.x = 0;
		intersect = vel.x < 0 ? INTERSECTED_SIDE::ODATA_RIGHT : INTERSECTED_SIDE::ODATA_LEFT;
	}
	// Non-axis-aligned velocity.
	else
	{
		float t_vert, t_hori;							//Vertical and horizontal parameters to calculate intersection points in the moving direction.
		sf::Vector2f insec_vert[2];					    //Vertical corners of object Other.
		sf::Vector2f insec_hori[2];					    //Horizontal corners of object Other.
		sf::Vector2f moving_vert[2];					//Vertical corners of object Moving.
		sf::Vector2f moving_hori[2];					//Horizontal corners of object Moving.

		// Set vertical and horizontal t parameters.
		t_vert = vel.x < 0 ? (other->getWorldPosition().x + other->getSize().x - ll.x) / vel.x	//Right side.
			: (other->getWorldPosition().x - lr.x) / vel.x;										//Left side.
		t_hori = vel.y < 0 ? (other->getWorldPosition().y + other->getSize().y - ul.y) / vel.y  //Bottom side.
			: (other->getWorldPosition().y - lr.y) / vel.y;									 	//Top side.

		// Calculate intersection points.
		if (vel.x < 0)
		{
			moving_vert[0] = ul;
			moving_vert[1] = ll;
			insec_vert[0] = sf::Vector2f(other->getWorldPosition().x + other->getSize().x, other->getWorldPosition().y);
			insec_vert[1] = other->getWorldPosition() + other->getSize();
		}
		else if (vel.x > 0)
		{
			moving_vert[0] = ur;
			moving_vert[1] = lr;
			insec_vert[0] = other->getWorldPosition();
			insec_vert[1] = sf::Vector2f(other->getWorldPosition().x, other->getWorldPosition().y + other->getSize().y);
		}

		if (vel.y < 0)
		{
			moving_hori[0] = ul;
			moving_hori[1] = ur;
			insec_hori[0] = sf::Vector2f(other->getWorldPosition().x, other->getWorldPosition().y + other->getSize().y);
			insec_hori[1] = other->getWorldPosition() + other->getSize();
		}
		else if (vel.y > 0)
		{
			moving_hori[0] = ll;
			moving_hori[1] = lr;
			insec_hori[0] = other->getWorldPosition();
			insec_hori[1] = sf::Vector2f(other->getWorldPosition().x + other->getSize().x, other->getWorldPosition().y);
		}

		// Vertical check + adjustment.
		if ((moving_vert[0].y + t_vert * vel.y > insec_vert[0].y && moving_vert[0].y + t_vert * vel.y < insec_vert[1].y) ||
			(moving_vert[1].y + t_vert * vel.y > insec_vert[0].y && moving_vert[1].y + t_vert * vel.y < insec_vert[1].y))
		{
			new_pos.x = vel.x < 0 ? other->getWorldPosition().x + other->getSize().x
				: other->getWorldPosition().x - moving->getSize().x;
			new_vel.x = 0;
			// Set intersected side.
			intersect = vel.x < 0 ? INTERSECTED_SIDE::ODATA_LEFT : INTERSECTED_SIDE::ODATA_RIGHT;
		}
		// Horizontal check + adjustment.
		else if ((moving_hori[0].x + t_hori * vel.x > insec_hori[0].x && moving_hori[0].x + t_hori * vel.x < insec_hori[1].x) ||
			(moving_hori[1].x + t_hori * vel.x > insec_hori[0].x && moving_hori[1].x + t_hori * vel.x < insec_hori[1].x))
		{
			new_pos.y = vel.y < 0 ? other->getWorldPosition().y + other->getSize().y
				: other->getWorldPosition().y - moving->getSize().y;
			new_vel.y = 0;
			// Set intersected side.
			intersect = vel.y < 0 ? INTERSECTED_SIDE::ODATA_TOP : INTERSECTED_SIDE::ODATA_BOTTOM;
			
		}
		// If something goes wrong. Do a simple unstuck-check.
		else
		{
			sf::FloatRect moving_hitbox = sf::FloatRect(moving->getWorldPosition(), moving->getSize());
			sf::FloatRect other_hitbox = sf::FloatRect(other->getWorldPosition(), other->getSize());
			std::vector<sf::Vector2f> unstuck_resolves = Hitbox::unstuck(moving_hitbox, other_hitbox, vel);
			new_vel = unstuck_resolves[1];
			new_pos = unstuck_resolves[0];
			// Set intersected side.
			if (new_pos.x - moving->getWorldPosition().x > 0)
				intersect = INTERSECTED_SIDE::ODATA_LEFT;
			else if (new_pos.x - moving->getWorldPosition().x < 0)
				intersect = INTERSECTED_SIDE::ODATA_RIGHT;
			else if (new_pos.y - moving->getWorldPosition().y > 0)
				intersect = INTERSECTED_SIDE::ODATA_TOP;
			else if (new_pos.y - moving->getWorldPosition().y < 0)
				intersect = INTERSECTED_SIDE::ODATA_BOTTOM;
		}
	}

	// Add updated velocities and return ObjectData.
	data.m_colliding_hitbox = sf::FloatRect(other->getWorldPosition(), other->getSize());
	data.m_vel = new_vel;
	data.m_wp = new_pos;
	data.m_intersect = intersect;
	return data;
}

std::vector<ObjectData> Hitbox::dualObjectSeparation(Object* i, Object* j)
{
	// Return value.
	std::vector<ObjectData> data;
	ObjectData i_data;
	ObjectData j_data;
	i_data.m_object = i;
	i_data.m_colliding_object = j;
	j_data.m_object = j;
	j_data.m_colliding_object = i;

	// Positions.
	sf::Vector2f ipos = i->getWorldPosition();
	sf::Vector2f jpos = j->getWorldPosition();

	// Velocities.
	sf::Vector2f ivel = i->getVelocity();
	sf::Vector2f jvel = j->getVelocity();

	// Previous positions.
	sf::Vector2f i_prev = i->getWorldPosition() - i->getVelocity();
	sf::Vector2f j_prev = j->getWorldPosition() - j->getVelocity();

	// X, Y overlaps.
	sf::Vector2f overlaps = Hitbox::getOverlaps(i, j);

	// Factors to move i and j by, 0.5 of the overlap for each direction initially.
	float i_c = 0.5;
	float j_c = 0.5;

	// Check if i and j are travelling parallel along x.
	if (Hitbox::sameXDirection(i, j))
	{
		// Only reposition i.
		if (std::abs(i->getVelocity().x) > std::abs(j->getVelocity().x))
		{
			i_c = 1;
			j_c = 0;
		}
		// Only reposition j.
		else
		{
			i_c = 0;
			j_c = 1;
		}
	}

	// Check if i and j are travelling parallel along y.
	if (Hitbox::sameYDirection(i, j))
	{
		// Only reposition i.
		if (i_prev.y < j_prev.y)
		{
			i_c = 1;
			j_c = 0;
		}
		// Only reposition j.
		else
		{
			i_c = 0;
			j_c = 1;
		}
	}

	// Calculate the separation distance multipliers.
	sf::Vector2f t = Hitbox::getT(i, j, overlaps);

	// If horizontal separation happens before vertical separation.
	if (t.x < t.y)
	{
		if (i_prev.x < j_prev.x)
		{
			ipos.x -= i_c * overlaps.x;
			jpos.x += j_c * overlaps.x;
			// Intersected sides.
			i_data.m_intersect = INTERSECTED_SIDE::ODATA_RIGHT;
			j_data.m_intersect = INTERSECTED_SIDE::ODATA_LEFT;
		}
		else
		{
			ipos.x += i_c * overlaps.x;
			jpos.x -= j_c * overlaps.x;
			// Intersected sides.
			i_data.m_intersect = INTERSECTED_SIDE::ODATA_LEFT;
			j_data.m_intersect = INTERSECTED_SIDE::ODATA_RIGHT;
		}
		ivel.x = 0;
		jvel.x = 0;
	}
	// If vertical spparation happens before horizontal separation.
	else
	{
		if (i_prev.y > j_prev.y)
		{
			ipos.y += i_c * overlaps.y;
			jpos.y -= j_c * overlaps.y;
			// Intersected sides.
			i_data.m_intersect = INTERSECTED_SIDE::ODATA_TOP;
			j_data.m_intersect = INTERSECTED_SIDE::ODATA_BOTTOM;
		}
		else
		{
			ipos.y -= i_c * overlaps.y;
			jpos.y += j_c * overlaps.y;
			// Intersected sides.
			i_data.m_intersect = INTERSECTED_SIDE::ODATA_BOTTOM;
			j_data.m_intersect = INTERSECTED_SIDE::ODATA_TOP;
		}
		ivel.y = 0;
		jvel.y = 0;
	}

	// Store data in the vector to be returned.
	i_data.m_wp = ipos;
	i_data.m_vel = { ivel.x, ivel.y };
	j_data.m_wp = jpos;
	j_data.m_vel = { jvel.x, jvel.y };
	i_data.m_colliding_hitbox = sf::FloatRect(j_data.m_wp, j->getSize());
	j_data.m_colliding_hitbox = sf::FloatRect(i_data.m_wp, i->getSize());
	data.push_back(i_data);
	data.push_back(j_data);
	return data;
}

sf::Vector2f Hitbox::getOverlaps(Object* i, Object* j)
{
	sf::Vector2f overlaps;
	sf::Vector2f sizes = i->getSize() + j->getSize();
	sf::Vector2f i_prev = i->getWorldPosition() - i->getVelocity();
	sf::Vector2f j_prev = j->getWorldPosition() - j->getVelocity();
	std::vector<Object*> x_order;
	std::vector<Object*> y_order;

	// Order i and j leftmost to rightmost.
	if (i_prev.x < j_prev.x)
		x_order.insert(x_order.begin(), { i, j });
	else
		x_order.insert(x_order.begin(), { j, i });

	// Order i and j upmost to downmost.
	if (i_prev.y < j_prev.y)
		y_order.insert(y_order.begin(), { i, j });
	else
		y_order.insert(y_order.begin(), { j, i });

	// Calculate the distance between the leftmost and rightmost points of i and j.
	float total_x = std::abs(x_order[1]->getWorldPosition().x + x_order[1]->getSize().x - x_order[0]->getWorldPosition().x);
	// Calculate the distance between the upmost and downmost points of i and j.
	float total_y = std::abs(y_order[1]->getWorldPosition().y + y_order[1]->getSize().y - y_order[0]->getWorldPosition().y);

	// Subtract the distances from (i.size + j.size) to retreive the overlaps.
	overlaps.x = std::abs(sizes.x - total_x);
	overlaps.y = std::abs(sizes.y - total_y);
	return overlaps;
}

sf::Vector2f Hitbox::getOverlaps(sf::FloatRect i_rect, sf::FloatRect j_rect)
{
	float overlap_x, overlap_y, total_x, total_y, diff_x, diff_y;
	total_x = i_rect.width + j_rect.width;
	total_y = i_rect.height + j_rect.height;
	diff_x = i_rect.left < j_rect.left ?
		std::abs(j_rect.left + j_rect.width - i_rect.left) :
		std::abs(i_rect.left + i_rect.width - j_rect.left);
	diff_y = i_rect.top < j_rect.top ?
		std::abs(j_rect.top + j_rect.height - i_rect.top) :
		std::abs(i_rect.top + i_rect.height - j_rect.top);

	overlap_x = total_x - diff_x < 0 ? 0.0f : total_x - diff_x;
	overlap_y = total_y - diff_y < 0 ? 0.0f : total_y - diff_y;
	return { overlap_x, overlap_y };
}

bool Hitbox::sameXDirection(Object* i, Object* j)
{
	sf::Vector2f i_prev = i->getWorldPosition() - i->getVelocity();
	sf::Vector2f j_prev = j->getWorldPosition() - j->getVelocity();
	// Make sure that one of the object is behind the other.
	if ( (i_prev.x + i->getSize().x < j_prev.x) || (j_prev.x + j->getSize().x < i_prev.x) )
	{
		return (i->getVelocity().x * j->getVelocity().x > 0);
	}
	return false;
}

bool Hitbox::sameYDirection(Object* i, Object* j)
{
	sf::Vector2f i_prev = i->getWorldPosition() - i->getVelocity();
	sf::Vector2f j_prev = j->getWorldPosition() - j->getVelocity();
	sf::FloatRect i_prev_rect = { i_prev, i->getSize() };
	sf::FloatRect j_prev_rect = { j_prev, j->getSize() };
	sf::Vector2f overlaps = Hitbox::getOverlaps(i_prev_rect, j_prev_rect);
	// Make sure that one of the object is behind the other.
	if (overlaps.x >= overlaps.y)
	{
		return (i->getVelocity().y * j->getVelocity().y > 0);
	}
	return false;
}

sf::Vector2f Hitbox::getT(Object* i, Object* j, sf::Vector2f overlaps)
{
	// Get previous positions.
	sf::Vector2f iprev = i->getWorldPosition() - i->getVelocity();
	sf::Vector2f jprev = j->getWorldPosition() - j->getVelocity();
	// Calculate denominators.
	float denom_x = std::abs(i->getVelocity().x - j->getVelocity().x);
	float denom_y = std::abs(i->getVelocity().y - j->getVelocity().x);
	// To avoid divisions close to 0 (resulting in huge T values),
	// only divide by denom if it's larger than 1.
	float t_x = denom_x < 1 ? overlaps.x : overlaps.x / denom_x;
	float t_y = denom_y < 1 ? overlaps.y : overlaps.y / denom_y;
	return { t_x, t_y };
}

/* In case the axis-aligned repositioning in Hitbox::singeObjectSeparation() fails, this function is called
* to do a kind of "last resort" resolve of the moving object's position. It tests the distance (no attention
* paid to velocity) from the moving object's current position to each of the sides of the other object's hitbox.
  the side that produces to shortest distane will be the position to which the moving object will be resolved. */
std::vector<sf::Vector2f> Hitbox::unstuck(sf::FloatRect moving, sf::FloatRect other, sf::Vector2f moving_vel)
{
	std::vector<sf::Vector2f> resolves =
	{
		{ moving.left, other.top - moving.height },			// Top.
		{ other.left + other.width, moving.top },			// Right.
		{ moving.left, other.top + other.height},			// Bottom.
		{ other.left - moving.width, moving.top }			// Left.
	};
	
	std::vector<sf::Vector2f> new_velocities =
	{
		{ moving_vel.x, 0.f },								// Top.
		{ 0.f, moving_vel.y },								// Right.
		{ moving_vel.y, 0.f },								// Bottom.
		{ 0.f, moving_vel.y }								// Left.
	};

	float current_shortest = HITBOX_INFINITY;
	int current_best_index = -1;

	for (int i = 0; i < 4; i++)
	{
		if ((i % 2 == 1) && std::abs(resolves[i].x - moving.left) < current_shortest)
		{
			current_shortest = std::abs(resolves[i].x - moving.left);
			current_best_index = i;
		}
		if ((i % 2 == 0) && std::abs(resolves[i].y - moving.top) < current_shortest)
		{
			current_shortest = std::abs(resolves[i].y - moving.top);
			current_best_index = i;
		}
	}
	return { resolves[current_best_index], new_velocities[current_best_index] };
}

/* Second version of unstuck without the parameter moving_vel. Compared to the previous
*  version, this one only returns the new position.
*/
sf::Vector2f Hitbox::unstuck(sf::FloatRect moving, sf::FloatRect other)
{
	std::vector<sf::Vector2f> resolves =
	{
		{ moving.left, other.top - moving.height },			// Top.
		{ other.left + other.width, moving.top },			// Right.
		{ moving.left, other.top + other.height},			// Bottom.
		{ other.left - moving.width, moving.top }			// Left.
	};

	float current_shortest = HITBOX_INFINITY;
	int current_best_index = -1;

	for (int i = 0; i < 4; i++)
	{
		if ((i % 2 == 1) && std::abs(resolves[i].x - moving.left) < current_shortest)
		{
			current_shortest = std::abs(resolves[i].x - moving.left);
			current_best_index = i;
		}
		if ((i % 2 == 0) && std::abs(resolves[i].y - moving.top) < current_shortest)
		{
			current_shortest = std::abs(resolves[i].y - moving.top);
			current_best_index = i;
		}
	}
	return resolves[current_best_index];
}

std::vector<sf::Vector2f> Hitbox::recalibrate(const ObjectData& odata, const sf::Vector2f& i_pos, const INTERSECTED_SIDE& adj_intersect, const sf::Vector2f& j_pos)
{
	// Quick check to see if the collision is double-sided.
	bool is_double = (adj_intersect != INTERSECTED_SIDE::ODATA_NONE);

	sf::Vector2f i_updated = i_pos;
	sf::Vector2f j_updated = j_pos;
	sf::Vector2f i_size = odata.m_object->getSize();
	sf::Vector2f j_size = odata.m_colliding_object->getSize();

	float i_c = is_double ? 0.5f : 1.f;
	float j_c = 1.f - i_c;
	sf::Vector2f overlaps = Hitbox::getOverlaps({ i_pos, i_size }, { j_pos, j_size });
	
	switch (odata.m_intersect)
	{
		case INTERSECTED_SIDE::ODATA_TOP:
		{
			
			if (is_double)
			{
				i_updated.y += i_c * overlaps.y;
				j_updated.y -= j_c * overlaps.y;
			}
			else
				i_updated.y = j_updated.y + j_size.y;
			break;
		}
		case INTERSECTED_SIDE::ODATA_RIGHT:
		{
			if (is_double)
			{
				i_updated.x -= i_c * overlaps.x;
				j_updated.x += j_c * overlaps.x;
			}
			else
				i_updated.x = j_updated.x - i_size.x;
			break;
		}
		case INTERSECTED_SIDE::ODATA_BOTTOM:
		{
			if (is_double)
			{
				i_updated.y -= i_c * overlaps.y;
				j_updated.y += j_c * overlaps.y;
			}
			else
				i_updated.y = j_updated.y - i_size.y;
			break;
		}
		case INTERSECTED_SIDE::ODATA_LEFT:
		{
			if (is_double)
			{
				i_updated.x += i_c * overlaps.x;
				j_updated.x -= j_c * overlaps.x;
			}
			else
				i_updated.x = j_updated.x + j_size.x;
			break;
		}
	}

	return { i_updated, j_updated };
}
