#pragma once
#include "Object.hpp"


struct Link
{
	Object* object_1;
	Object* object_2;
	float target_dist;

	void apply()
	{
		Object& obj_1 = *object_1;
		Object& obj_2 = *object_2;
		target_dist = std::max(obj_1.radius, obj_2.radius) * 2 + 3;


		const sf::Vector2f axis = obj_1.pos - obj_2.pos;
		const float dist = sqrt(axis.x * axis.x + axis.y * axis.y);
		const sf::Vector2f n = axis / dist;
		const float delta = target_dist - dist;
		obj_1.pos += 0.5f * delta * n;
		obj_2.pos -= 0.5f * delta * n;
	}
};