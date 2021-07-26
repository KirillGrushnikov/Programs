#include "SFML/Graphics.hpp"
#include <iostream>
#include <Windows.h>
#include <cstdlib>
#include <time.h>
#include <math.h>
#include <vector>
#include <string.h>

using namespace sf;
using namespace std;

const int N = 20; // 800
const int NN = N - 1; //задавать на N-1
//const int radius = 25;
bool clik = false;
const double PI = 3.14159265358979323846;
float posx[N];
float posy[N];
float Dangel[N]; //угол для точек
float Dspeed[N]; // скорость для точек
float lines[N][NN];
float map = 700;
bool prov = 0;
int II;
int IS;
vector <VertexArray> Dlines;
CircleShape d[N];
int Distance = 250;
int radius[N];
RenderWindow window(VideoMode(700, 700), "Net", Style::Default, ContextSettings()); //сглаживание в ContextSettings
Event event;
Text text;
Font myfont;
void keyboard()
{
	text.setString("NULL");
	if (GetAsyncKeyState(49))
	{
		Distance = -1;
		text.setString("-1");
	}

	if (GetAsyncKeyState(50))
	{
		Distance = 150;
		text.setString("150");
	}

	if (GetAsyncKeyState(51))
	{
		Distance = 250;
		text.setString("250");
	}

	if (GetAsyncKeyState(52))
	{
		Distance = 350;
		text.setString("350");
	}

	if (GetAsyncKeyState(53))
	{
		Distance = 400;
		text.setString("400");
	}

	if (GetAsyncKeyState(54))
	{
		Distance = 1000;
		text.setString("1000");
	}

	if (GetAsyncKeyState(27)) //esc выход
		exit(0);
}

