#include "Block.h"
#include "colorFormat.hpp"

sf::Color Block::getColorFromMass() const
{
	HSL hsl_color = rgbToHsl(RGB(14, 17, 103));

	if(is_fixed)
		hsl_color.l = 0;
	else
		hsl_color.l = 0.7 / 1 - mass / 1000;

	RGB rgb_color = hslToRgb(hsl_color);

	return sf::Color(rgb_color.r, rgb_color.g, rgb_color.b);
}



Block::Block(sf::Vector2f _position, bool _is_fixed, float _mass, sf::Vector2f render_size) :
	pos(_position),
	is_fixed(_is_fixed),
	mass(_mass)
{
	sprite.setSize(render_size);
	sprite.setOrigin(render_size.x / 2, render_size.y / 2);
	sprite.setPosition(pos);
	sprite.setFillColor(getColorFromMass());
	sprite.setOutlineColor(sf::Color::Red);
}



void Block::setPosition(sf::Vector2f _position)
{
	pos = _position;
	sprite.setPosition(pos);
}

void Block::setPosition(float x, float y)
{
	setPosition(sf::Vector2f(x, y));
}

sf::Vector2f Block::getPosition() const
{
	return pos;
}

void Block::setVelosity(sf::Vector2f _velosity)
{
	vec = _velosity;
}

void Block::setVelosity(float x, float y)
{
	setVelosity(sf::Vector2f(x, y));
}

sf::Vector2f Block::getVelosity() const
{
	return vec;
}



void Block::setNewVelosity(sf::Vector2f _velosity)
{
	new_vec = _velosity;
}

void Block::setNewVelosity(float x, float y)
{
	setNewVelosity(sf::Vector2f(x, y));
}

sf::Vector2f Block::getNewVelosity() const
{
	return new_vec;
}



void Block::setMass(float _mass)
{
	mass = _mass;
	sprite.setFillColor(getColorFromMass());
}

float Block::getMass() const
{
	return mass;
}



void Block::setFixed(bool _is_fixed)
{
	is_fixed = _is_fixed;
	sprite.setFillColor(getColorFromMass());
}

bool Block::isFixed() const
{
	return is_fixed;
}



void Block::setRenderSize(sf::Vector2f size)
{
	sprite.setSize(size);
	sprite.setOrigin(size.x / 2, size.y / 2);
}

void Block::setRenderSize(float weight, float height)
{
	setRenderSize(sf::Vector2f(weight, height));
}

sf::Vector2f Block::getRenderSize() const
{
	return sprite.getSize();
}

sf::Color Block::getColor() const
{
	return sprite.getFillColor();
}

void Block::setSelect(bool is_select)
{
	if(!is_select)
		sprite.setOutlineThickness(0);

	if (is_select)
		sprite.setOutlineThickness(1);
}



void Block::update()
{
	if (is_fixed) return;

	vec.x = vec.x + (new_vec.x / mass);
	vec.y = vec.y + (new_vec.y / mass);
	new_vec = sf::Vector2f();

	pos.x += vec.x;
	pos.y += vec.y;

	sprite.setPosition(pos);
}



void Block::draw(sf::RenderWindow& window)
{
	window.draw(sprite);
}
