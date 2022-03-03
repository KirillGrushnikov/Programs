#include "SFML/Graphics.hpp"
#include "Menu.h"
#include "SFML/Audio.hpp"
#include "Button.hpp"
#include "LoadSettings.h"
#include "Player.h"

using namespace sf;

void menu(RenderWindow &window)
{
	Image icon;
	icon.loadFromFile("image\\icon.png");
	window.setFramerateLimit(120);
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	Event event;
	Vector2f MousePos;

	Texture TexLoading;
	TexLoading.loadFromFile("image\\Loading.png");
	RectangleShape SpriteLoading;
	SpriteLoading.setPosition(1380, 0);
	SpriteLoading.setSize(Vector2f(window.getSize().x + 200, window.getSize().y));
	SpriteLoading.setTexture(&TexLoading);
	bool IsLoading = false;

	Settings settings("Settings\\Settings.info");

	Clock MainClock, clock;
	float timer = 0;
	double delay = 0.005;

	float TextSpeed = 0.2;
	bool InMenu = true, UnlockSecret = false, InSettings = false;
	bool win[2] = { false, false };
	bool holePlay = false;
	SoundBuffer buffer[3];
	Sound swing, shole, charge;
	float volume = 5;

	buffer[0].loadFromFile("Sound\\swing.ogg");
	buffer[1].loadFromFile("Sound\\hole.ogg");
	buffer[2].loadFromFile("Sound\\charge.ogg");
	swing.setBuffer(buffer[0]);
	swing.setVolume(volume);
	shole.setBuffer(buffer[1]);
	shole.setVolume(volume);
	charge.setBuffer(buffer[2]);
	charge.setVolume(volume);

	Texture TBg;
	TBg.loadFromFile("image\\Bg.png");
	RectangleShape Bg;
	Bg.setTexture(&TBg);
	Bg.setSize(Vector2f(window.getSize()));

	Font font;
	font.loadFromFile("Font\\font.ttf");
	Text Golf[2];
	for (int i = 0; i < 2; i++)
	{
		Golf[i].setFont(font);
		Golf[i].setCharacterSize(200);
		Golf[i].setPosition(640 + i * 130, 85 + i * 180);
		Golf[i].setOrigin(240, 100);
		Golf[0].setString("Pair-");
		Golf[1].setString("Golf");
		Golf[i].setOutlineColor(Color::Black);
		Golf[i].setOutlineThickness(9);
	}
	Button buttons[3];
	for (int i = 0; i < 3; i++)
	{
		buttons[i].SetFont("Font\\font.ttf");
		buttons[i].SetPosition(Vector2f(640, 500 + i * 150));
		buttons[i].SetSize(Vector2f(200, 80));
		buttons[i].SetOriginCenter(buttons[i].GetSize());
		buttons[i].SetFillColor(Color(130, 85, 64), 0);
		buttons[i].SetFillColor(Color(107, 69, 53), 1);
		buttons[i].SetFillColor(Color(86, 58, 42), 2);
		buttons[i].SetNM(buttons[i].GetSize().x/6, buttons[i].GetSize().y/6);
		buttons[i].SetTextCharacterSize(40);
		buttons[i].SetTextPosition(Vector2f(buttons[i].GetPosition().x - buttons[i].GetSize().x / 2, buttons[i].GetPosition().y - buttons[i].GetSize().y / 2));
		buttons[i].SetTextOrigin(Vector2f(70, 25));
		for (int j = 0; j < 3; j++)
		{
			buttons[i].SetOutlineColor(Color(0, 0, 0), j);
			buttons[i].SetOutlineThickness(4, j);
		}
		buttons[i].SetTexture(0);
	}

	buttons[0].SetText("Play");
	buttons[1].SetText("Settings");
	buttons[2].SetText("Exit");


	//CircleShape hole[2], shadowHole[2];
	Player player;

	for (int i = 0; i < 2; i++)
	{
		player.SetId(3);
		player.SetRadius(6);
		player.SetOutlineThickness(6);
		player.SetPosition(Vector2f(Golf[1].getPosition().x + 100, Golf[1].getPosition().y));
	}
	
	while(InMenu)
	{

		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				settings.Save();
				window.close();
				exit(0);
			}

			switch (player.OnCliked(event, MousePos, charge))
			{
			case 1:
				player.UbdatePoint(MousePos);
				break;
			case 2:
				UnlockSecret = true;
				swing.play();
				break;
			}
		}
		if(player.OnCliked(event, MousePos, charge) == 1)
			player.UbdatePoint(MousePos);

		for (int i = 0; i < 3; i++)
			if (buttons[i].ChekClik(MousePos, event))
			{
				switch (i)
				{
				case 0:
					IsLoading = true;
					for (int j = 0; j < 3; j++)
					{
						buttons[j].SetTexture(0);
						buttons[j].SetState(Button::block);
					}
					break;
				case 1:
					InSettings = !InSettings;
					break;
				case 2:
					settings.Save();
					exit(0);
					break;
				default:
					break;
				}
			}

		Vector2i pixelPos = Mouse::getPosition(window);//забираем коорд курсора
		MousePos = window.mapPixelToCoords(pixelPos);//переводим их в игровые (уходим от коорд окна)

		float MainTime = MainClock.getElapsedTime().asSeconds();
		float time = clock.getElapsedTime().asSeconds();
		MainClock.restart();
		timer += MainTime;
		if (timer > delay)
		{
			timer = 0;
			player.Ubdate();

			for (int i = 0; i < 2; i++)
			{
				float a = 3 * sin(2 * time);
				Golf[i].setPosition(Golf[i].getPosition().x, Golf[i].getPosition().y + TextSpeed * sin(2 * time));
				if (!UnlockSecret)
					player.SetPosition(Vector2f(Golf[1].getPosition().x + 50 - 1.9 * a, Golf[1].getPosition().y + 95 + a ));
				Golf[i].setRotation(a);
			}
			for (int i = 0; i < 3; i++)
			{
				buttons[i].SetPosition(Vector2f(buttons[i].GetPosition().x, buttons[i].GetPosition().y + 0.05 * sin(2 * time)));
				buttons[i].SetRotation(3 * sin(2 * time));
				buttons[i].SetTextPosition(Vector2f(buttons[i].GetPosition().x, buttons[i].GetPosition().y + 0.05 * sin(2 * time)));
				buttons[i].SetTextRotation(3 * sin(2 * time));
			}

			if (IsLoading)
			{
				SpriteLoading.setPosition(SpriteLoading.getPosition().x - 15, 0);
				if (SpriteLoading.getPosition().x <= -100)
				{
					SpriteLoading.setPosition(-100, 0);
					InMenu = false;
				}
			}
		}

		window.clear();

		window.draw(Bg);

		player.Draw(window);

		for (int i = 0; i < 2; i++)
			window.draw(Golf[i]);
		for (int i = 0; i < 3; i++)
			buttons[i].Draw(window);

		if (IsLoading)
			window.draw(SpriteLoading);

		window.display();
	}
	settings.Save();
}