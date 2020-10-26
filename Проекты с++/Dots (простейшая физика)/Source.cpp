#include "SFML/Graphics.hpp"
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <math.h>
#include <string.h>

using namespace sf;
using namespace std;

const int N = 10; // 800
const int NN = N-1; //задавать на N-1
const int radius = 25;
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
CircleShape d[N];
RenderWindow window(VideoMode(700, 700), " ", Style::Default, ContextSettings()); //сглаживание в ContextSettings
Event event;

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
		if(event.key.code == Mouse::Right)
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
				float ck = sqrt((posx[II] - (pos.x - radius)) * (posx[II] - (pos.x - radius)) + (posy[II] - (pos.y - radius)) * (posy[II] - (pos.y - radius)));
				posx[II] += 3 * ((pos.x - radius) - posx[II]) / ck;
				posy[II] += 3 * ((pos.y - radius) - posy[II]) / ck;

				posx[II] += Dspeed[II] / 4;
				posy[II] += Dspeed[II] / 4;
			}

				if (!prov) // реализация prov(prov - если 1 шарик взят второй браться не будет)
				{
					if (pos.x >= posx[i] && pos.x <= posx[i] + radius * 2 && pos.y >= posy[i] && pos.y <= posy[i] + radius * 2)
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

					if (posx[i] <= 0 || posx[i] + radius * 2 >= 700)
					{
						Dspeed[i] = -Dspeed[i];
						Dangel[i] = -Dangel[i];
					}
					if (posy[i] <= 0 || posy[i] + radius * 2 >= 700)
					{
						Dangel[i] = -Dangel[i];
					}
					//////////////

					//Столкновения точек
					for (int j = 0; j < N - 1; j++)
					{
						if (i != j)
						{
							lines[i][j] = sqrt(pow((posx[i] + radius) - (posx[j] + radius), 2) + pow((posy[i] + radius) - (posy[j] + radius), 2));
						}
						if (lines[i][j] <= radius * 2 && lines[i][j] != 0)
						{
							if (i == II || j == II) {}
							else
							{
								swap(Dspeed[i], Dspeed[j]);
								swap(Dangel[i], Dangel[j]);
							}
						}
					}
		}
		else
		{
			if(prov)
			Dspeed[II] = IS;
			prov = 0;
			//обычный разлёт

			//posx[i] += Dspeed[i] / 4;
			//posy[i] += Dspeed[i] / 4;

			float bx = cos(Dangel[i] * PI / 180) * Dspeed[i];
			float by = sin(Dangel[i] * PI / 180) * Dspeed[i];
			posx[i] += bx;
			posy[i] += by;

			if (posx[i] <= 0 || posx[i] + radius * 2 >= 700)
			{
				Dspeed[i] = -Dspeed[i];
				Dangel[i] = -Dangel[i];
			}
			if (posy[i] <= 0 || posy[i] + radius * 2 >= 700)
			{
				Dangel[i] = -Dangel[i];
			}
			//////////////

			//Столкновения точек
			for (int j = 0; j < N-1; j++)
			{
				if (i != j)
				{
					lines[i][j] = sqrt(pow((posx[i] + radius) - (posx[j] + radius), 2) + pow((posy[i] + radius) - (posy[j] + radius), 2));
				}
				if (lines[i][j] <= radius * 2 && lines[i][j] != 0)
				{
						swap(Dspeed[i], Dspeed[j]);
						swap(Dangel[i], Dangel[j]);
				}
			}
		}
		d[i].setPosition(posx[i], posy[i]);
	}

}


int main()
{
	
	srand(time(0));
	for (int i = 0; i < N; i++)
	{
		posx[i] = rand() % (700 - (1 + radius * 2));
		posy[i] = rand() % (700 - (1 + radius * 2));

	Again_sx:
		Dspeed[i] = (-2 + rand() % 5);
		if (Dspeed[i] == 0)
	goto Again_sx;

		Dangel[i] = rand() % 360 - 180;
	}

	for (int i = 0; i < N; i++)
	{
		d[i].setRadius(radius);
		d[i].setFillColor(Color(rand()%255, rand() % 255, rand() % 255, 255));
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
		}

		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;

		if (timer > 0.006) { timer = 0; Tick(); }
		window.clear();
		for (int i = 0; i < N; i++)
			window.draw(d[i]);
		window.display();
	}
}