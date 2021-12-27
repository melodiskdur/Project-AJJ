#include "Hitbox.h"

Hitbox::Hitbox():
	PhysicsAttribute("Hitbox")
{

}

Hitbox::~Hitbox()
{

}
void Hitbox::separateHitboxes(Object* i, Object* j)
{
	sf::Vector2f wp_i = i->getWorldPosition();
	sf::Vector2f wp_j = j->getWorldPosition();
	sf::Vector2f size_i = i->getSize();
	sf::Vector2f size_j = j->getSize();

	sf::FloatRect r_i = sf::FloatRect(wp_i, size_i);
	sf::FloatRect r_j = sf::FloatRect(wp_j, size_j);

	sf::Vector2f vel_i = i->getVelocity();
	sf::Vector2f vel_j = j->getVelocity();

	// Exit if no objects are moving.
	if (vel_i.x == 0 && vel_i.y == 0 && vel_j.x == 0 && vel_j.y == 0)
		return;

	sf::Vector2f abs_vel_i = sf::Vector2f(std::abs(i->getVelocity().x), std::abs(i->getVelocity().y));
	sf::Vector2f abs_vel_j = sf::Vector2f(std::abs(j->getVelocity().x), std::abs(j->getVelocity().y));

	sf::Vector2f new_wp_i = wp_i;
	sf::Vector2f new_wp_j = wp_j;

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
			Hitbox::singleObjectSeparation(i, j);
		}
		// If j is moving and i is not.
		else if ((abs_vel_j.x != 0 || abs_vel_j.y != 0)
			&& abs_vel_i.x == 0 && abs_vel_i.y == 0)
		{
			Hitbox::singleObjectSeparation(j, i);
		}
		// If both are moving.
		else
		{
			Hitbox::dualObjectSeparation(j, i);
		}
	}
	// If no object is encapsulated within the other.
	else
		//if i is moving and j is not.
		if ((abs_vel_i.x != 0 || abs_vel_i.y != 0)
			&& abs_vel_j.x == 0 && abs_vel_j.y == 0)
		{
			Hitbox::singleObjectSeparation(i, j);
		}
	// If j is moving and i is not.
		else if ((abs_vel_j.x != 0 || abs_vel_j.y != 0)
			&& abs_vel_i.x == 0 && abs_vel_i.y == 0)
		{
			Hitbox::singleObjectSeparation(j, i);
		}
	// If both are moving.
		else
		{
			Hitbox::dualObjectSeparation(i, j);
		}
}

void Hitbox::singleObjectSeparation(Object* moving, Object* other)
{
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
	}
	else if (vel.y == 0)
	{
		new_pos.x = vel.x < 0 ? other->getWorldPosition().x + other->getSize().x
			:					other->getWorldPosition().x - moving->getSize().x;
		new_vel.x = 0;
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
		}
		// Horizontal check + adjustment.
		else if ((moving_hori[0].x + t_hori * vel.x > insec_hori[0].x && moving_hori[0].x + t_hori * vel.x < insec_hori[1].x) ||
			(moving_hori[1].x + t_hori * vel.x > insec_hori[0].x && moving_hori[1].x + t_hori * vel.x < insec_hori[1].x))
		{
			new_pos.y = vel.y < 0 ? other->getWorldPosition().y + other->getSize().y
				: other->getWorldPosition().y - moving->getSize().y;
			new_vel.y = 0;
		}
	}
	moving->setVelocity(new_vel);
	moving->setWorldPosition(new_pos);
}

void Hitbox::dualObjectSeparation(Object* i, Object* j)
{
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
		if (std::abs(i->getVelocity().y) > std::abs(j->getVelocity().y))
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
		}
		else
		{
			ipos.x += i_c * overlaps.x;
			jpos.x -= j_c * overlaps.x;
		}
		ivel.x = 0;
		jvel.x = 0;
	}
	// If vertical spparation happens before horizontal separation.
	else
	{
		if (i_prev.y < j_prev.y)
		{
			ipos.y -= i_c * overlaps.y;
			jpos.y += j_c * overlaps.y;
		}
		else
		{
			ipos.y += i_c * overlaps.y;
			jpos.y -= j_c * overlaps.y;
		}
		ivel.y = 0;
		jvel.y = 0;
	}

	i->setWorldPosition(ipos);
	j->setWorldPosition(jpos);
	i->setVelocity({ivel.x, ivel.y});
	j->setVelocity({jvel.x, jvel.y});
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

bool Hitbox::sameXDirection(Object* i, Object* j)
{
	return (i->getVelocity().x * j->getVelocity().x > 0);
}

bool Hitbox::sameYDirection(Object* i, Object* j)
{
	return (i->getVelocity().y * j->getVelocity().y > 0);
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