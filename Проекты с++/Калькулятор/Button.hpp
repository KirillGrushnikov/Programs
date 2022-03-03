#pragma once
#include "SFML/Graphics.hpp"
#include <iostream>
#include <locale>
#include "SuperEllipse.hpp"

using namespace sf;

class Button
{
public: enum State { peace, alert, active, block };
protected:
	Vector2f pos;
	Vector2f size;
	SupEllipseShape sprite;
	Texture texture[4];
	Font font;
	Vector2f TextPos;
	Text text;
	unsigned int ButtonState = peace;
	bool clik = false;


	CircleShape dots;
public:
	Button()
	{

	}

	// функция движение
	virtual void move(float x, float y, float speed)
	{
		float dx = x * speed, dy = y * speed;
		SetPosition(Vector2f(pos.x + dx, pos.y + dy));
		SetTextPosition(Vector2f(TextPos.x + dx, TextPos.y + dy));
	}

	void anim(Vector2f MousePos)
	{
		if (ChekMouse(MousePos))
		{
			for (int i = 0; i < 60; i++)
			{
				dots.setFillColor(Color(171, 43, 171));
				dots.setPosition(sprite.getPoint(i));
				dots.setRadius(1);
				dots.setOrigin(1, 1);
			}
		}
	}

	// проверка клика по кнопке
	virtual bool ChekClik(Vector2f MousePos, Event event)
	{
		if (ButtonState == State::block)
			return false;

		if (ChekMouse(MousePos))
		{
			SetTexture(Button::alert);
			if (!clik)
			{
				if (event.type == Event::MouseButtonPressed && event.key.code == Mouse::Left)
				{
					clik = true;
					SetTexture(Button::active);
				}
			}
			else
			{
				if (Mouse::isButtonPressed(Mouse::Left))
					SetTexture(Button::active);

				if (event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left)
				{
					SetTexture(Button::alert);
					clik = false;
					return true;
				}
			}
		}
		else
		{
			SetTexture(Button::peace);
			clik = false;
		}

		return false;
	}

	// проверка навидение на кнопку
	virtual bool ChekMouse(Vector2f MousePos)
	{
		if (MousePos.x > pos.x && MousePos.x < pos.x + size.x && MousePos.y > pos.y && MousePos.y < pos.y + size.y)
			return true;
		else
			return false;
	}

	// установить размер кнопки
	void SetSize(Vector2f Size)
	{
		size = Size;
		sprite.setRadius(Vector2f(-size.x / 2, -size.y / 2));
		sprite.setOrigin(-size.x / 2, -size.y / 2);
	}

	void SetNM(float N, float M)
	{
		sprite.setNM(N, M);
	}

	// получить размер кнопки
	Vector2f GetSize()
	{
		return size;
	}

	// установить позицию кнопки
	void SetPosition(Vector2f position)
	{
		pos = position;
		sprite.setPosition(pos);
	}

	// получить позицию кнопки
	Vector2f GetPosition()
	{
		return pos;
	}

	// загрузить текстуру кнопки
	void LoadTexture(std::string str, unsigned int state)
	{
		texture[state].loadFromFile(str);
	}

	// установить текстуру кнопки
	void SetTexture(unsigned int state)
	{
		ButtonState = state;
		sprite.setTexture(&texture[ButtonState]);
	}

	// установить состояние кнопки
	void SetState(unsigned int state)
	{
		ButtonState = state;
	}

	// получить состояние кнопки
	unsigned int GetState()
	{
		return ButtonState;
	}

	// установить шрифт текста кнопки
	virtual void SetFont(std::string str)
	{
		font.loadFromFile(str);
		text.setFont(font);
	}

	void SetTextRotate(float angel)
	{
		text.setRotation(angel);
	}

	// установить текст кнопки
	void SetText(std::string str)
	{
		text.setString(str);
	}

	void SetText(std::wstring str)
	{
		text.setString(str);
	}

	void SetText(char str)
	{
		text.setString(str);
	}

	// получить текст кнопки
	std::string GetTextString()
	{
		return text.getString();
	}

	// установить позицию текста кнопки
	void SetTextPosition(Vector2f position)
	{
		TextPos = position;
		text.setPosition(TextPos);
	}

	// получить позицию текста кнопки
	Vector2f GetTextPosition()
	{
		return TextPos;
	}

	// установить размер шрифта текста кнопки
	void SetTextCharacterSize(unsigned int size)
	{
		text.setCharacterSize(size);
	}

	// отрисовка кнопки
	virtual void Draw(RenderWindow& window)
	{
		window.draw(sprite);

		window.draw(dots);

		window.draw(text);
	}
};