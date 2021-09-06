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
}

void Hitbox::singleObjectSeparation(Object* moving, Object* other)
{
	sf::Vector2f vel = moving->getVelocity();
	sf::Vector2f prev_pos = moving->getWorldPosition() - vel;
	sf::Vector2f ul = prev_pos;
	sf::Vector2f ll = prev_pos + sf::Vector2f(0.0f, moving->getSize().y);
	sf::Vector2f ur = prev_pos + sf::Vector2f(moving->getSize().x, 0.0f);
	sf::Vector2f lr = prev_pos + moving->getSize();

	float t_left, t_up, t_right, t_down, x, y;
	sf::Vector2f insec;
	sf::Vector2f new_pos = moving->getWorldPosition();

	//Axis-correction (if v.x or v.y = 0).
	if (vel.x == 0)
	{
		new_pos.y = vel.y < 0 ? other->getWorldPosition().y + other->getSize().y : other->getWorldPosition().y - other->getSize().y;
	}
	else if (vel.y == 0)
	{
		new_pos.x = vel.x < 0 ? other->getWorldPosition().x + other->getSize().x : other->getWorldPosition().x - other->getSize().y;
	}

	//Right-down.
	if (vel.x > 0 && vel.y > 0)
	{
		t_left = (other->getWorldPosition().x - lr.x) / vel.x;
		//Check left.
		y = lr.y + t_left * vel.y;
		if ( (lr.y + t_left * vel.y > other->getWorldPosition().y && lr.y + t_left * vel.y < other->getWorldPosition().y + other->getSize().y) ||
			 (ur.y + t_left * vel.y > other->getWorldPosition().y && ur.y + t_left * vel.y < other->getWorldPosition().y + other->getSize().y))
		{
			new_pos.x = other->getWorldPosition().x - moving->getSize().x;
		}
		//Check up.
		t_up = (other->getWorldPosition().y - lr.y) / vel.y;
		if ((lr.x + t_up * vel.x > other->getWorldPosition().x && lr.x + t_up * vel.x < other->getWorldPosition().x + other->getSize().x) ||
			(ll.x + t_up * vel.x > other->getWorldPosition().x && ll.x + t_up * vel.x < other->getWorldPosition().x + other->getSize().x))
		{
			new_pos.y = other->getWorldPosition().y - moving->getSize().y;
		}
	}
	//Right-up.
	else if (vel.x > 0 && vel.y < 0)
	{
		t_left = (other->getWorldPosition().x - lr.x) / vel.x;
		//Check left.
		if ((lr.y + t_left * vel.y > other->getWorldPosition().y && lr.y + t_left * vel.y < other->getWorldPosition().y + other->getSize().y) ||
			(ur.y + t_left * vel.y > other->getWorldPosition().y && ur.y + t_left * vel.y < other->getWorldPosition().y + other->getSize().y))
		{
			new_pos.x = other->getWorldPosition().x - moving->getSize().x;
		}
		//Check down.
		t_down = (other->getWorldPosition().y + other->getSize().y - ur.y) / vel.y;
		if ((ur.x + t_down * vel.x > other->getWorldPosition().x && ur.x + t_down * vel.x < other->getWorldPosition().x + other->getSize().x) ||
			(ul.x + t_down * vel.x > other->getWorldPosition().x && ul.x + t_down * vel.x < other->getWorldPosition().x + other->getSize().x))
		{
			new_pos.y = other->getWorldPosition().y + other->getSize().y;
		}
	}
	//Left-down.
	else if (vel.x < 0 && vel.y > 0)
	{
		t_right = (other->getWorldPosition().x + other->getSize().x - ll.x) / vel.x;
		//Check right.
		if ((ll.y + t_right * vel.y > other->getWorldPosition().y && ll.y + t_right * vel.y < other->getWorldPosition().y + other->getSize().y) ||
			(ul.y + t_right * vel.y > other->getWorldPosition().y && ul.y + t_right * vel.y < other->getWorldPosition().y + other->getSize().y))
		{
			new_pos.x = other->getWorldPosition().x + moving->getSize().x;
		}
		//Check down.
		t_down = (other->getWorldPosition().y - ll.y) / vel.y;
		if ((lr.x + t_down * vel.x > other->getWorldPosition().x && lr.x + t_down * vel.x < other->getWorldPosition().x + other->getSize().x) ||
			(ll.x + t_down * vel.x > other->getWorldPosition().x && ll.x + t_down * vel.x < other->getWorldPosition().x + other->getSize().x))
		{
			new_pos.y = other->getWorldPosition().y - other->getSize().y;
		}
	}
	//Left-up.
	else if (vel.x < 0 && vel.y < 0)
	{
		t_right= (other->getWorldPosition().x + other->getSize().x - ul.x) / vel.x;
		//Check right.
		if ((ll.y + t_right * vel.y > other->getWorldPosition().y && ll.y + t_right * vel.y < other->getWorldPosition().y + other->getSize().y) ||
			(ul.y + t_right * vel.y > other->getWorldPosition().y && ul.y + t_right * vel.y < other->getWorldPosition().y + other->getSize().y))
		{
			new_pos.x = other->getWorldPosition().x + moving->getSize().x;
		}
		//Check down.
		t_up = (other->getWorldPosition().y + other->getSize().y - ul.y) / vel.y;
		if ((ul.x + t_up * vel.x > other->getWorldPosition().x && ul.x + t_up * vel.x < other->getWorldPosition().x + other->getSize().x) ||
			(ur.x + t_up * vel.x > other->getWorldPosition().x && ul.x + t_up * vel.x < other->getWorldPosition().x + other->getSize().x))
		{
			new_pos.y = other->getWorldPosition().y + other->getSize().y;
		}
	}
	moving->setWorldPosition(new_pos);
}

void Hitbox::dualObjectSeparation(Object* i, Object* j)
{

}