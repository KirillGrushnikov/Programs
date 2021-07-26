#include "SFML/Graphics.hpp"
#include <Windows.h>
#include <vector>
#include "SuperEllipse.hpp"
#include <fstream>
#include <sstream>
#include <locale>
#include "Menu.h"

using namespace sf; 

bool fullscreen = false;
int menuNum = 0;
Vector2f poss;
Text text[4];
Text TextSettings[4];
Vector2f ws;
Texture Tbutton[8];
int volume_music = 0;
int volume_effect = 0;
int what_diff = 0;
int buttonFinish = 3;
int buttonStart = 0;
int AudioStart = 0;
const int Audio_count = 2;
RectangleShape GAudio[Audio_count];
RectangleShape s_audio;
std::vector <RectangleShape> selAudio;
RectangleShape planet;
SupEllipseShape selPlanet;
SupEllipseShape music[2];
SupEllipseShape effect[2];
Texture menuTexture1, menuTexture2;
RectangleShape menu1, menu2;
const int countButton = 4;
RectangleShape Buttons[countButton];
Cursor cursor;


Font myfont;

int countMusic = 6;
int musicPlayNow;
bool choose_mars = 0;
bool UserSet = 0;
bool START = 1;
SoundBuffer SoundBuf;
Sound shoot;
Music bgmusic;
int ColorLoading = 0;
RectangleShape Loading;
bool isLoading = 0;

void save()
{
	std::ofstream file;
	file.open("settings.txt", std::ios_base::trunc);
	file << choose_mars << "\n";
	file << volume_music << "\n";
	file << volume_effect << "\n";
	file << what_diff << "\n";
	file.close();
}

void settings()
{
	UserSet = 1;
	menuNum = 1;
	buttonStart = 2;
	AudioStart = 3;
	text[3].setString("Back");
	
	//////////select planet///////////////
	Vector2f b(selPlanet.getPosition() - selPlanet.getRadius());
	Vector2f bs(selPlanet.getRadius().x*2, selPlanet.getRadius().y*2);
	if (poss.x > b.x && poss.x < b.x + bs.x && poss.y > b.y && poss.y < b.y + bs.y)
	{
		selPlanet.setOutlineColor(Color::Red);
		if (GetAsyncKeyState(1) & 0x1)
		{
			choose_mars = !choose_mars;
			switch (choose_mars)
			{
			case false:
				planet.setTexture(&Tbutton[4]);
				break;
			case true:
				planet.setTexture(&Tbutton[5]);
				break;
			}
			save();
		}
	}
	else
		selPlanet.setOutlineColor(Color(0, 241, 255));
	//////////////////////////////////////

	////////////////volume//////////////////
	for (int i = 0; i < 2; i++)
	{
		////////////volume music/////////////////
		b = music[i].getPosition();
		bs = music[i].getRadius();
		if (poss.x > b.x - bs.x && poss.x < b.x + bs.x && poss.y > b.y - bs.y && poss.y < b.y + bs.y)
		{
			music[i].setOutlineColor(Color::Red);
			if (GetAsyncKeyState(1) & 0x1)
			{
				if (i == 0)
				{
					if (volume_music - 1 < 0)
						volume_music = 0;
					else
						volume_music--;
				}
				else
				{
					if (volume_music + 1 > 16) 
						volume_music = 16;
					else
						volume_music++;
					
				}

				for (int j = 0; j < selAudio.size() - 16; j++)
				{
					Color c = selAudio[j].getFillColor();
					if (j <= volume_music)
						c.a = 255;
					else
						c.a = 0;
					selAudio[j].setFillColor(c);
				}
				bgmusic.setVolume(100 / 16 * volume_music);
				save();
			}
		}
		else
			music[i].setOutlineColor(Color(7, 149, 183));
		/////////////////////////////////////////

		////////////volume effect////////////////
		b = effect[i].getPosition();
		bs = effect[i].getRadius();
		if (poss.x > b.x - bs.x && poss.x < b.x + bs.x && poss.y > b.y - bs.y && poss.y < b.y + bs.y)
		{
			effect[i].setOutlineColor(Color::Red);
			if (GetAsyncKeyState(1) & 0x1)
			{
				if (i == 0)
				{
					if (volume_effect - 1 < 0) 
						volume_effect = 0;
					else
						volume_effect--;
				}
				else
				{
					if (volume_effect + 1 > 16) 
						volume_effect = 16;
					else
						volume_effect++;
				}
				for (int j = 16; j < selAudio.size(); j++)
				{
					Color c = selAudio[j].getFillColor();
					if (j - 16 <= volume_effect)
						c.a = 255;
					else
						c.a = 0;
					selAudio[j].setFillColor(c);
				}
				shoot.setVolume(100 / 16 * volume_effect);
				shoot.play();
				save();
			}
		}
		else
			effect[i].setOutlineColor(Color(7, 149, 183));

		/////////////////////////////////////////
	}
}

