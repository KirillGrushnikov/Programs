#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include <string>
#include <vector>
#include "level.h"
#include "Player.h"

float Player::factor = 1;

Player::Player()
{
	shadow.setFillColor(Color(50, 50, 50, 155));
}

void Player::NormVec(Vector2f& a)
{
	Vector2f b = a;
	float d = sqrt(pow(b.x, 2)) + sqrt(pow(b.y, 2));
	a.x /= d;
	a.y /= d;
}

float Player::GetDistant(const Vector2f& a, const Vector2f& b)
{
	return sqrt(pow(abs(a.x - b.x), 2) + pow(abs(a.y - b.y), 2));
}

bool Player::ChekCollision(const FloatRect& rect, const std::string& type)
	{
		if (type == "solid")
		{
			if (pos.x > rect.left - realRadius && pos.x < rect.left + 7 && pos.y > rect.top && pos.y < rect.top + rect.height) // left
			{
				pos.x = rect.left - realRadius; vec.x = -vec.x;
				return true;
			}
			if (pos.x < rect.left + rect.width + realRadius && pos.x > rect.left + rect.width - 7 && pos.y > rect.top && pos.y < rect.top + rect.height) // right
			{
				pos.x = rect.left + rect.width + realRadius; vec.x = -vec.x;
				return true;
			}
			if (pos.y > rect.top - realRadius && pos.y < rect.top + 7 && pos.x > rect.left && pos.x < rect.left + rect.width) // top
			{
				pos.y = rect.top - realRadius; vec.y = -vec.y;
				return true;
			}
			if (pos.y < rect.top + rect.height + realRadius && pos.y > rect.top + rect.height - 7 && pos.x > rect.left && pos.x < rect.left + rect.width) // bottom
			{
				pos.y = rect.top + rect.height + realRadius; vec.y = -vec.y;
				return true;
			}
		}
		else
			return rect.intersects(FloatRect(pos, Vector2f(realRadius * 2, realRadius * 2)));
		return false;
	}

bool Player::Collision(float dx, float dy)
	{
		bool col = false;
		if (pos.y < realRadius) { pos.y = realRadius; vec.y = -vec.y; return true; }
		if (pos.y > 960 - realRadius) { pos.y = 960 - realRadius; vec.y = -vec.y; return true; }
		if (pos.x < realRadius) { pos.x = realRadius; vec.x = -vec.x; return true; }
		if (pos.x > 1280 - realRadius) { pos.x = 1280 - realRadius; vec.x = -vec.x; return true; }
		if (id == 0 && pos.x > 640 - realRadius && pos.x < 670) { pos.x = 640 - realRadius; vec.x = -vec.x; return true; }
		if (id == 1 && pos.x < 640 + realRadius && pos.x > 610) { pos.x = 640 + realRadius; vec.x = -vec.x; return true; }


		for (int i = 0; i < objects.size(); i++)//проходимся по объектам
		{
			if (ChekCollision(objects[i].rect, objects[i].type))//проверяем пересечение сущьности с объектом
			{
				if (objects[i].type == "solid")//если встретили препятствие
				{
					
				}
			}
		}
		return false;
	}

void Player::LoadObjectsFromLevel(Level& level)
{
	objects = level.GetAllObjects();
}

void Player::SetFactor(float Factor)
{
	if (Factor > 0 || Factor == 0) Factor = 1;
	if (Factor < 0) Factor = -1;
	factor = Factor;
}

void Player::SetId(unsigned int ID)
	{
		id = ID;
	}

void Player::SetColor(const Color& color)
	{
		sprite.setFillColor(color);
		sprite.setOutlineColor(Color::Black);
		shadow.setFillColor(Color(50, 50, 50, 155));
	}

void Player::SetPosition(const Vector2f& Pos)
{
	sprite.setPosition(Pos);
	shadow.setPosition(Pos.x + 1, Pos.y + 4);
	pos = Pos;
}

Vector2f Player::GetPosition()
	{
		return pos;
	}

float Player::CalcSpeed(const Vector2f& a, const Vector2f& b)
	{
		float dX = a.x - b.x;//вектор , колинеарный прямой, которая пересекает спрайт и курсор //dx - pos.x  // dy - pos.y
		float dY = a.y - b.y;//он же, координата y
		float rotation = (atan2(dY, dX));//получаем угол в радианах и переводим его в градусы * 180 / 3.14159265

		Vector2f p;

		for (int i = 0; i < 2; i++)
		{
			point[i] = sprite;
			point[i].setRadius(1);
			point[i].setOrigin(1, 1);
			point[i].setOutlineThickness(4);
		}
		float dist = GetDistant(a, b) / 3;
		if (dist > 50) dist = 50;
		pointDist = dist;
		pointRot = rotation;

		p.x = pos.x + pointDist / 2 * cos(pointRot);
		p.y = pos.y + pointDist / 2 * sin(pointRot);
		point[1].setPosition(p);
		p.x = pos.x + pointDist * cos(pointRot);
		p.y = pos.y + pointDist * sin(pointRot);
		point[0].setPosition(p);

		return GetDistant(p, pos) / 2.5;
	}


Vector2f Player::GetVec()
{
	return Vector2f(vec.x * factor, vec.y * factor);
}

float Player::GetVelocity()
{
	return velocity;
}

