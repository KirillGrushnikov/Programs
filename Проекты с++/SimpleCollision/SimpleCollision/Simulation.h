#pragma once
#include <vector>
#include "Object.hpp"
#include "Link.hpp"

class Simulation
{
private:
	std::vector<Object> objects;
	std::vector<Link> links;
	const sf::Vector2f gravity = { 0.0f, 1000.0f };

	sf::CircleShape container;
	const sf::Vector2f cont_pos = { 640.0f, 360.0f };
	const float cont_radius = 300.0f;
	const float sub_steps = 8;
	const bool constraint;
	int color = 0;

	bool pause = true;

	float distant(sf::Vector2f pos_1, sf::Vector2f pos_2)
	{
		return sqrt(pow(pos_1.x - pos_2.x, 2) + pow(pos_1.y - pos_2.y, 2));
	}

	[[nodiscard]]
	float lenght(sf::Vector2f pos)
	{
		return sqrt(pos.x * pos.x + pos.y * pos.y);
	}

	sf::Vector2f norm(sf::Vector2f pos)
	{
		float _lenght = lenght(pos);
		float X = 0, Y = 0;
		if (_lenght != 0) {
			X = pos.x / _lenght;
			Y = pos.y / _lenght;
		}
		return sf::Vector2f(X, Y);
	}

	void updateObjects(float dt)
	{
		for (Object& obj : objects)
			obj.update(dt);
	}

	void applyLinks()
	{
		if (links.empty() || !objects.empty() && links[0].object_1 != &objects[0])
		{
			links.clear();
			for (int i = 1; i < objects.size() && i < 48; i++)
			{
				Link link;
				link.object_1 = &objects[i - 1];
				link.object_2 = &objects[i];

				links.push_back(link);
			}
		}

		for (Link& link : links)
			link.apply();

		if (!links.empty())
		{
			links.begin()->object_1->pos = sf::Vector2f(1180, 500);
			links.back().object_2->pos = sf::Vector2f(100, 500);
		}
	}

	void clearBuffer()
	{
		for (int i = 0; i < objects.size(); i++)
		{
			if (objects[i].pos.x > 1380 || objects[i].pos.x < -100 || objects[i].pos.y < -100 || objects[i].pos.y > 820)
			{
				if(objects.size() > 48)
					objects.erase(objects.begin() + i);
			}
		}
	}

	void applyGravity()
	{
		for(Object& obj : objects)
			obj.accelerate(gravity);
	}

	void applyConstraint()
	{
		for (Object& obj : objects)
		{
			const sf::Vector2f to_obj = cont_pos - obj.pos;
			const float dist = lenght(to_obj);

			if (dist > (cont_radius - obj.radius))
			{
				const sf::Vector2f n = to_obj / dist;
				obj.pos = cont_pos - n * (cont_radius - obj.radius);
			}
		}
	}

	void applyCollisions()
	{
		const float    response_coef = 0.75f;
		const uint64_t objects_count = objects.size();
		// Iterate on all objects
		for (uint64_t i{ 0 }; i < objects_count; ++i) {
			Object& object_1 = objects[i];
			// Iterate on object involved in new collision pairs
			for (uint64_t k{ i + 1 }; k < objects_count; ++k) {
				Object& object_2 = objects[k];
				const sf::Vector2f v = object_1.pos - object_2.pos;
				const float        dist2 = v.x * v.x + v.y * v.y;
				const float        min_dist = object_1.radius + object_2.radius;
				// Check overlapping
				if (dist2 < min_dist * min_dist) {
					const float        dist = sqrt(dist2);
					const sf::Vector2f n = v / dist;
					const float mass_ratio_1 = object_1.radius / (object_1.radius + object_2.radius);
					const float mass_ratio_2 = object_2.radius / (object_1.radius + object_2.radius);
					const float delta = 0.5f * response_coef * (dist - min_dist);
					// Update positions
					object_1.pos -= n * (mass_ratio_2 * delta);
					object_2.pos += n * (mass_ratio_1 * delta);
				}
			}
		}
	}

	float getStepDt(float frame_dt) const
	{
		return frame_dt / static_cast<float>(sub_steps);
	}

	float hueToRgb(float v1, float v2, float vH) {
		if (vH < 0)
			vH += 1;

		if (vH > 1)
			vH -= 1;

		if ((6 * vH) < 1)
			return (v1 + (v2 - v1) * 6 * vH);

		if ((2 * vH) < 1)
			return v2;

		if ((3 * vH) < 2)
			return (v1 + (v2 - v1) * ((2.0f / 3) - vH) * 6);

		return v1;
	}

	sf::Color hslToRgb(int h, float s, float l) {
		unsigned char r = 0;
		unsigned char g = 0;
		unsigned char b = 0;

		if (s == 0)
		{
			r = g = b = (unsigned char)(l * 255);
		}
		else
		{
			float v1, v2;
			float hue = (float)h / 360;

			v2 = (l < 0.5) ? (l * (1 + s)) : ((l + s) - (l * s));
			v1 = 2 * l - v2;

			r = (unsigned char)(255 * hueToRgb(v1, v2, hue + (1.0f / 3)));
			g = (unsigned char)(255 * hueToRgb(v1, v2, hue));
			b = (unsigned char)(255 * hueToRgb(v1, v2, hue - (1.0f / 3)));
		}

		return sf::Color(r, g, b);
	}

public:
	Simulation() : Simulation(true) {}

	Simulation(bool _constraint) : constraint(_constraint)
	{
		if (constraint)
		{
			container.setPointCount(360);
			container.setPosition(cont_pos);
			container.setRadius(cont_radius);
			container.setOrigin(cont_radius, cont_radius);
			container.setOutlineThickness(1);
			container.setOutlineColor(sf::Color(128,128,128));
			container.setFillColor(sf::Color(0, 0, 0, 255));
		}

		for (int i = 0, j = 0; i < 48; i++)
		{
			objects.emplace_back(sf::Vector2f(100 + i * 22, 500 + i * 2), sf::Color(255, 255, 255, 255), 10);
		}
		applyLinks();
	}

	void start_pause()
	{
		pause == false ? pause = true : pause = false;
		std::printf("%d\n", pause);
	}

	void addObject(sf::Vector2f& pos, float radius = 10)
	{
		objects.emplace_back(sf::Vector2f(pos), sf::Color::White, 10.0f);
		objects.back().setVelocity(sf::Vector2f(0, 2), 1);
	}

	void addObjectRGB(sf::Vector2f& pos, float radius = 10)
	{
		if (distant(objects.back().pos, pos) > objects.back().radius + radius + 5)
		{
			objects.emplace_back(sf::Vector2f(pos), hslToRgb(color, 1, 0.5f), radius);
			objects.back().setVelocity(sf::Vector2f(0, 2), 1);
			color++;
			if (color > 360)
				color = 0;
		}
	}

	void clear()
	{
		objects.erase(objects.begin() + 48, objects.end());
	}

	void update(float dt)
	{
		if (!pause)
		{
			float step_dt = getStepDt(dt);
			for (uint32_t i(sub_steps); i--;)
			{
				applyGravity();
				if (constraint)
					applyConstraint();
				applyCollisions();
				updateObjects(step_dt);
				applyLinks();
				clearBuffer();
			}
		}
	}

	void draw(sf::RenderWindow& window)
	{
		if (constraint)
			window.draw(container);
		for (int i = 0; i < objects.size(); i++)
			objects[i].draw(window);
	}
};

