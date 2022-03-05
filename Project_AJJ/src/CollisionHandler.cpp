#include "CollisionHandler.h"

#define CollisionHandler_INFINITY 20000000.0f

CollisionHandler::CollisionHandler():
	PhysicsAttribute("CollisionHandler")
{

}

CollisionHandler::~CollisionHandler()
{

}

std::vector<HBData> CollisionHandler::separateHitboxes(HitboxNode* i, HitboxNode* j)
{

	sf::Vector2f wp_i = i->getGlobalPos();
	sf::Vector2f wp_j = j->getGlobalPos();

	sf::FloatRect r_i = i->getBB();
	sf::FloatRect r_j = j->getBB();

	sf::Vector2f size_i = { r_i.width, r_i.height };
	sf::Vector2f size_j = { r_j.width, r_j.height };

	sf::Vector2f vel_i = i->getParentObject()->getVelocity();
	sf::Vector2f vel_j = j->getParentObject()->getVelocity();

	// Return values.
	std::vector<HBData> wp_and_vel_re;

	// Special case: If no objects are moving.
	if (vel_i.x == 0 && vel_i.y == 0 && vel_j.x == 0 && vel_j.y == 0)
	{
		if (i->getBehavior() == HBOX::STATIC && j->getBehavior() == HBOX::STATIC)
			return wp_and_vel_re;
		else if (i->getBehavior() == HBOX::STATIC && j->getBehavior() == HBOX::DYNAMIC)
			wp_and_vel_re.push_back(CollisionHandler::singleSeparation(j, i));
		else if (i->getBehavior() == HBOX::DYNAMIC && j->getBehavior() == HBOX::STATIC)
			wp_and_vel_re.push_back(CollisionHandler::singleSeparation(i, j));
		else if (i->getBehavior() == HBOX::DYNAMIC && j->getBehavior() == HBOX::DYNAMIC)
			wp_and_vel_re = CollisionHandler::dualSeparation(i, j);
		return wp_and_vel_re;
	}

	// Check non-rectangle collision.
	if (j->getSB_pts().size() != 0 && j->getBehavior() == HBOX::STATIC)
	{
		wp_and_vel_re = CollisionHandler::nonRectangleSeparation(i, j);
		return wp_and_vel_re;
	}

	sf::Vector2f abs_vel_i = sf::Vector2f(std::abs(vel_i.x), std::abs(vel_i.y));
	sf::Vector2f abs_vel_j = sf::Vector2f(std::abs(vel_j.x), std::abs(vel_j.y));

	// If i is inside j or if j is inside i.
	if (    //i in j.
		(r_j.contains(wp_i) &&												  //Upper left corner.
			r_j.contains(sf::Vector2f(wp_i.x + size_i.x, wp_i.y)) &&			//Upper right corner.
			r_j.contains(sf::Vector2f(wp_i.x, wp_i.y + size_i.y)) &&			//Lower left corner.
			r_j.contains(sf::Vector2f(wp_i.x + size_i.x, wp_i.y + size_i.y)))   //Lower right corner.
		||	//j in i.
		(r_i.contains(wp_j) &&									            //Upper left corner.
			r_i.contains(sf::Vector2f(wp_j.x + size_j.x, wp_j.y)) &&			//Upper right corner.
			r_i.contains(sf::Vector2f(wp_j.x, wp_j.y + size_j.y)) &&			//Lower left corner.
			r_i.contains(sf::Vector2f(wp_j.x + size_j.x, wp_j.y + size_j.y)))   //Lower right corner.
		)
	{
		// If i is moving and j is not.
		if ((abs_vel_i.x != 0 || abs_vel_i.y != 0)
			&& abs_vel_j.x == 0 && abs_vel_j.y == 0)
		{
			wp_and_vel_re.push_back(CollisionHandler::singleSeparation(i, j));
		}
		// If j is moving and i is not.
		else if ((abs_vel_j.x != 0 || abs_vel_j.y != 0)
			&& abs_vel_i.x == 0 && abs_vel_i.y == 0)
		{
			wp_and_vel_re.push_back(CollisionHandler::singleSeparation(j, i));
		}
		// If both are moving.
		else
		{
			wp_and_vel_re = CollisionHandler::dualSeparation(j, i);
		}
	}
	// If no object is encapsulated within the other.
	else
		//if i is moving and j is not.
		if ((abs_vel_i.x != 0 || abs_vel_i.y != 0)
			&& abs_vel_j.x == 0 && abs_vel_j.y == 0)
		{
			wp_and_vel_re.push_back(CollisionHandler::singleSeparation(i, j));
		}
	// If j is moving and i is not.
		else if ((abs_vel_j.x != 0 || abs_vel_j.y != 0)
			&& abs_vel_i.x == 0 && abs_vel_i.y == 0)
		{
			wp_and_vel_re.push_back(CollisionHandler::singleSeparation(j, i));
		}
	// If both are moving.
		else
		{
			wp_and_vel_re = CollisionHandler::dualSeparation(i, j);
		}

	// Return the collected data.
	return wp_and_vel_re;
}