void Player::SetVelocity(const Vector2f& Vec, float Speed)
{
	velocity = Speed;
	vec.x = Vec.x;
	vec.y = Vec.y;
	NormVec(vec);
}

void Player::SetVelocity(float speed)
{
	velocity = speed;
}

void Player::SetRadius(float Radius)
	{
		sprite.setRadius(Radius);
		sprite.setOrigin(Radius, Radius);
		radius = Radius;
		realRadius = radius;
		shadow.setRadius(realRadius);
		shadow.setOrigin(realRadius, realRadius);
	}

float Player::GetRadius()
	{
		return radius;
	}

void Player::SetOutlineThickness(float pixel)
	{
		sprite.setOutlineThickness(pixel);
		sprite.setOutlineColor(Color(0, 0, 0));
		realRadius = radius + pixel;
		shadow.setRadius(realRadius);
		shadow.setOrigin(realRadius, realRadius);
	}

Vector3f Player::GetPoint()
{
	return Vector3f(pointDist * factor, pointRot, point[0].getRadius());
}

void Player::UbdatePoint(const Vector2f& MousePos)
{
	float dX = pos.x - MousePos.x;//вектор , колинеарный прямой, которая пересекает спрайт и курсор //dx - pos.x  // dy - pos.y
	float dY = pos.y - MousePos.y;//он же, координата y
	float rotation = (atan2(dY, dX));//получаем угол в радианах и переводим его в градусы * 180 / 3.14159265

	Vector2f p;

	for (int i = 0; i < 2; i++)
	{
		point[i] = sprite;
		point[i].setRadius(1);
		point[i].setOrigin(1, 1);
		point[i].setOutlineThickness(4);
	}
	float dist = GetDistant(MousePos, pos) / 3;
	if (dist > 50) dist = 50;
	pointDist = dist;
	pointRot = rotation;

	p.x = pos.x + pointDist / 2 * cos(pointRot);
	p.y = pos.y + pointDist / 2 * sin(pointRot);
	point[1].setPosition(p);
	p.x = pos.x + pointDist * cos(pointRot);
	p.y = pos.y + pointDist * sin(pointRot);
	point[0].setPosition(p);
}

void Player::SetPoint(const Vector3f& param)
{
	for (int i = 0; i < 2; i++)
	{
		point[i] = sprite;
		point[i].setRadius(param.z);
		point[i].setOrigin(1, 1);
		point[i].setOutlineThickness(4 * param.z);
	}

	Vector2f p;
	p.x = pos.x + param.x * cos(param.y);
	p.y = pos.y + param.x * sin(param.y);
	point[0].setPosition(p);
	p.x = pos.x + param.x / 2 * cos(param.y);
	p.y = pos.y + param.x / 2 * sin(param.y);
	point[1].setPosition(p);
}

int Player::OnCliked(Event& event, const Vector2f& MousePos, Sound& sound)
	{
		if (event.type == Event::MouseButtonPressed && Mouse::isButtonPressed(Mouse::Left) && !focus && velocity == 0)
			if (GetDistant(MousePos, pos) < realRadius)
			{
				focus = true;
				sound.play();
			}

		if (event.type == Event::MouseButtonReleased)
		{
			for (int i = 0; i < 2; i++)
				point[i].setRadius(0);
		}

		if (focus)
		{
			if (event.type == Event::MouseButtonPressed && Mouse::isButtonPressed(Mouse::Right))
			{
				focus = false;
				for (int i = 0; i < 2; i++)
					point[i].setRadius(0);
				return 0;
			}

			CalcSpeed(pos, MousePos);
			if (event.type == Event::MouseButtonReleased)
			{
				if (GetDistant(MousePos, pos) < realRadius) { focus = false; return false; }
				SetVelocity(pos - MousePos, CalcSpeed(pos, MousePos));
				for (int i = 0; i < 2; i++)
					point[i].setRadius(0);
				focus = 0;
				return 2;
			}
			return 1;
		}
		return 0;
	}


bool Player::ChekWin(const Vector2f& hole, float holeRad)
	{
		if (GetDistant(pos, hole) < radius + holeRad)
			return true;
		return false;
	}

bool Player::Win(const Vector2f& hole)
	{
		static float angel = 0;
		angel -= 0.1;
		SetPosition(Vector2f(hole.x + (3 * cos(angel) - 3 * sin(angel)), hole.y + (3 * sin(angel) + 3 * cos(angel))));
		SetRadius(radius - 0.15);
		if (radius < 0)
		{
			radius = 0;
			sprite.setFillColor(Color(0, 0, 0, 0));
			sprite.setOutlineColor(Color(0, 0, 0, 0));
			return true;
		}
		return false;
	}


bool Player::Ubdate()
	{
		bool col = false;
		float dx = velocity * vec.x;
		float dy = velocity * vec.y;
		pos.x += dx;
		if (Collision(dx, 0)) col = true;
		pos.y += dy;
		if (Collision(0, dy)) col = true;
		SetPosition(pos);

		if (velocity > 0)
			velocity -= friction;
		if (velocity < 0) velocity = 0;
		return col;
	}

void Player::Draw(RenderWindow& window)
	{
		for (int i = 0; i < 2; i++)
			window.draw(point[i]);
		window.draw(shadow);
		window.draw(sprite);
	}