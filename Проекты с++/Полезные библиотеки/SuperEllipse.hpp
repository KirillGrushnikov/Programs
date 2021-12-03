#include "SFML/Graphics.hpp"

class SupEllipseShape : public sf::Shape
{
private:
	float DN = 2;
	float DM = 2;
	sf::Vector2f m_radius;
	unsigned PointCount = 100;
	
	int sgn(float a) const
	{
		if(a < 0) return -1;
		if (a > 0) return 1;
		return 0;
	}
public:

	explicit SupEllipseShape(const sf::Vector2f& radius = sf::Vector2f(0, 0)) : m_radius(radius)
	{
		update();
	}

	void setRadius(const sf::Vector2f& radius)
	{
		m_radius = radius;
		update();
	}

	void setNM(float N, float M)
	{
		DN = N;
		DM = M;
		update();
	}

	const sf::Vector2f& getRadius() const
	{
		return m_radius;
	}

	virtual unsigned int getPointCount() const
	{
		return PointCount;
	}

	void SetPointCount(unsigned Count)
	{
		PointCount = Count;
	}

	virtual sf::Vector2f getPoint(unsigned int index) const
	{
		static const float pi = 3.141592654f;

		float a = m_radius.x;
		float b = m_radius.y;
		float x, y;
		float angle = index * 2 * pi / getPointCount() - pi / 2;

		float DNa = 2 / DN;
		float DMa = 2 / DM;

		x = pow(abs(cos(angle)), DNa) * a * sgn(cos(angle));
		y = pow(abs(sin(angle)), DMa) * b * sgn(sin(angle));
		return sf::Vector2f(x, y);
	}
};
