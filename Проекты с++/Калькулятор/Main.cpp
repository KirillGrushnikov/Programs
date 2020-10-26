#include "SFML/Graphics.hpp"
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <math.h>
#include <string.h>
#include <sstream>
#include <Windows.h>
#include <vector>
#include <string>

// класс для рисования элипса(овал)
class EllipseShape : public sf::Shape
{
public:

	explicit EllipseShape(const sf::Vector2f& radius = sf::Vector2f(0, 0)) :
		m_radius(radius)
	{
		update();
	}

	void setRadius(const sf::Vector2f& radius)
	{
		m_radius = radius;
		update();
	}

	const sf::Vector2f& getRadius() const
	{
		return m_radius;
	}

	virtual unsigned int getPointCount() const
	{
		return 30; // здесь фиксировано, но может быть атрибутом класса, если это необходимо
	}

	virtual sf::Vector2f getPoint(unsigned int index) const
	{
		static const float pi = 3.141592654f;

		float angle = index * 2 * pi / getPointCount() - pi / 2;
		float x = std::cos(angle) * m_radius.x;
		float y = std::sin(angle) * m_radius.y;

		return sf::Vector2f(m_radius.x + x, m_radius.y + y);
	}

private:

	sf::Vector2f m_radius;
}; 

using namespace sf;
using namespace std;

const int N = 21; //колличество кнопок
int qualytity = 0;
ostringstream output;
bool minus_click = 0;
bool zero = 1;
bool signs[7];
int braket = 0;
int shift = 26;
string temp;
bool ttt;

// знаки
RectangleShape b[N];
RectangleShape equally[2];
CircleShape dot;
Text plus_minus;

Text sign[9];


// цыфры
Text number[10];

Font Myfont;
Text text;

RenderWindow window(VideoMode(330, 500), "Calculator", Style::Default, ContextSettings()); //сглаживание в ContextSettings
Event event;

//счёт
double number_one;
double number_two;
double result;

