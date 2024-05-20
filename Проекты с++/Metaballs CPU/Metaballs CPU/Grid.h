#pragma once
#include "SFML/Graphics.hpp"
#include <vector>
#include <iostream>
#include <cmath>

struct Line
{
	sf::VertexArray line;

	Line()
	{
		sf::Vertex v;
		line.resize(2);
		line[0].color = sf::Color::Red;
		line[1].color = sf::Color::Red;
	}

	void setPosition(sf::Vector2f pos, sf::Vector2f pos2)
	{
		std::vector<sf::Vector2f> points;

		points.push_back(pos);
		points.push_back(pos2);

		for (int i = 0; i < 2; i++)
			line[i].position = points[i];
	}

	void setColor(sf::Color col, sf::Color col2)
	{
		line[0].color = col;
		line[1].color = col2;
	}

	void draw(sf::RenderWindow& window)
	{
		line.setPrimitiveType(sf::PrimitiveType::Lines);
		window.draw(line);
	}
};


struct Point
{
	bool is_active = false;
	float value = 0;
	sf::CircleShape sprite;

	void draw(sf::RenderWindow& window)
	{
		window.draw(sprite);
	}
};

class Grid
{
private:
	bool smooth = true;
	std::vector<Point> points;
	std::vector<int> index_active_point;

	std::vector<Line> lines;

	sf::Vector2i grid_size;
	float cell_size;


	float dist(sf::Vector2f pos, sf::Vector2f pos2)
	{
		return sqrt(pow(pos2.x - pos.x, 2) + pow(pos2.y - pos.y, 2));
	}

	float lerp(float a, float b, float amt)
	{
		return a + amt * (b - a);
	}
public:

	Grid(sf::Vector2i grid_size, float cell_size, bool smooth = true)
	{
		this->smooth = smooth;
		this->grid_size = grid_size;
		this->cell_size = cell_size;

		sf::Vector2i count_cells;
		count_cells.x = (grid_size.x / cell_size) + 1;
		count_cells.y = (grid_size.y / cell_size) + 1;


		for (int i = 0; i < count_cells.y; i++)
			for (int j = 0; j < count_cells.x; j++)
			{
				Point point;
				point.sprite.setPosition(j * cell_size, i * cell_size);
				point.sprite.setRadius(2);
				point.sprite.setOrigin(2, 2);
				point.sprite.setFillColor(sf::Color(120, 120, 120));

				points.push_back(point);
			}
	}

	void life(std::vector<sf::CircleShape>& circles)
	{
		index_active_point.clear();
		for (int i = 0; i < circles.size(); i++)
		{
			for (int j = 0; j < points.size(); j++)
			{

				float r = circles[i].getRadius();
				sf::Vector2f p = circles[i].getPosition();

				sf::Vector2f pos = points[j].sprite.getPosition();

				points[j].value += pow(r, 2) / (pow(p.x - pos.x, 2) + pow(p.y - pos.y, 2));

				if (points[j].value >= 1)
				{
					points[j].is_active = true;
				}
			}
		}
	}

