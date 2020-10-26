#include "SFML/Graphics.hpp"
#include <vector>
#include <time.h>
#include <Windows.h>
#include <iostream>
using namespace sf;
using namespace std;

int N = 3;
int sch = 4;
float radius = 0.5; //много точек
int rad = 3; // 3 основных точки(радиус)
bool pause = 1;
vector <CircleShape> d;
Event event;
RenderWindow window(VideoMode(700, 700), " ", Style::Default, ContextSettings());

void base()
{
	if (N > 3 || N < 3)
	{
		for (int i = 0; i < N; i++)
		{
			d.push_back(CircleShape());
			d[i].setFillColor(Color::Green);
			d[i].setRadius(rad);
			d[i].setPosition(rand() % 690 + 10, rand() % 690 + 10);
		}
	}
	else
	{
		d.push_back(CircleShape());
		d[0].setFillColor(Color::Green);
		d[0].setRadius(rad);
		d[0].setPosition(350, 70);

		d.push_back(CircleShape());
		d[1].setFillColor(Color::Green);
		d[1].setRadius(rad);
		d[1].setPosition(50, 600);

		d.push_back(CircleShape());
		d[2].setFillColor(Color::Green);
		d[2].setRadius(rad);
		d[2].setPosition(650, 630);
	}
		d.push_back(CircleShape());
		d[N].setFillColor(Color::Red);
		d[N].setRadius(rad);
		d[N].setPosition(500, 300);
}

void Tick()
{
	Vector2i pixelPos = Mouse::getPosition(window);//забираем коорд курсора
	Vector2f pos = window.mapPixelToCoords(pixelPos);//переводим их в игровые (уходим от коорд окна)

	if (GetAsyncKeyState(1) & 0x1 && d.size() == N+1)
	{
		d[N].setPosition(pos.x, pos.y);
	}

	if (GetAsyncKeyState(32) & 0x1)
	{
		if (pause)
			pause = false;
		else
			pause = true;
	}
	if (GetAsyncKeyState(107) & 0x1)
	{
		int random = rand() % N;
		d.push_back(CircleShape());
		d[sch].setFillColor(Color::Red);
		d[sch].setRadius(rad);
		d[sch].setPosition(d[random].getPosition().x + ((d[sch - 1].getPosition().x - d[random].getPosition().x) / (N - 1)), d[random].getPosition().y + ((d[sch - 1].getPosition().y - d[random].getPosition().y) / (N - 1)));
		d[sch - 1].setFillColor(Color::White);
		d[sch - 1].setRadius(radius);
		sch++;
	}
		if (!pause)
		{
			int random = rand() % N;
			d.push_back(CircleShape());
			d[sch].setFillColor(Color::Red);
			d[sch].setRadius(rad);
			d[sch].setPosition(d[random].getPosition().x + ((d[sch - 1].getPosition().x - d[random].getPosition().x) / (N - 1)), d[random].getPosition().y + ((d[sch - 1].getPosition().y - d[random].getPosition().y) / (N - 1)));
			d[sch - 1].setFillColor(Color::White);
			d[sch - 1].setRadius(radius);
			sch++;
		}
}

int main()
{
	srand(time(0));

	base();

	Clock clock;
	float timer = 0;

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}

		float time = clock.getElapsedTime().asMicroseconds(); // .asSeconds()
		clock.restart();
		timer += time;

		if (timer > 0.006) { timer = 0; Tick(); } //0.006
		
		window.clear();
		for (int i = 0; i < sch; i++)
			window.draw(d[i]);
		window.display();
	}
}