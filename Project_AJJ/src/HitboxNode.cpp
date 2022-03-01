#include "HitboxNode.h"

HitboxNode::HitboxNode() : PropertyNode() { this->property_name = sf::String("Hitbox"); };

HitboxNode::~HitboxNode() { }

sf::Vector2f HitboxNode::getLocalPos()
{
	if (this->parent_object == nullptr) return {0.f, 0.f};
	sf::Vector2f o_sz = this->parent_object->getSize();
	return sf::Vector2f(this->bb_pos.x * o_sz.x, this->bb_pos.y * o_sz.y);
}

sf::Vector2f HitboxNode::getGlobalPos()
{
	if (this->parent_object == nullptr) return { 0.f, 0.f };
	// Retreive the local position and add the parent's world position to it.
	sf::Vector2f loc_pos = this->getLocalPos();
	sf::Vector2f o_wp = this->parent_object->getWorldPosition();
	return sf::Vector2f(loc_pos.x + o_wp.x, loc_pos.y + o_wp.y);

}

sf::FloatRect HitboxNode::getBB()
{
	sf::Vector2f o_pos = this->parent_object->getWorldPosition();
	sf::Vector2f o_sz = this->parent_object->getSize();
	return sf::FloatRect(this->getGlobalPos(), { this->bb_size.x * o_sz.x, this->bb_size.y * o_sz.y} );
}

void HitboxNode::setHitboxTotal(sf::Vector2f hb_center, sf::Vector2f hb_size)
{
	if (this->parent_object == nullptr) return;
	// Find the top left corner of the given rectangle input and store
	// everything as ratios.
	sf::Vector2f topleft = hb_center - 0.5f * hb_size;
	this->setHitboxTotal(sf::FloatRect(topleft, hb_size));
}

void HitboxNode::setHitboxTotal(sf::FloatRect hb)
{
	if (this->parent_object == nullptr) return;
	this->bb_size = { hb.width / this->parent_object->getSize().x, hb.height / this->parent_object->getSize().y };
	this->bb_pos = { hb.left / this->parent_object->getSize().x, hb.top / this->parent_object->getSize().y };
}

void HitboxNode::setHitboxRatio(sf::Vector2f hb_center, sf::Vector2f hb_size)
{
	if (this->parent_object == nullptr) return;
	sf::Vector2f topleft_r = hb_center - 0.5f * hb_size;
	this->setHitboxRatio(sf::FloatRect(topleft_r, hb_size));
}

void HitboxNode::setHitboxRatio(sf::FloatRect hb_ratios)
{
	if (this->parent_object == nullptr) return;
	this->bb_size = { hb_ratios.width, hb_ratios.height };
	this->bb_pos = { hb_ratios.left, hb_ratios.top };
}

void HitboxNode::updateObjectPos(sf::Vector2f hb_wp)
{
	sf::Vector2f o_wp = hb_wp - this->getLocalPos();
	this->parent_object->setWorldPosition(o_wp);
}

sf::VertexArray HitboxNode::getDrawable()
{
	sf::VertexArray bb_drawable(sf::LineStrip, 5);
	sf::FloatRect bb = this->getBB();
	bb_drawable[0].position = { bb.left, bb.top };
	bb_drawable[1].position = { bb.left + bb.width, bb.top };
	bb_drawable[2].position = { bb.left + bb.width, bb.top + bb.height };
	bb_drawable[3].position = { bb.left, bb.top + bb.height };
	bb_drawable[4].position = { bb.left, bb.top };
	sf::Color gc = sf::Color::Cyan;
	bb_drawable[0].color = gc;
	bb_drawable[1].color = gc;
	bb_drawable[2].color = gc;
	bb_drawable[3].color = gc;
	bb_drawable[4].color = gc;
	return bb_drawable;

	
}