	void update()
	{
		lines.clear();
		for (int i = 0; i < points.size(); i++)
		{
			int count_cells_x = (grid_size.x / cell_size) + 1;
			std::vector<int> indexs;
			indexs.push_back(i);
			indexs.push_back(i - 1);
			indexs.push_back(i - count_cells_x);
			indexs.push_back(i - count_cells_x - 1);

			int sch = 0;
			std::vector<int> p;
			for (int j = 0; j < 4; j++)
			{
				if (indexs[j] < 0)
				{
					sch++;
					p.push_back(0);
					continue;
				}

				if (points[indexs[j]].is_active)
				{
					sch = 0;
					p.push_back(1);
				}
				else
				{
					sch++;
					p.push_back(0);
				}
			}

			if (sch == 5) return;

			std::vector<float> amts; // 0 - lenght 1 - height 2 - L_up 3 - H_left
			for (int j = 0; j < 4; j++)
				amts.push_back(0.5f);

			if (indexs[0] > 0 && indexs[1] > 0)
				amts[0] = (1 - points[indexs[0]].value) / (points[indexs[1]].value - points[indexs[0]].value);

			if (indexs[0] > 0 && indexs[2] > 0)
				amts[1] = (1 - points[indexs[2]].value) / (points[indexs[0]].value - points[indexs[2]].value);

			if (indexs[2] > 0 && indexs[3] > 0)
				amts[2] = (1 - points[indexs[2]].value) / (points[indexs[3]].value - points[indexs[2]].value);

			if (indexs[3] > 0 && indexs[1] > 0)
				amts[3] = (1 - points[indexs[1]].value) / (points[indexs[3]].value - points[indexs[1]].value);


			sf::Vector2f p_pos = points[i].sprite.getPosition();
			float offset = cell_size / 2;

			
			std::vector<sf::Vector2f> poi;
			poi.push_back(sf::Vector2f(p_pos.x - offset,	p_pos.y));
			poi.push_back(sf::Vector2f(p_pos.x,				p_pos.y - offset));
			poi.push_back(sf::Vector2f(p_pos.x - offset,	p_pos.y - cell_size));
			poi.push_back(sf::Vector2f(p_pos.x - cell_size, p_pos.y - offset));

			int type = 8 * p[3] + 4 * p[2] + 2 * p[0] + p[1];

			if (type == 14) type = 1;
			
			else if (type == 13) type = 2;

			else if (type == 12) type = 3;

			else if (type == 11) type = 4;

			else if (type == 9) type = 6;

			else if (type == 8) type = 7;

			sf::Vector2f pos;
			sf::Vector2f pos2;
			float a;
			Line l;
			switch (type)
			{
			case 1:
				l.setPosition(poi[0], poi[3]);
				if (smooth)
				{
					pos.x = lerp(p_pos.x, p_pos.x - cell_size, amts[0]);
					pos.y = p_pos.y;

					pos2.x = p_pos.x - cell_size;
					pos2.y = lerp(p_pos.y, p_pos.y - cell_size, amts[3]);

					l.setPosition(pos, pos2);
				}
				lines.push_back(l);
				break;
			case 2:
				l.setPosition(poi[0], poi[1]);
				if (smooth)
				{
					pos.x = lerp(p_pos.x, p_pos.x - cell_size, amts[0]);
					pos.y = p_pos.y;

					pos2.x = p_pos.x;
					pos2.y = lerp(p_pos.y - cell_size, p_pos.y, amts[1]);

					l.setPosition(pos, pos2);
				}
				lines.push_back(l);
				break;
			case 3:
				l.setPosition(poi[1], poi[3]);
				if (smooth)
				{
					pos.x = p_pos.x - cell_size;
					pos.y = lerp(p_pos.y, p_pos.y - cell_size, amts[3]);

					pos2.x = p_pos.x;
					pos2.y = lerp(p_pos.y - cell_size, p_pos.y, amts[1]);

					l.setPosition(pos, pos2);
				}
				lines.push_back(l);
				break;
			case 4:
				l.setPosition(poi[1], poi[2]);
				if (smooth)
				{
					pos.x = p_pos.x;
					pos.y = lerp(p_pos.y - cell_size, p_pos.y, amts[1]);

					pos2.x = lerp(p_pos.x, p_pos.x - cell_size, amts[2]);
					pos2.y = p_pos.y - cell_size;

					l.setPosition(pos, pos2);
				}
				lines.push_back(l);
				break;
			case 5:
				l.setPosition(poi[0], poi[1]);
				if (smooth)
				{
					pos.x = lerp(p_pos.x, p_pos.x - cell_size, amts[0]);
					pos.y = p_pos.y;

					pos2.x = p_pos.x;
					pos2.y = lerp(p_pos.y - cell_size, p_pos.y, amts[1]);

					l.setPosition(pos, pos2);
				}
				lines.push_back(l);

				l.setPosition(poi[3], poi[2]);
				if (smooth)
				{
					pos.x = lerp(p_pos.x, p_pos.x - cell_size, amts[2]);
					pos.y = p_pos.y - cell_size;

					pos2.x = p_pos.x - cell_size;
					pos2.y = lerp(p_pos.y, p_pos.y - cell_size, amts[3]);

					l.setPosition(pos, pos2);
				}
				lines.push_back(l);
				break;
			case 6:
				l.setPosition(poi[0], poi[2]);
				if (smooth)
				{
					pos.x = lerp(p_pos.x, p_pos.x - cell_size, amts[0]);
					pos.y = p_pos.y;

					pos2.x = lerp(p_pos.x, p_pos.x - cell_size, amts[2]);
					pos2.y = p_pos.y - cell_size;

					l.setPosition(pos, pos2);
				}
				lines.push_back(l);
				break;
			case 7:
				l.setPosition(poi[3], poi[2]);
				if (smooth)
				{
					pos.x = lerp(p_pos.x, p_pos.x - cell_size, amts[2]);
					pos.y = p_pos.y - cell_size;

					pos2.x = p_pos.x - cell_size;
					pos2.y = lerp(p_pos.y, p_pos.y - cell_size, amts[3]);

					l.setPosition(pos, pos2);
				}
				lines.push_back(l);
				break;
			case 10:
				l.setPosition(poi[1], poi[2]);
				if (smooth)
				{
					pos.x = p_pos.x;
					pos.y = lerp(p_pos.y - cell_size, p_pos.y, amts[1]);

					pos2.x = lerp(p_pos.x, p_pos.x - cell_size, amts[2]);
					pos2.y = p_pos.y - cell_size;

					l.setPosition(pos, pos2);
				}
				lines.push_back(l);

				l.setPosition(poi[0], poi[3]);
				if (smooth)
				{
					pos.x = lerp(p_pos.x, p_pos.x - cell_size, amts[0]);
					pos.y = p_pos.y;

					pos2.x = p_pos.x - cell_size;
					pos2.y = lerp(p_pos.y, p_pos.y - cell_size, amts[3]);

					l.setPosition(pos, pos2);
				}
				lines.push_back(l);
				break;
			}
		}
		for (int i = 0; i < points.size(); i++)
		{
			points[i].is_active = false;
			points[i].value = 0;
		}
	}

	void draw(sf::RenderWindow& window)
	{
		#if defined _DEBUG
		for (int i = 0; i < points.size(); i++)
			points[i].draw(window);
		#endif

		for (int i = 0; i < lines.size(); i++)
			lines[i].draw(window);
	}
};