//логика
void Tick()
{
	Vector2i pixelPos = Mouse::getPosition(window);//забираем коорд курсора
	Vector2f pos = window.mapPixelToCoords(pixelPos);//переводим их в игровые (уходим от коорд окна)

	// наведение курсора на кнопку
	for (int i = 0; i < N; i++)
	{
		if (pos.x >= b[i].getPosition().x && pos.y >= b[i].getPosition().y && pos.x <= b[i].getPosition().x + 80 && pos.y <= b[i].getPosition().y + 60)
		{
			if (i == 15)
				b[i].setFillColor(Color(66, 170, 255));
			else
				b[i].setFillColor(Color(48, 48, 48));

			if (event.type == Event::MouseButtonPressed && event.key.code == Mouse::Left)
			{
				if (i == 15)
					b[i].setFillColor(Color(117, 197, 255));
				else
					b[i].setFillColor(Color(68, 68, 68));

				if (GetAsyncKeyState(1) & 0x1) // если было нажатие на кнопку
				{

						if (i == 4)
						{
							text.setString("0");
							text.setPosition(303, 70);
							shift = 26;
							zero = 1;
							minus_click = 0;
							for (int i = 0; i < 7; i++)
								signs[i] = 0;
						}
						else
						{
							string srt;
							 // удаление последнего знака
							if (i == 20)
							{
								srt = text.getString();
								if (srt.size() > 1)
								{
									if (srt[srt.size() - 1] == '*' || srt[srt.size() - 1] == '^' || srt[srt.size() - 1] == '+' || srt[srt.size() - 1] == '-' || srt[srt.size() - 1] == '/' || srt[srt.size() - 1] == '.')
									{
										for (int z = 0; z < 6; z++)
											signs[z] = 0;
									}
									if (srt[srt.size() - 1] == '-')
									{
										text.setPosition(text.getPosition().x + shift - 10, 70);
									}
									else
										if (srt[srt.size() - 1] == '/')
										{
											text.setPosition(text.getPosition().x + shift - 8, 70);
										}
										else
											if (srt[srt.size() - 1] == '.')
											{
												text.setPosition(text.getPosition().x + shift - 13, 70);
											}
											else
												text.setPosition(text.getPosition().x + shift, 70);
									srt.erase(srt.size() - 1, 1);
									text.setString(srt);
								}
								else
								{
									text.setString("0");
									text.setPosition(303, 70);
									zero = 1;
								}
							}
							
							if (text.getString().getSize() <= 14) // нельзя ввести больше 15 знаков
							{
								/*
							if (i == 0) // минус
							{
								
								if (!minus_click)
								{
									if (zero)
									{
										minus_click = 1;
										text.setString("(-");
										text.setPosition(text.getPosition().x - (shift - 10), 70);
										srt = text.getString();
									}
									else
										if (braket < 0)
										{
											text.setString(text.getString() + "(-");
											text.setPosition(text.getPosition().x - (shift - 10), 70);
										}
										else
										{
											text.setString("(-" + text.getString());
											text.setPosition(text.getPosition().x - (shift - 10), 70);
										}
								}
								else
								{
									minus_click = 0;
									srt.erase(0, 2);
									text.setString(srt);
									text.setPosition(text.getPosition().x + (shift - 10), 70);
								}
								
							}
							*/
							if (i == 1) // один
							{
								if (zero)
								{
									if (minus_click)
										text.setString("(-1");
									else
										text.setString("1");
									zero = 0;
								}
								else
								{
									text.setString(text.getString() + "1");
									text.setPosition(text.getPosition().x - shift, 70);
									for (int i = 0; i < 5; i++)
										signs[i] = 0;
								}

							}
							if (i == 2) // четыре
							{
								if (zero)
								{
									if (minus_click)
									{
										text.setString("(-4");
									}
									else
										text.setString("4");
									zero = 0;
								}
								else
								{
									text.setString(text.getString() + "4");
									text.setPosition(text.getPosition().x - shift, 70);
									for (int i = 0; i < 5; i++)
										signs[i] = 0;
								}
							}
							if (i == 3) // семь
							{
								if (zero)
								{
									if (minus_click)
										text.setString("(-7");
									else
										text.setString("7");
									zero = 0;
								}
								else
								{
									text.setString(text.getString() + "7");
									text.setPosition(text.getPosition().x - shift, 70);
									for (int i = 0; i < 5; i++)
										signs[i] = 0;
								}
							}
							if (i == 5) // ноль
							{
								if (zero)
								{
									if (minus_click)
										text.setString("(-0");
									else
										text.setString("0");
								}
								else
								{
									text.setString(text.getString() + "0");
									text.setPosition(text.getPosition().x - shift, 70);
									for (int i = 0; i < 5; i++)
										signs[i] = 0;
								}
							}
							if (i == 6) // два
							{
								if (zero)
								{
									if (minus_click)
										text.setString("(-2");
									else
										text.setString("2");
									zero = 0;
								}
								else
								{
									text.setString(text.getString() + "2");
									text.setPosition(text.getPosition().x - shift, 70);
									for (int i = 0; i < 5; i++)
										signs[i] = 0;
								}
							}
							if (i == 7) // пять
							{
								if (zero)
								{
									if (minus_click)
										text.setString("(-5");
									else
										text.setString("5");
									zero = 0;
								}
								else
								{
									text.setString(text.getString() + "5");
									text.setPosition(text.getPosition().x - shift, 70);
									for (int i = 0; i < 5; i++)
										signs[i] = 0;
								}
							}
							if (i == 8) //  восемь
							{
								if (zero)
								{
									if (minus_click)
										text.setString("(-8");
									else
										text.setString("8");
									zero = 0;
								}
								else
								{
									text.setString(text.getString() + "8");
									text.setPosition(text.getPosition().x - shift, 70);
									for (int i = 0; i < 5; i++)
										signs[i] = 0;
								}
							}
							if (i == 11) // три
							{
								if (zero)
								{
									if (minus_click)
										text.setString("(-3");
									else
										text.setString("3");
									zero = 0;
								}
								else
								{
									text.setString(text.getString() + "3");
									text.setPosition(text.getPosition().x - shift, 70);
									for (int i = 0; i < 5; i++)
										signs[i] = 0;
								}
							}
							if (i == 12) // шесть
							{
								if (zero)
								{
									if (minus_click)
										text.setString("(-6");
									else
										text.setString("6");
									zero = 0;
								}
								else
								{
									text.setString(text.getString() + "6");
									text.setPosition(text.getPosition().x - shift, 70);
									for (int i = 0; i < 5; i++)
										signs[i] = 0;
								}
							}
							if (i == 13) // девять
							{
								if (zero)
								{
									if (minus_click)
										text.setString("(-9");
									else
										text.setString("9");
									zero = 0;
								}
								else
								{
									text.setString(text.getString() + "9");
									text.setPosition(text.getPosition().x - shift, 70);
									for (int i = 0; i < 5; i++)
										signs[i] = 0;
								}
							}
							///////////////////////////////////////////////////////////////////////////////////

							if (i == 14) // степень
							{
								if (zero)
								{
									if (minus_click)
										text.setString("(-0^");
									else
										text.setString("0^");
									zero = 0;
									text.setPosition(text.getPosition().x - shift, 70);
									signs[0] = 1;
								}
								else
								{
									if (!signs[0] && !signs[1] && !signs[2] && !signs[3] && !signs[4])
									{
										text.setString(text.getString() + "^");
										text.setPosition(text.getPosition().x - shift, 70);
										signs[0] = 1;
										signs[5] = 0;
									}
								}
							}
							if (i == 16) // плюс
							{
								if (zero)
								{
									if (minus_click)
										text.setString("(-0+");
									else
										text.setString("0+");
									zero = 0;
									text.setPosition(text.getPosition().x - shift, 70);
									signs[1] = 1;
								}
								else
								{
									if (!signs[0] && !signs[1] && !signs[2] && !signs[3] && !signs[4])
									{
										text.setString(text.getString() + "+");
										text.setPosition(text.getPosition().x - shift, 70);
										signs[1] = 1;
										signs[5] = 0;
									}
								}
							}
							if (i == 17) // минус
							{
								if (zero)
								{
									if (minus_click)
										text.setString("(-0-");
									else
										text.setString("0-");
									zero = 0;
									text.setPosition(text.getPosition().x - (shift - 10), 70);
									signs[2] = 1;
								}
								else
								{
									if (!signs[0] && !signs[1] && !signs[2] && !signs[3] && !signs[4])
									{
										text.setString(text.getString() + "-");
										text.setPosition(text.getPosition().x - (shift - 10), 70);
										signs[2] = 1;
										signs[5] = 0;
									}
								}
							}
							if (i == 18) // умножить
							{
								if (zero)
								{
									if (minus_click)
										text.setString("(-0*");
									else
										text.setString("0*");
									zero = 0;
									text.setPosition(text.getPosition().x - shift, 70);
									signs[3] = 1;
								}
								else
								{
									if (!signs[0] && !signs[1] && !signs[2] && !signs[3] && !signs[4])
									{
										text.setString(text.getString() + "*");
										text.setPosition(text.getPosition().x - shift, 70);
										signs[3] = 1;
										signs[5] = 0;
									}
								}
							}
							if (i == 19) // делить
							{
								if (zero)
								{
									if (minus_click)
										text.setString("(-0/");
									else
										text.setString("0/");
									zero = 0;
									text.setPosition(text.getPosition().x - (shift - 8), 70);
									signs[4] = 1;
								}
								else
								{
									if (!signs[0] && !signs[1] && !signs[2] && !signs[3] && !signs[4])
									{
										text.setString(text.getString() + "/");
										text.setPosition(text.getPosition().x - (shift - 8), 70);
										signs[4] = 1;
										signs[5] = 0;
									}
								}
							}
							if (i == 10) // точка
							{
								if (zero)
								{
									if (minus_click)
										text.setString("(-0.");
									else
										text.setString("0.");
									zero = 0;
									text.setPosition(text.getPosition().x - (shift - 13), 70);
									signs[5] = 1;
								}
								else
								{
									if (!signs[0] && !signs[1] && !signs[2] && !signs[3] && !signs[4] && !signs[5])
									{
										text.setString(text.getString() + ".");
										text.setPosition(text.getPosition().x - (shift - 13), 70);
										signs[5] = 1;
										for (int j = 0; j < 5; j++)
											signs[j] = 1;
									}
								}
							}
							if (i == 9) // скобки
							{
								if (zero)
								{
									if (minus_click)
										text.setString("(-(");
									else
										text.setString("(");
									braket--;
									zero = 0;
									text.setPosition(text.getPosition().x - (shift - 11), 70);
								}
								else
									if (braket <= 0)
									{
										text.setString(text.getString() + "(");
										text.setPosition(text.getPosition().x - (shift - 11), 70);
										signs[5] = 1;
										braket--;
										for (int j = 0; j < 5; j++)
											signs[j] = 1;
									}
									else
									{
										text.setString(text.getString() + ")");
										text.setPosition(text.getPosition().x - (shift - 11), 70);
										signs[5] = 1;
										braket--;
										for (int j = 0; j < 5; j++)
											signs[j] = 1;
									}
							}
						}
					}
				}
			}
		}
		else
		{
			if (i == 3 || i == 1 || i == 2 || i == 5 || i == 6 || i == 7 || i == 13 || i == 11 || i == 12 || i == 8)
			{
				b[i].setFillColor(Color(28, 28, 28));
			}
			else
			{
				if (i == 15)
					b[i].setFillColor(Color(0, 140, 240));
				else
					b[i].setFillColor(Color(38, 38, 38));
			}
		}
		temp = text.getString();
		if (temp.size() >= 13)
		{
			if (!ttt)
			{
				ttt = 1;
				text.setCharacterSize(36);
				text.setPosition(text.getPosition().x + 63, 70);
				shift = 21;
			}
		}
		else
		{
			ttt = 0;
			text.setCharacterSize(46);
		}
	}
}