void Tick()
{
	Vector2i pixelPos = Mouse::getPosition(window);//забираем коорд курсора
	Vector2f pos = window.mapPixelToCoords(pixelPos);//переводим их в игровые (уходим от коорд окна)

	if (event.type == Event::MouseButtonPressed)
	{
		/*
		if (event.key.code == Mouse::Left)
		{
			if (!clik)
				clik = true;
			else
				clik = false;
		}
		else
		*/
		if (event.key.code == Mouse::Middle)
		{
			clik = false;

			for (int i = 0; i < N; i++)
			{
			Again_xx:
				Dspeed[i] = Dspeed[i] = (-6 + rand() % 13); //Dspeed[i] = (-15 + rand() % 31); 
				if (Dspeed[i] == 0)
					goto Again_xx;
			}
		}
		if (event.key.code == Mouse::Right)
		{
			clik = false;

			for (int i = 0; i < N; i++)
			{
			Again_s:
				Dspeed[i] = (-2 + rand() % 5); //Dspeed[i] = (-15 + rand() % 31); 
				if (Dspeed[i] == 0)
					goto Again_s;
			}
		}
	}

	for (int i = 0; i < N; i++)
	{
		if (Mouse::isButtonPressed(Mouse::Left))
		{
			if (prov) // взять шарик (prov - если 1 шарик взят второй браться не будет)
			{
				float ck = sqrt((posx[II] - (pos.x - radius[II])) * (posx[II] - (pos.x - radius[II])) + (posy[II] - (pos.y - radius[II])) * (posy[II] - (pos.y - radius[II])));
				posx[II] += 3 * ((pos.x - radius[II]) - posx[II]) / ck;
				posy[II] += 3 * ((pos.y - radius[II]) - posy[II]) / ck;

				posx[II] += Dspeed[II] / 4;
				posy[II] += Dspeed[II] / 4;
			}

			if (!prov) // реализация prov(prov - если 1 шарик взят второй браться не будет)
			{
				if (pos.x >= posx[i] && pos.x <= posx[i] + radius[i] * 2 && pos.y >= posy[i] && pos.y <= posy[i] + radius[i] * 2)
				{
					II = i;
					IS = Dspeed[II];
					Dspeed[II] = 0;
					prov = 1;
				}
				else
					II = -1;
			}

			//else
				/*
				// в курсоре черная дыра
				float ck = sqrt((posx[i] - (pos.x - radius)) * (posx[i] - (pos.x - radius)) + (posy[i] - (pos.y - radius)) * (posy[i] - (pos.y - radius)));
				posx[i] += 3 * ((pos.x - radius) - posx[i]) / ck;
				posy[i] += 3 * ((pos.y - radius) - posy[i]) / ck;

				posx[i] += Dspeed[i] / 4;
				posy[i] += Dspeed[i] / 4;
				*/
			float bx = cos(Dangel[i] * PI / 180) * Dspeed[i];
			float by = sin(Dangel[i] * PI / 180) * Dspeed[i];
			posx[i] += bx;
			posy[i] += by;

			if (posx[i] <= 0 || posx[i] + radius[i] * 2 >= 700)
			{
				Dspeed[i] = -Dspeed[i];
				Dangel[i] = -Dangel[i];
			}
			if (posy[i] <= 0 || posy[i] + radius[i] * 2 >= 700)
			{
				Dangel[i] = -Dangel[i];
			}
			//////////////
		}
		else
		{
			if (prov)
				Dspeed[II] = IS;
			prov = 0;
			//обычный разлёт

			//posx[i] += Dspeed[i] / 4;
			//posy[i] += Dspeed[i] / 4;

			float bx = cos(Dangel[i] * PI / 180) * Dspeed[i];
			float by = sin(Dangel[i] * PI / 180) * Dspeed[i];
			posx[i] += bx;
			posy[i] += by;

			if (posx[i] <= 0 || posx[i] + radius[i] * 2 >= 700)
			{
				Dspeed[i] = -Dspeed[i];
				Dangel[i] = -Dangel[i];
			}
			if (posy[i] <= 0 || posy[i] + radius[i] * 2 >= 700)
			{
				Dangel[i] = -Dangel[i];
			}
			//////////////
		}
		d[i].setPosition(posx[i], posy[i]);
	}

	Dlines.clear();
	for (int i = 0; i < N; i++)
		for (int j = i + 1; j < N; j++)
		{
			if (i != j)
			{
				Vector2f Dp = d[i].getPosition();
				Vector2f Dp2 = d[j].getPosition();
				sf::VertexArray line(sf::Lines, 2);
				line[0].position = Vector2f(Dp.x + radius[i], Dp.y + radius[i]);
				line[0].color = Color(220, 157, 0);
				line[1].position = Vector2f(Dp2.x + radius[j], Dp2.y + radius[j]);
				line[1].color = Color(220, 157, 0);
				lines[i][j] = sqrt(pow((Dp.x + radius[i]) - (Dp2.x + radius[j]), 2) + pow((Dp.y + radius[i]) - (Dp2.y + radius[j]), 2));
				if (lines[i][j] <= Distance)
					Dlines.push_back(line);
			}
		}
}

int main()
{
	srand(time(0));
	myfont.loadFromFile("Font\\8277.ttf");
	text.setFont(myfont);

	for (int i = 0; i < N; i++)
	{
		radius[i] = rand() % 20 + 3;
		posx[i] = rand() % (700 - (1 + radius[i] * 2));
		posy[i] = rand() % (700 - (1 + radius[i] * 2));

	Again_sx:
		Dspeed[i] = (-2 + rand() % 5);
		if (Dspeed[i] == 0)
			goto Again_sx;

		Dangel[i] = rand() % 360 - 180;
	}

	for (int i = 0; i < N; i++)
	{
		d[i].setRadius(radius[i]);
		//d[i].setFillColor(Color(rand() % 255, rand() % 255, rand() % 255, 255));
		d[i].setFillColor(Color(220, 157, 0));
		d[i].setPosition(posx[i], posy[i]);
	}
	Clock clock;
	float timer = 0;

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();

			keyboard();
		}

		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;

		if (timer > 0.006) { timer = 0; Tick(); }
		window.clear();
		for (int i = 0; i < Dlines.size(); i++)
			window.draw(Dlines[i]);
		for (int i = 0; i < N; i++)
			window.draw(d[i]);
		if(text.getString() != "NULL")
			window.draw(text);
		window.display();
	}
}