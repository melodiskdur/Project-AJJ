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

std::vector<sf::Vector2f> HitboxNode::getSB_pts()
{
	if (this->sub_boxes.size() == 0) return std::vector<sf::Vector2f>();
	std::vector<sf::Vector2f> sb = this->sub_boxes[0].b_pts;
	sf::FloatRect bb = this->getBB();
	auto translate = [bb](const sf::Vector2f& rel_pos) -> sf::Vector2f
	{ return sf::Vector2f(bb.left + rel_pos.x * bb.width, bb.top + rel_pos.y * bb.height); };
	for (sf::Vector2f& pt : sb) pt = translate(pt);
	return sb;
}

SubBox HitboxNode::getSB()
{
	SubBox sb = this->sub_boxes[0];
	sb.b_pts = this->getSB_pts();
	return sb;
}

SubBox2* HitboxNode::getSB2()
{
	if (this->sb != nullptr)
		this->sb->updateWorldPos(this->parent_object->getWorldPosition(), this->parent_object->getSize());
	return this->sb;
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

bool HitboxNode::createSubBox(std::vector<sf::Vector2f> pts)
{
	if (this->parent_object == nullptr || this->box_behavior != HBOX::STATIC) return false;
	SubBox new_sb;
	new_sb.interpolation_type = HBSPLINE::LINEAR;
	new_sb.b_pts = pts;
	this->sub_boxes.push_back(new_sb);
	return true;
}

void HitboxNode::addSubBox(SubBox2* sb)
{
	if (this->sb == nullptr)
	{
		this->sb = sb;
	}
}

std::vector<sf::VertexArray> HitboxNode::getDrawable()
{
	std::vector<sf::VertexArray> drawables;
	sf::Color gc = sf::Color::Cyan;
	// Bounding box.
	sf::VertexArray bb_drawable(sf::LineStrip, 5);
	sf::FloatRect bb = this->getBB();
	bb_drawable[0].position = { bb.left, bb.top }; 
	bb_drawable[1].position = { bb.left + bb.width, bb.top };
	bb_drawable[2].position = { bb.left + bb.width, bb.top + bb.height };
	bb_drawable[3].position = { bb.left, bb.top + bb.height };
	bb_drawable[4].position = { bb.left, bb.top };
	for (int i = 0; i < 5; i++) bb_drawable[i].color = gc;
	drawables.push_back(bb_drawable);
	// Sub box.
	if (this->sb != nullptr)
	{
		this->sb->updateWorldPos(this->parent_object->getWorldPosition(), this->parent_object->getSize());
		drawables.push_back(this->sb->getDrawable());
	}
	return drawables;
}

RectBox::RectBox()
{
}

RectBox::RectBox(sf::FloatRect rb)
{
}

RectBox::RectBox(sf::Vector2f center, sf::Vector2f size)
{

}

RectBox::~RectBox()
{
}

sf::VertexArray RectBox::getDrawable()
{
	sf::Color sbcc = sf::Color::Green;
	sf::VertexArray sb(sf::LineStrip, 5);
	sb[0].position = { this->global.x, this->global.y };
	sb[1].position = { this->global.x + this->sb_size.x, this->global.y };
	sb[2].position = this->global + this->sb_size;
	sb[3].position = { this->global.x, this->global.y + this->sb_size.y };
	sb[4].position = sb[0].position;
	for (int i = 0; i < 5; i++) sb[i].color = sbcc;
	return sb;
}

std::vector<sf::Vector2f> RectBox::getVertices()
{
	return std::vector<sf::Vector2f>();
}

sf::FloatRect RectBox::getRect()
{
	return sf::FloatRect(this->global, this->sb_size);
}

void RectBox::setLocalCenter(sf::Vector2f c)
{
}

void RectBox::rescale()
{
}

void RectBox::updateWorldPos(sf::Vector2f o_wp, sf::Vector2f o_sz)
{
	this->global = o_wp + sf::Vector2f(o_sz.x * this->sb_rect.left, o_sz.y * this->sb_rect.top);
	this->sb_size = sf::Vector2f(o_sz.x * this->sb_rect.width, o_sz.y * this->sb_rect.height);
}

void CircleBox::setLocalCenter(sf::Vector2f c)
{
}

void CircleBox::rescale()
{
}

void CircleBox::updateWorldPos(sf::Vector2f o_wp, sf::Vector2f o_sz) { this->global = o_wp + sf::Vector2f(o_sz.x * this->center.x, o_sz.y * this->center.y); }

CircleBox::CircleBox()
{
}

CircleBox::CircleBox(sf::Vector2f c, float r)
{
	this->center = c;
	this->radius = r;
}

CircleBox::~CircleBox()
{
}

sf::VertexArray CircleBox::getDrawable()
{
	sf::Color sbcc = sf::Color::Green;
	// Decide number of vertices depending on radius size.
	unsigned int numv = (int)this->radius * 40;
	sf::VertexArray sb(sf::LineStrip, numv + 1);
	float ratio = 2.f * 3.14159f / (float)numv;
	for (int i = 0; i < numv + 1; i++)
	{
		sb[i].position = this->global + this->radius * sf::Vector2f(std::cos(ratio * (float)i), std::sin(ratio * (float)i));
		sb[i].color = sbcc;
	}
	return sb;
}

std::vector<sf::Vector2f> CircleBox::getVertices()
{
	return std::vector<sf::Vector2f>();
}
