#pragma once
#include "SFML/Graphics.hpp"

struct Object
{
	sf::CircleShape circle;
	sf::Vector2f pos;
	sf::Vector2f last_pos;
	sf::Vector2f acceleration;
	sf::Color color;
	float radius;

	Object()
	{
		radius = 100;
		color = sf::Color(rand() % 255, rand() % 255, rand() % 255, 255);
		circle.setPointCount(100);
		circle.setRadius(radius);
		circle.setOrigin(radius, radius);
		circle.setPosition(0, 0);
		circle.setFillColor(color);
	}

	Object(sf::Vector2f _pos, sf::Color _color = sf::Color::Green, float _radius = 100) :
		radius(_radius),
		color(_color),
		pos(_pos),
		last_pos(_pos)
	{
		circle.setPointCount(100);
		circle.setRadius(radius);
		circle.setOrigin(radius, radius);
		circle.setPosition(pos.x, pos.y);
		circle.setFillColor(color);
	}

	void accelerate(sf::Vector2f acc)
	{
		acceleration += acc;
	}

	void setVelocity(sf::Vector2f v, float dt)
	{
		last_pos = pos - (v * dt);
	}

	void addVelocity(sf::Vector2f v, float dt)
	{
		last_pos -= v * dt;
	}

	[[nodiscard]]
	sf::Vector2f getVelocity(float dt) const
	{
		return (pos - last_pos) / dt;
	}

	void update(float dt)
	{
		sf::Vector2f velosity = pos - last_pos;
		last_pos = pos;

		pos = pos + velosity + acceleration * (dt * dt);
		acceleration = {};

		circle.setPosition(pos.x, pos.y);
	}

	void draw(sf::RenderWindow& window)
	{
		window.draw(circle);
	}
};