HBData CollisionHandler::singleSeparation(HitboxNode* moving, HitboxNode* still)
{
	// Return value.
	HBData data;
	data.m_this = moving;
	data.m_other = still;
	INTERSECTED_SIDE intersect = INTERSECTED_SIDE::ODATA_NONE;
	sf::Vector2f vel = moving->getParentObject()->getVelocity();
	sf::Vector2f prev_pos = moving->getGlobalPos() - vel;
	sf::Vector2f ul = prev_pos;
	sf::Vector2f ll = prev_pos + sf::Vector2f(0.0f, moving->getBB().height);
	sf::Vector2f ur = prev_pos + sf::Vector2f(moving->getBB().width, 0.0f);
	sf::Vector2f lr = prev_pos + sf::Vector2f(moving->getBB().width, moving->getBB().height);
	sf::Vector2f new_pos = moving->getGlobalPos();
	// Axis-aligned velocity.
	if (vel.x == 0)
	{
		new_pos.y = vel.y < 0 ? still->getGlobalPos().y + still->getBB().height
			: still->getGlobalPos().y - moving->getBB().height;
		intersect = vel.y < 0 ? INTERSECTED_SIDE::ODATA_TOP : INTERSECTED_SIDE::ODATA_BOTTOM;
	}
	else if (vel.y == 0)
	{
		new_pos.x = vel.x < 0 ? still->getGlobalPos().x + still->getBB().width
			: still->getGlobalPos().x - moving->getBB().width;
		intersect = vel.x < 0 ? INTERSECTED_SIDE::ODATA_LEFT : INTERSECTED_SIDE::ODATA_RIGHT;
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
		t_vert = vel.x < 0 ? (still->getGlobalPos().x + still->getBB().width - ll.x) / vel.x	//Right side.
			: (still->getGlobalPos().x - lr.x) / vel.x;										    //Left side.
		t_hori = vel.y < 0 ? (still->getGlobalPos().y + still->getBB().height - ul.y) / vel.y   //Bottom side.
			: (still->getGlobalPos().y - lr.y) / vel.y;									     	//Top side.

		// Calculate intersection points.
		if (vel.x < 0)
		{
			moving_vert[0] = ul;
			moving_vert[1] = ll;
			insec_vert[0] = sf::Vector2f(still->getGlobalPos().x + still->getBB().width, still->getGlobalPos().y);
			insec_vert[1] = still->getGlobalPos() + sf::Vector2f(still->getBB().width, still->getBB().height);
		}
		else if (vel.x > 0)
		{
			moving_vert[0] = ur;
			moving_vert[1] = lr;
			insec_vert[0] = still->getGlobalPos();
			insec_vert[1] = sf::Vector2f(still->getGlobalPos().x, still->getGlobalPos().y + still->getBB().height);
		}
		if (vel.y < 0)
		{
			moving_hori[0] = ul;
			moving_hori[1] = ur;
			insec_hori[0] = sf::Vector2f(still->getGlobalPos().x, still->getGlobalPos().y + still->getBB().height);
			insec_hori[1] = still->getGlobalPos() + sf::Vector2f(still->getBB().width, still->getBB().height);
		}
		else if (vel.y > 0)
		{
			moving_hori[0] = ll;
			moving_hori[1] = lr;
			insec_hori[0] = still->getGlobalPos();
			insec_hori[1] = sf::Vector2f(still->getGlobalPos().x + still->getBB().width, still->getGlobalPos().y);
		}
		// Vertical check + adjustment.
		if ((moving_vert[0].y + t_vert * vel.y > insec_vert[0].y && moving_vert[0].y + t_vert * vel.y < insec_vert[1].y) ||
			(moving_vert[1].y + t_vert * vel.y > insec_vert[0].y && moving_vert[1].y + t_vert * vel.y < insec_vert[1].y))
		{
			new_pos.x = vel.x < 0 ? still->getGlobalPos().x + still->getBB().width
				: still->getGlobalPos().x - moving->getBB().width;
			// Set intersected side.
			intersect = vel.x < 0 ? INTERSECTED_SIDE::ODATA_LEFT : INTERSECTED_SIDE::ODATA_RIGHT;
		}
		// Horizontal check + adjustment.
		else if ((moving_hori[0].x + t_hori * vel.x > insec_hori[0].x && moving_hori[0].x + t_hori * vel.x < insec_hori[1].x) ||
			(moving_hori[1].x + t_hori * vel.x > insec_hori[0].x && moving_hori[1].x + t_hori * vel.x < insec_hori[1].x))
		{
			new_pos.y = vel.y < 0 ? still->getGlobalPos().y + still->getBB().height
				: still->getGlobalPos().y - moving->getBB().height;
			// Set intersected side.
			intersect = vel.y < 0 ? INTERSECTED_SIDE::ODATA_TOP : INTERSECTED_SIDE::ODATA_BOTTOM;

		}
		// If something goes wrong. Do a simple unstuck-check.
		else
		{
			std::vector<sf::Vector2f> unstuck_resolves = CollisionHandler::unstuck(moving->getBB(), still->getBB(), vel);
			new_pos = unstuck_resolves[0];
			// Set intersected side.
			if (new_pos.x - moving->getGlobalPos().x > 0)
				intersect = INTERSECTED_SIDE::ODATA_LEFT;
			else if (new_pos.x - moving->getGlobalPos().x < 0)
				intersect = INTERSECTED_SIDE::ODATA_RIGHT;
			else if (new_pos.y - moving->getGlobalPos().y > 0)
				intersect = INTERSECTED_SIDE::ODATA_TOP;
			else if (new_pos.y - moving->getGlobalPos().y < 0)
				intersect = INTERSECTED_SIDE::ODATA_BOTTOM;
		}
	}
	// Add updated velocities and return ObjectData.
	data.m_this_resolve = new_pos;
	data.m_this_side = intersect;
	return data;
}

