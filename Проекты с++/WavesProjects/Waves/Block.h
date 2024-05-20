#pragma once
#include "SFML/Graphics.hpp"

class Block
{
private:
	sf::RectangleShape sprite;

	sf::Vector2f pos;
	sf::Vector2f vec;
	sf::Vector2f new_vec;
	float mass;

	bool is_fixed;

	bool select;

	sf::Color getColorFromMass() const;
public:
	Block(sf::Vector2f _position = sf::Vector2f(), bool _is_fixed = false, float _mass = 1, sf::Vector2f render_size = sf::Vector2f(5.f, 5.f));
	

	void setPosition(sf::Vector2f _position);
	void setPosition(float x, float y);
	sf::Vector2f getPosition() const;


	void setVelosity(sf::Vector2f _velosity);
	void setVelosity(float x, float y);
	sf::Vector2f getVelosity() const;

	void setNewVelosity(sf::Vector2f _velosity);
	void setNewVelosity(float x, float y);
	sf::Vector2f getNewVelosity() const;


	void setMass(float _mass);
	float getMass() const;

	void setFixed(bool _is_fixed);
	bool isFixed() const;

	void setRenderSize(sf::Vector2f size);
	void setRenderSize(float weight, float height);
	sf::Vector2f getRenderSize() const;


	sf::Color getColor() const;
	void setSelect(bool is_select);

	void update();

	void draw(sf::RenderWindow& window);
};