//отрисовка кнопок
void _interface()
{
	// отрисовка кнопок
	int sch = 0; // счётчик кнопок(20)
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			b[sch].setSize(Vector2f(80.f, 60.f));
			b[sch].setPosition(2 + i * 82, 438 - j * 62);
			if (sch == 3 || sch == 1 || sch == 2 || sch == 5 || sch == 6 || sch == 7 || sch == 13 || sch == 11 || sch == 12 || sch == 8)
				b[sch].setFillColor(Color(28, 28, 28));
			else
			{
				if (sch == 15)
					b[sch].setFillColor(Color(0, 140, 240));
				else
					b[sch].setFillColor(Color(38, 38, 38));
			}
			sch++;
		}
	}

	Myfont.loadFromFile("C:\\Users\\12266764\\Desktop\\Программы\\Своё\\Калькулятор\\Debug\\Font\\OpenSans-SemiBold.ttf"); //OpenSans-SemiBold
	// отрисовка знаков

	// отрисовка знаков(общие параметры)
	for (int i = 0; i < 9; i++)
	{
		sign[i].setFont(Myfont);
		sign[i].setCharacterSize(40);
	}

	// отрисовка знака +
	sign[0].setString("+");
	sign[0].setPosition(275, 381);

	// отрисовка знака -
	sign[1].setString("-");
	sign[1].setPosition(281, 319);

	// отрисовка знака +/-
	sign[2].setString("+/-");
	sign[2].setPosition(16, 443);

	// отрисовка знака X
	sign[3].setString("+");
	sign[3].rotate(45);
	sign[3].setPosition(298, 256);

	// отрисовка знака /
	sign[4].setString("/");
	sign[4].rotate(10);
	sign[4].setPosition(285, 194);

	// отрисовка знака ^
	sign[5].setString("^");
	sign[5].setPosition(196, 202);

	// отрисовка знака ()
	sign[6].setString("( )");
	sign[6].setPosition(106, 193);

	// отрисовка знака C
	sign[7].setFillColor(Color(229, 81, 55));
	sign[7].setString("C");
	sign[7].setPosition(30, 194);

	// отрисовка знака .
	dot.setRadius(2);
	dot.setPosition(203, 473);

	// отрисовка знака =
	equally[0].setSize(Vector2f(20, 3));
	equally[0].rotate(180);
	equally[0].setPosition(297, 466);

	equally[1].setSize(Vector2f(20, 3));
	equally[1].rotate(180);
	equally[1].setPosition(297, 473);

	// отрисовка знака <=
	sign[8].setFillColor(Color::White);
	sign[8].setPosition(265, 146);
	sign[8].setString("<=");
	b[20].setPosition(248, 158);
	b[20].setSize(Vector2f(80.f, 30.f));
	b[20].setFillColor(Color(28, 28, 28));

	// отрисовка цифр
	
	// отрисовка цифр(общие параметры)
	for (int i = 0; i < 10; i++)
	{
		number[i].setFont(Myfont);
		number[i].setCharacterSize(45);
		number[i].setFillColor(Color::White);
		ostringstream num;
		num << i;
		number[i].setString(num.str());
	}


	//отрисовка цифры 0
	number[0].setPosition(113, 440);

	//отрисовка цифры 1
	number[1].setPosition(31, 377);

	//отрисовка цифры 2
	number[2].setPosition(113, 377);

	//отрисовка цифры 3
	number[3].setPosition(195, 377);

	//отрисовка цифры 4
	number[4].setPosition(31, 315);

	//отрисовка цифры 5
	number[5].setPosition(113, 315);

	//отрисовка цифры 6
	number[6].setPosition(195, 315);

	//отрисовка цифры 7
	number[7].setPosition(31, 253);

	//отрисовка цифры 8
	number[8].setPosition(113, 253);

	//отрисовка цифры 9
	number[9].setPosition(195, 253);

		text.setFont(Myfont);
		text.setString("0");
		text.setPosition(303, 70);
		text.setCharacterSize(46);
		text.setFillColor(Color::White);

}



int main()
{
	float timer = 0;
	Clock clock;

	_interface(); //отрисовка интерфейса

	while(window.isOpen())
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
			window.draw(b[i]);

		window.draw(dot);
		for (int i = 0; i < 2; i++)
			window.draw(equally[i]);

		for (int i = 0; i < 9; i++)
			window.draw(sign[i]);

		for (int i = 0; i < 10; i++)
		window.draw(number[i]);

		window.draw(text);

		window.display();
	}
}