void back()
{
	menuNum = 0;
	buttonStart = 0;
	AudioStart = 0;
	UserSet = 0;
	text[0].setString("Created NeFFeX");
	text[0].setPosition(ws.x / 2 - 110, 40);

	text[1].setString("Play");
	text[1].setPosition(ws.x / 2 - 20, 150);

	text[2].setString("Settings");
	text[2].setPosition(ws.x / 2 - 30, 350);

	text[3].setString("Exit");
	text[3].setPosition(ws.x / 2 - 20, 550);
}

void Start(RenderWindow& window)
{
	setlocale(LC_ALL, "Russian");
	window.setFramerateLimit(60);

	Image imcur;
	imcur.loadFromFile("Texture\\arrow.png");
	if (cursor.loadFromPixels(imcur.getPixelsPtr(), imcur.getSize(), Vector2u(0, 0)))
		window.setMouseCursor(cursor);

	std::ifstream file;
	file.open("settings.txt");
	file >> choose_mars;
	file >> volume_music;
	file >> volume_effect;
	file >> what_diff;
	file.close();

	myfont.loadFromFile("Font\\rus_eng.ttf");
	ws = Vector2f(window.getSize());

	Loading.setSize(ws);
	Loading.setPosition(0, 0);
	Loading.setFillColor(Color(0, 0, 0, ColorLoading));

	for (int i = 0; i < 4; i++)
	{
		text[i].setFont(myfont);
		text[i].setFillColor(Color(0, 241, 255));
		text[i].setCharacterSize(25);
	}
	text[0].setString("Created NeFFeX");
	text[0].setPosition(window.getSize().x / 2 - 110, 40);
	text[0].setOutlineThickness(1);
	text[0].setOutlineColor(Color::Black);

	text[1].setString("Play");
	text[1].setPosition(window.getSize().x / 2 - 20, 150);

	text[2].setString("Settings");
	text[2].setPosition(window.getSize().x / 2 - 30, 350);

	text[3].setString("Exit");
	text[3].setPosition(window.getSize().x / 2 - 20, 550);

	menuTexture1.loadFromFile("Texture\\menu_background.png");
	menu1.setSize(ws);
	menu1.setTexture(&menuTexture1);

	menuTexture2.loadFromFile("Texture\\setings.png");
	menu2.setSize(ws);
	menu2.setTexture(&menuTexture2);

	Tbutton[0].loadFromFile("Texture\\button1.png");
	Tbutton[1].loadFromFile("Texture\\button2.png");

	Tbutton[2].loadFromFile("Texture\\Audio.png");
	Tbutton[3].loadFromFile("Texture\\Audio1.png");
	Tbutton[4].loadFromFile("Texture\\Earth.png");
	Tbutton[5].loadFromFile("Texture\\Mars2.png");
	Tbutton[6].loadFromFile("Texture\\Audio-.png");
	Tbutton[7].loadFromFile("Texture\\Audio+.png");

	for (int i = 0; i < countButton; i++)
	{
		Buttons[i].setTexture(&Tbutton[0]);
		Buttons[i].setSize(Vector2f(343, 130));
		Buttons[i].setPosition(window.getSize().x / 2 - Buttons[i].getSize().x / 2, 100 + i * 200);
	}
	Buttons[countButton - 1].setFillColor(Color(0, 0, 0, 0));
	Buttons[countButton - 1].setSize(Vector2f(window.getSize()));
	Buttons[countButton - 1].setPosition(0, 0);

	for (int i = 0; i < Audio_count; i++)
	{
		GAudio[i].setTexture(&Tbutton[2]);
		GAudio[i].setSize(Vector2f(Tbutton[2].getSize()));
		GAudio[i].setScale(2, 2);
		GAudio[i].setPosition(120, 100 + i * 100);
	}
	s_audio.setTexture(&Tbutton[3]);
	s_audio.setSize(Vector2f(Tbutton[3].getSize()));

	for (int i = 0; i < Audio_count; i++)
	{
		Vector2f b(GAudio[i].getPosition());
		for (int j = 0; j < 16; j++)
		{
			s_audio.setScale(GAudio[i].getScale());
			s_audio.setPosition(b.x + (3 * GAudio[i].getScale().x) + j * (9 * GAudio[i].getScale().x), b.y + (3 * GAudio[i].getScale().y));
			Color c = s_audio.getFillColor();
			if (j > volume_music && i == 0 || j > volume_effect && i == 1)
				c.a = 0;
			else
				c.a = 255;
			s_audio.setFillColor(c);
			selAudio.push_back(s_audio);
		}
	}
	for (int i = 0; i < 2; i++)
	{
		music[0].setTexture(&Tbutton[6]);
		music[1].setTexture(&Tbutton[7]);
		music[i].setRadius(Vector2f(GAudio[0].getSize().y * GAudio[0].getScale().y / 2, GAudio[0].getSize().y * GAudio[0].getScale().y / 2));
		music[0].setPosition(GAudio[0].getPosition().x - music[i].getRadius().y - 10, GAudio[0].getPosition().y + music[i].getRadius().y);
		music[1].setPosition(GAudio[0].getPosition().x + GAudio[0].getSize().x * GAudio[0].getScale().x + music[i].getRadius().y + 10, GAudio[0].getPosition().y + music[i].getRadius().y);
		music[i].setNM(8, 8);
		music[i].setFillColor(Color::Black);
		music[i].setOutlineColor(Color(7, 149, 183));
		music[i].setOutlineThickness(1);


		effect[0].setTexture(&Tbutton[6]);
		effect[1].setTexture(&Tbutton[7]);
		effect[i].setRadius(Vector2f(GAudio[1].getSize().y * GAudio[1].getScale().y / 2, GAudio[1].getSize().y * GAudio[1].getScale().y / 2));
		effect[0].setPosition(GAudio[1].getPosition().x - effect[i].getRadius().y - 10, GAudio[1].getPosition().y + effect[i].getRadius().y);
		effect[1].setPosition(GAudio[1].getPosition().x + GAudio[1].getSize().x * GAudio[1].getScale().x + effect[i].getRadius().y + 10, GAudio[1].getPosition().y + effect[i].getRadius().y);
		effect[i].setNM(8, 8);
		effect[i].setFillColor(Color::Black);
		effect[i].setOutlineColor(Color(7, 149, 183));
		effect[i].setOutlineThickness(1);
	}


	selPlanet.SetPointCount(500);
	selPlanet.setRadius(Vector2f(125, 125));
	selPlanet.setPosition(ws.x - selPlanet.getRadius().x - 100, 70 + selPlanet.getRadius().y);
	selPlanet.setNM(8, 8);
	selPlanet.setFillColor(Color(0, 9, 32));
	selPlanet.setOutlineColor(Color(0, 241, 255));
	selPlanet.setOutlineThickness(1);

	for (int i = 0; i < 4; i++)
	{
		TextSettings[i].setFont(myfont);
		TextSettings[i].setFillColor(Color(0, 241, 255));
		TextSettings[i].setCharacterSize(15);
		TextSettings[i].setOutlineColor(Color::Black);
		TextSettings[i].setOutlineThickness(1);
	}
	TextSettings[0].setString("Volume Music");
	TextSettings[0].setPosition(GAudio[0].getPosition().x + 90, GAudio[0].getPosition().y - 30);
	TextSettings[1].setString("Volume Effects");
	TextSettings[1].setPosition(GAudio[1].getPosition().x + 90, GAudio[1].getPosition().y - 30);
	TextSettings[2].setString("Select Planet");
	TextSettings[2].setPosition(selPlanet.getPosition().x - 60, selPlanet.getPosition().y - selPlanet.getRadius().y - 30);
	TextSettings[3].setLineSpacing(1.3);
	TextSettings[3].setString(L"Управление: \n f11 - Полный/Оконный \n W - ускорение \n Space - тормоз \n Lкм - выстрел \n Esc - Проигрыш");
	TextSettings[3].setCharacterSize(35);
	TextSettings[3].setOutlineColor(Color::Black);
	TextSettings[3].setOutlineThickness(2);
	TextSettings[3].setPosition(65, 310);


	if (choose_mars)
		planet.setTexture(&Tbutton[5]);
	else
		planet.setTexture(&Tbutton[4]);

	planet.setSize(Vector2f(selPlanet.getRadius().x * 2, selPlanet.getRadius().y * 2));
	planet.setPosition(selPlanet.getPosition().x - selPlanet.getRadius().x, selPlanet.getPosition().y - selPlanet.getRadius().y);

	SoundBuf.loadFromFile("Sound\\lazer2.ogg");
	shoot.setBuffer(SoundBuf);
	shoot.setVolume(100 / 16 * volume_effect);

	if (START)
	{
		START = 0;
		musicPlayNow = rand() % countMusic + 1;
		std::ostringstream MusicNum;
		MusicNum << musicPlayNow;
		bgmusic.openFromFile("Sound\\background_music" + MusicNum.str() + ".ogg");
		bgmusic.setVolume(100 / 16 * volume_music);
		bgmusic.play();
	}
}