std::vector<HBData> CollisionHandler::dualSeparation(HitboxNode* i, HitboxNode* j)
{
	// Return value.
	std::vector<HBData> data;
	HBData i_data;
	HBData j_data;
	i_data.m_this = i;
	i_data.m_other = j;
	j_data.m_this = j;
	j_data.m_other = i;

	// Positions.
	sf::Vector2f ipos = i->getGlobalPos();
	sf::Vector2f jpos = j->getGlobalPos();

	// Velocities.
	sf::Vector2f ivel = i->getParentObject()->getVelocity();
	sf::Vector2f jvel = j->getParentObject()->getVelocity();

	// Previous positions.
	sf::Vector2f i_prev = ipos - ivel;
	sf::Vector2f j_prev = jpos - jvel;

	// X, Y overlaps.
	sf::Vector2f overlaps = CollisionHandler::getOverlaps(i->getBB(), j->getBB());

	// Factors to move i and j by, 0.5 of the overlap for each direction initially.
	float i_c = 0.5;
	float j_c = 0.5;

	// Check if i and j are travelling parallel along x.
	if (CollisionHandler::sameXDirection(i->getParentObject(), j->getParentObject()))
	{
		// Only reposition i.
		if (std::abs(i->getParentObject()->getVelocity().x) > std::abs(j->getParentObject()->getVelocity().x))
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
	if (CollisionHandler::sameYDirection(i->getParentObject(), j->getParentObject()))
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
	sf::Vector2f t = CollisionHandler::getT(i, j, overlaps);

	// If horizontal separation happens before vertical separation.
	if (t.x < t.y)
	{
		if (i_prev.x < j_prev.x)
		{
			ipos.x -= i_c * overlaps.x;
			jpos.x += j_c * overlaps.x;
			// Intersected sides.
			i_data.m_this_side = INTERSECTED_SIDE::ODATA_RIGHT;
			j_data.m_this_side = INTERSECTED_SIDE::ODATA_LEFT;
		}
		else
		{
			ipos.x += i_c * overlaps.x;
			jpos.x -= j_c * overlaps.x;
			// Intersected sides.
			i_data.m_this_side = INTERSECTED_SIDE::ODATA_LEFT;
			j_data.m_this_side = INTERSECTED_SIDE::ODATA_RIGHT;
		}
	}
	// If vertical spparation happens before horizontal separation.
	else
	{
		if (i_prev.y > j_prev.y)
		{
			ipos.y += i_c * overlaps.y;
			jpos.y -= j_c * overlaps.y;
			// Intersected sides.
			i_data.m_this_side = INTERSECTED_SIDE::ODATA_TOP;
			j_data.m_this_side = INTERSECTED_SIDE::ODATA_BOTTOM;
		}
		else
		{
			ipos.y -= i_c * overlaps.y;
			jpos.y += j_c * overlaps.y;
			// Intersected sides.
			i_data.m_this_side = INTERSECTED_SIDE::ODATA_BOTTOM;
			j_data.m_this_side = INTERSECTED_SIDE::ODATA_TOP;
		}
	}
	// Store data in the vector to be returned.
	i_data.m_this_resolve = ipos;
	j_data.m_this_resolve = jpos;
	data.push_back(i_data);
	data.push_back(j_data);
	return data;
}

std::vector<HBData> CollisionHandler::nonRectangleSeparation(HitboxNode* i_hb, HitboxNode* j_hb)
{
	std::vector<HBData> data;
	HBData i_data;
	i_data.m_this = i_hb;
	i_data.m_other = j_hb;

	std::vector<sf::Vector2f> sb_j = j_hb->getSB_pts();
	sf::FloatRect rect_i = i_hb->getBB();
	sf::Vector2f l_mp = { rect_i.left + 0.5f * rect_i.width, rect_i.top + rect_i.height };
	auto ins_test = [sb_j, l_mp](sf::Vector2f p1, sf::Vector2f p2) -> float
	{
		float dx = p2.x - p1.x >= 0.f ? (p2.y - p1.y) / (p2.x - p1.x) : 0.f;
		return (l_mp.y - (p1.y + dx*(l_mp.x - p1.x)));
	};
	// Test each pair of points for intersection.
	for (int i = 1; i < sb_j.size(); i++)
	{
		if (l_mp.x < sb_j[i - 1].x || l_mp.x > sb_j[i].x) continue;
		float diff = ins_test(sb_j[i - 1], sb_j[i]);
		if (diff < 0.f)
			return data;
		else
		{
			i_data.m_this_resolve = i_hb->getGlobalPos() - sf::Vector2f(0.f, diff);
			i_data.m_this_side = INTERSECTED_SIDE::ODATA_BOTTOM;
			data.push_back(i_data);
			break;
		}
	}
	return data;
}

sf::Vector2f CollisionHandler::getOverlaps(sf::FloatRect i_rect, sf::FloatRect j_rect)
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

bool CollisionHandler::sameXDirection(Object* i, Object* j)
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

bool CollisionHandler::sameYDirection(Object* i, Object* j)
{
	sf::Vector2f i_prev = i->getWorldPosition() - i->getVelocity();
	sf::Vector2f j_prev = j->getWorldPosition() - j->getVelocity();
	sf::FloatRect i_prev_rect = { i_prev, i->getSize() };
	sf::FloatRect j_prev_rect = { j_prev, j->getSize() };
	sf::Vector2f overlaps = CollisionHandler::getOverlaps(i_prev_rect, j_prev_rect);
	// Make sure that one of the object is behind the other.
	if (overlaps.x >= overlaps.y)
	{
		return (i->getVelocity().y * j->getVelocity().y > 0);
	}
	return false;
}

sf::Vector2f CollisionHandler::getT(HitboxNode* i, HitboxNode* j, sf::Vector2f overlaps)
{
	sf::Vector2f denom = i->getParentObject()->getVelocity() - j->getParentObject()->getVelocity();
	denom = { std::abs(denom.x), std::abs(denom.y) };
	float t_x = denom.x < 1 ? overlaps.x : overlaps.x / denom.x;
	float t_y = denom.y < 1 ? overlaps.y : overlaps.y / denom.y;
	return { t_x, t_y };
}

/* In case the axis-aligned repositioning in CollisionHandler::singeObjectSeparation() fails, this function is called
* to do a kind of "last resort" resolve of the moving object's position. It tests the distance (no attention
* paid to velocity) from the moving object's current position to each of the sides of the other object's CollisionHandler.
  the side that produces to shortest distane will be the position to which the moving object will be resolved. */
std::vector<sf::Vector2f> CollisionHandler::unstuck(sf::FloatRect moving, sf::FloatRect other, sf::Vector2f moving_vel)
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

	float current_shortest = CollisionHandler_INFINITY;
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
sf::Vector2f CollisionHandler::unstuck(sf::FloatRect moving, sf::FloatRect other)
{
	std::vector<sf::Vector2f> resolves =
	{
		{ moving.left, other.top - moving.height },			// Top.
		{ other.left + other.width, moving.top },			// Right.
		{ moving.left, other.top + other.height},			// Bottom.
		{ other.left - moving.width, moving.top }			// Left.
	};

	float current_shortest = CollisionHandler_INFINITY;
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

std::vector<sf::Vector2f> CollisionHandler::recalibrate(const HBData& odata, const sf::Vector2f& i_pos, const INTERSECTED_SIDE& adj_intersect, const sf::Vector2f& j_pos)
{
	// Quick check to see if the collision is double-sided.
	bool is_double = (adj_intersect != INTERSECTED_SIDE::ODATA_NONE);

	sf::Vector2f i_updated = i_pos;
	sf::Vector2f j_updated = j_pos;
	sf::Vector2f i_size = { odata.m_this->getBB().width, odata.m_this->getBB().height };
	sf::Vector2f j_size = { odata.m_other->getBB().width, odata.m_other->getBB().height };

	float i_c = is_double ? 0.5f : 1.f;
	float j_c = 1.f - i_c;
	sf::Vector2f overlaps = CollisionHandler::getOverlaps({ i_pos, i_size }, { j_pos, j_size });

	switch (odata.m_this_side)
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

std::vector<HBData> CollHandler::calculateResolves(HitboxNode* i, HitboxNode* j)
{
	std::vector<HBData> hbdata;
	if (i == nullptr || j == nullptr) return hbdata;
	if (i->getBehavior() == HBOX::STATIC && j->getBehavior() == HBOX::STATIC) return hbdata;
	// Retrieve hitboxes.
	SubBox2* sb_i = i->getSB2();
	SubBox2* sb_j = j->getSB2();
	if (sb_i == nullptr || sb_j == nullptr) return hbdata;
	// Check for the involved box types and choose that resolving algorithm accordingly.
	
	// RectBox or CircleBox.
	if (sb_i->getType() == sf::String("RectBox") || sb_i->getType() == sf::String("CircleBox"))
	{
		// RectBoxes and Circles are axis-aligned and can be solved using the simple AABB algorithm.
		if (sb_j->getType() == sf::String("RectBox") || sb_j->getType() == sf::String("CircleBox"))
			hbdata = CollHandler::resolveAA(i, j);
		// Collison with a ConvexBox requires the Separating Axis algorithm.
		else if (sb_j->getType() == sf::String("ConvexBox"))
			hbdata = CollHandler::resolveSAT(i, j);
		// If collision with a static Spline hitbox, run a customized algorithm for that situation.
		else if (sb_j->getType() == sf::String("SplineBox"))
			hbdata = CollHandler::resolveSpline(i, j);
	}
	// ConvexBox.
	else if (sb_i->getType() == sf::String("ConvexBox"))
	{
		// Spline hitbox case.
		if (sb_j->getType() == sf::String("SplineBox"))
			hbdata = CollHandler::resolveSpline(i, j);
		// Every other situation requires SAT.
		else hbdata = CollHandler::resolveSAT(i, j);
	}
	// SplineBox.
	else if (sb_i->getType() == sf::String("SplineBox"))
	{
		// Only valid case is a non-SplineBox collision.
		if (sb_j->getType() != sf::String("SplineBox"))
			hbdata = CollHandler::resolveSpline(i, j);
	}
	return hbdata;
}

std::vector<HBData> CollHandler::resolveAA(HitboxNode* i, HitboxNode* j)
{
	std::vector<HBData> hbdata;
	SubBox2* sb_i = i->getSB2();
	SubBox2* sb_j = j->getSB2();
	// Calculate overlaps from the involved Hitbox types.
	sf::Vector2f overlaps = CollHandler::calculateOverlaps(sb_i, sb_j);
	return hbdata;
}

std::vector<HBData> CollHandler::resolveSAT(HitboxNode* i, HitboxNode* j)
{
	return std::vector<HBData>();
}

std::vector<HBData> CollHandler::resolveSpline(HitboxNode* i, HitboxNode* j)
{
	return std::vector<HBData>();
}

sf::Vector2f CollHandler::calculateOverlaps(SubBox2* i, SubBox2* j)
{
	sf::Vector2f ol;
	// Decide which the overlap function to be called depending on the involved Hitbox types.
	if (i->getType() == sf::String("RectBox"))
	{
		if (j->getType() == sf::String("RectBox"))
			ol = CollHandler::ol_RectRect(static_cast<RectBox*>(i), static_cast<RectBox*>(j));
		else if (j->getType() == sf::String("CircleBox"))
			ol = CollHandler::ol_RectCircle(static_cast<RectBox*>(i), static_cast<CircleBox*>(j));
	}
	return ol;
}

sf::Vector2f CollHandler::ol_CircleCircle(CircleBox* i, CircleBox* j)
{
	return sf::Vector2f();
}

sf::Vector2f CollHandler::ol_RectRect(RectBox* i, RectBox* j)
{
	return sf::Vector2f();
}

sf::Vector2f CollHandler::ol_RectCircle(RectBox* i, CircleBox* j)
{
	return sf::Vector2f();
}

sf::Vector2f CollHandler::ol_RectConv(RectBox* i, ConvexBox* j)
{
	return sf::Vector2f();
}

sf::Vector2f CollHandler::ol_ConvConv(ConvexBox* i, ConvexBox* j)
{
	return sf::Vector2f();
}

sf::Vector2f CollHandler::ol_CircleConv(CircleBox* i, ConvexBox* j)
{
	return sf::Vector2f();
}

sf::Vector2f CollHandler::ol_RectSpline(RectBox* i, SplineBox* j)
{
	return sf::Vector2f();
}

sf::Vector2f CollHandler::ol_ConvSpline(ConvexBox* i, SplineBox* j)
{
	return sf::Vector2f();
}

sf::Vector2f CollHandler::ol_CircleSpline(CircleBox* i, SplineBox* j)
{
	return sf::Vector2f();
}
