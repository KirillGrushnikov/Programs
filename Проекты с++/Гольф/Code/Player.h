#pragma once
#include "level.h"

using namespace sf;


class Player
{
private:
	CircleShape sprite, shadow, point[2];
	std::vector <Object> objects;

	float pointDist = 0;
	float pointRot = 0;
	static float factor;

	Vector2f pos = {0, 0};
	float radius = 0;
	float realRadius = 0;
	Vector2f vec = {0, 0};
	Vector2f StartVec = { 0, 0 };
	float velocity = 0;
	float friction = 10.0 / 100;
	bool focus = false;
	int id = 0;

	void NormVec(Vector2f& a);

	float GetDistant(const Vector2f& a, const Vector2f& b);

	bool ChekCollision(const FloatRect& rect, const std::string& type);
	bool Collision(float dx, float dy);

public:
	Player();

	void LoadObjectsFromLevel(Level& level);
	
	static void SetFactor(float Factor);

	void SetId(unsigned int ID);

	void SetColor(const Color& color);

	void SetPosition(const Vector2f& Pos);
	Vector2f GetPosition();

	Vector2f GetVec();
	float CalcSpeed(const Vector2f& a, const Vector2f& b);

	void UbdatePoint(const Vector2f& MousePos);
	void SetPoint(const Vector3f& point);
	Vector3f GetPoint();
	void SetVelocity(const Vector2f& Vec, float speed = 0);
	void SetVelocity(float speed = 0);

	float GetVelocity();

	void SetRadius(float Radius);
	float GetRadius();

	void SetOutlineThickness(float pixel);

	int OnCliked(Event& event, const Vector2f& MousePos, Sound& sound);

	bool ChekWin(const Vector2f& hole, float holeRad);
	bool Win(const Vector2f& hole);

	bool Ubdate();
	void Draw(RenderWindow& window);
};