void chekMusic()
{
	if (bgmusic.getStatus() == bgmusic.Stopped)
	{
		if (musicPlayNow < countMusic)
			musicPlayNow++;
		else
			musicPlayNow = 1;
		std::ostringstream MusicNum;
		MusicNum << musicPlayNow;
		bgmusic.openFromFile("Sound\\background_music" + MusicNum.str() + ".ogg");
		bgmusic.play();
	}
}

void keyboard(RenderWindow& window)
{
	if (GetAsyncKeyState(122) & 0x1)
	{
		if (!fullscreen)
		{
			fullscreen = true;
			std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
			int windowX = modes[0].width;
			int windowY = modes[0].height;
			window.create(VideoMode(windowX, windowY, modes[0].bitsPerPixel), "Defense of the planet", 8, ContextSettings());
			Start(window);
		}
		else
		{
			fullscreen = false;
			std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
			int windowX = 1280;
			int windowY = 720;
			window.create(VideoMode(windowX, windowY, modes[0].bitsPerPixel), "Defense of the planet", 4, ContextSettings());
			Start(window);
		}
	}

	if (GetAsyncKeyState(27) & 0x1) //esc
		exit(0);
}

void menu(RenderWindow& window)
{
	Start(window);
	bool isMenu = 1;
	isLoading = 0;
	bool clik = 0;

	Clock clock;
	float timer = 0;

	////////////////МЕНЮ///////////////////
	while (isMenu)
	{
		chekMusic();
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				window.close();
				exit(0);
			}
			if (event.type == Event::Resized)
			{
				fullscreen = true;
				std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
				int windowX = modes[0].width;
				int windowY = modes[0].height;
				window.create(VideoMode(windowX, windowY, modes[0].bitsPerPixel), "Defense of the planet", 8, ContextSettings());
				Start(window);
			}

			keyboard(window);
			window.setMouseCursor(cursor);
		}
		Vector2i pixelPos = Mouse::getPosition(window);//забираем коорд курсора
		poss = window.mapPixelToCoords(pixelPos);//переводим их в игровые (уходим от коорд окна)

		if (UserSet)
			settings();

		for (int i = 0; i < countButton; i++)
		{
			Vector2f b(Buttons[i].getPosition());
			Vector2f bs(Buttons[i].getSize());
			if (poss.x > b.x && poss.x < b.x + bs.x && poss.y > b.y && poss.y < b.y + bs.y)
			{
				Buttons[i].setTexture(&Tbutton[1]);
				if (GetAsyncKeyState(1) & 0x1)
				{
					switch (i)
					{
					case 0:
						if (menuNum == 0)
							isLoading = true;
						break;
					case 1:
						if (menuNum == 0)
							settings();
						break;
					case 2:
						if (menuNum == 0)
							exit(0);
						else
							back();
						break;
					case 3:
						break;
					}
				}
			}
			else
				Buttons[i].setTexture(&Tbutton[0]);
		}

		window.clear(Color(129, 181, 221));

		switch (menuNum)
		{
		case 0:
			window.draw(menu1);
			break;
		case 1:
			window.draw(menu2);
			break;
		}

		for (int i = buttonStart; i < countButton; i++)
			window.draw(Buttons[i]);

		if (menuNum == 1)
		{
			for (int i = 0; i < Audio_count; i++)
				window.draw(GAudio[i]);

			for (int i = 0; i < selAudio.size(); i++)
				window.draw(selAudio[i]);

			for (int i = 0; i < 2; i++)
			{
				window.draw(music[i]);
				window.draw(effect[i]);
			}

			window.draw(selPlanet);
			window.draw(planet);

			for (int i = 0; i < 4; i++)
				window.draw(TextSettings[i]);
		}

		for (int i = AudioStart; i < 4; i++)
			window.draw(text[i]);

		if (isLoading)
		{
			float time = clock.getElapsedTime().asSeconds();
			clock.restart();
			timer += time;
			double delay = 0.005;
			if (timer > delay)
			{
				timer = 0;
				ColorLoading += 3;
				Loading.setFillColor(Color(0, 0, 0, ColorLoading));
			}
			window.draw(Loading);
			if (ColorLoading >= 255)
				isMenu = false;
		}

		window.display();
	}
	////////////////////////////////////////////////////
}