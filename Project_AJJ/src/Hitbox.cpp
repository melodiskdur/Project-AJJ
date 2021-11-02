#include "Hitbox.h"

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

	sf::Vector2f abs_vel_i = sf::Vector2f(std::abs(i->getVelocity().x), std::abs(i->getVelocity().y));
	sf::Vector2f abs_vel_j = sf::Vector2f(std::abs(j->getVelocity().x), std::abs(j->getVelocity().y));

	sf::Vector2f new_wp_i = wp_i;
	sf::Vector2f new_wp_j = wp_j;

	//If i is inside j or if j is inside i.
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
		//if i is moving and j is not.
		if ((abs_vel_i.x != 0 || abs_vel_i.y != 0)
			&& abs_vel_j.x == 0 && abs_vel_j.y == 0)
		{
			Hitbox::singleObjectSeparation(i, j);
		}
		//if j is moving and i is not.
		else if ((abs_vel_j.x != 0 || abs_vel_j.y != 0)
			&& abs_vel_i.x == 0 && abs_vel_i.y == 0)
		{
			Hitbox::singleObjectSeparation(j, i);
		}
		//if both are moving.
		else
		{
			Hitbox::dualObjectSeparation(j, i);
		}
	}
	//If no object is encapsulated within the other.
	else
		//if i is moving and j is not.
		if ((abs_vel_i.x != 0 || abs_vel_i.y != 0)
			&& abs_vel_j.x == 0 && abs_vel_j.y == 0)
		{
			Hitbox::singleObjectSeparation(i, j);
		}
	//if j is moving and i is not.
		else if ((abs_vel_j.x != 0 || abs_vel_j.y != 0)
			&& abs_vel_i.x == 0 && abs_vel_i.y == 0)
		{
			Hitbox::singleObjectSeparation(j, i);
		}
	//If both are moving.
		else
		{

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
	else
	{
		float t_vert, t_hori;							//Vertical and horizontal parameters to calculate intersection points in the moving direction.
		sf::Vector2f insec_vert[2];					    //Vertical corners of object Other.
		sf::Vector2f insec_hori[2];					    //Horizontal corners of object Other.
		sf::Vector2f moving_vert[2];					//Vertical corners of object Moving.
		sf::Vector2f moving_hori[2];					//Horizontal corners of object Moving.

		//Set vertical t parameter.
		t_vert = vel.x < 0 ? (other->getWorldPosition().x + other->getSize().x - ll.x) / vel.x	//Right side.
			: (other->getWorldPosition().x - lr.x) / vel.x;										//Left side.
		t_hori = vel.y < 0 ? (other->getWorldPosition().y + other->getSize().y - ll.y) / vel.y  //Bottom side.
			: (other->getWorldPosition().y - lr.y) / vel.y;									 	//Top side.
		//Calculate intersection points.
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
		//Vertical check + adjustment.
		if ((moving_vert[0].y + t_vert * vel.y > insec_vert[0].y && moving_vert[0].y + t_vert * vel.y < insec_vert[1].y) ||
			(moving_vert[1].y + t_vert * vel.y > insec_vert[0].y && moving_vert[1].y + t_vert * vel.y < insec_vert[1].y))
		{
			new_pos.x = vel.x < 0 ? other->getWorldPosition().x + other->getSize().x
				: other->getWorldPosition().x - moving->getSize().x;
			new_vel.x = 0;
		}
		//Horizontal check + adjustment.
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

}