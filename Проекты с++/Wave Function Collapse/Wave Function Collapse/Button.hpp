#pragma once
#include "SFML/Graphics.hpp"
#include "SuperEllipse.hpp"
#include <iostream>
#include <string>
#include <locale>

using namespace sf;

class Lable
{
private:
	Font font;
	Vector2f pos;
	Text text;
public:
	Lable()
	{

	}
	
	// установить текст
	void SetText(std::string str)
	{
		text.setString(str);
	}
	// установить текст
	void SetText(char* str)
	{
		std::string s(str);
		text.setString(s);
	}
	// получить текст
	std::string  GetText()
	{
		return text.getString();
	}

	// добавить к тексту
	void AddText(std::string str)
	{
		text.setString(text.getString() + str);
	}

	// добавить к тексту
	void AddText(int value)
	{
		text.setString(std::to_string(value));
	}
	
	// установить шрифт
	void SetFont(std::string filename)
	{
		font.loadFromFile(filename);
		text.setFont(font);
	}
	// установить цвет
	void SetColor(Color color)
	{
		text.setFillColor(color);
	}
	// установить позицию
	void SetPosition(Vector2f Pos)
	{
		pos = Pos;
		text.setPosition(pos);
	}
	// получить позицию
	Vector2f GetPosition()
	{
		return pos;
	}
	// установить позицию
	void SetPosition(float x, float y)
	{
		pos.x = x;
		pos.y = y;
		text.setPosition(pos);
	}
	// установить размер символов
	void SetCharacterSize(unsigned int size)
	{
		text.setCharacterSize(size);
	}
	// отрисовка текста
	virtual void Draw(RenderWindow& window)
	{
		window.draw(text);
	}
};

class Button
{
public: enum State { peace, alert, active, block };
protected:
	Vector2f pos;
	Vector2f origin;
	Vector2f size;
	SupEllipseShape sprite;
	Texture texture[4];
	Color col[4];
	Color outline[4];
	float outlineT[4] = { 0, 0, 0, 0};
	Font font;
	Vector2f TextPos;
	Text text;
	float angel = 0;
	unsigned int ButtonState = peace;
	bool clik = false;
	bool visible = true;
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
					SetTexture(Button::active);
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
		FloatRect rect(pos - origin, size);
		Vector2f Pos = MousePos - pos;
		Vector2f p;
		float Angel = -angel * 3.14 / 180;
		p.x = Pos.x * cos(Angel) - Pos.y * sin(Angel);
		p.y = Pos.x * sin(Angel) + Pos.y * cos(Angel);

		if (rect.contains(Vector2f(pos.x + p.x, pos.y + p.y)))
			return true;
		else
			return false;
	}

	void SetNM(float N, float M)
	{
		sprite.setNM(N, M);
	}

	virtual void SetScale(Vector2f Scale)
	{
		size.x *= Scale.x;
		size.y *= Scale.y;
		sprite.setSize(size);
	}

	// установить поворот кнопки
	virtual void SetRotation(float Angel)
	{
		angel = Angel;
		sprite.setRotation(angel);
	}

	// установить local point кнопки
	virtual void SetOrigin(Vector2f Size)
	{
		origin = Size;
		sprite.setOrigin(origin);
	}

	// установить local point кнопки в центр
	void SetOriginCenter(Vector2f Size)
	{
		origin = Vector2f(Size.x / 2, Size.y / 2);
		sprite.setOrigin(origin);
	}

	// установить размер кнопки
	virtual void SetSize(Vector2f Size)
	{
		size = Size;
		sprite.setSize(size);
	}

	// получить размер кнопки
	Vector2f GetSize()
	{
		return size;
	}

	// установить позицию кнопки
	virtual void SetPosition(Vector2f position)
	{
		pos = position;
		sprite.setPosition(pos);
	}

	// получить позицию кнопки
	Vector2f GetPosition()
	{
		return pos;
	}

	// установить цвет кнопки
	void SetFillColor(Color color, unsigned int state)
	{
		col[state] = color;
	}

	// установить цвет обводки кнопки
	void SetOutlineColor(Color color, unsigned int state)
	{
		outline[state] = color;
	}

	// установить размер обводки кнопки
	void SetOutlineThickness(float thickness, unsigned int state)
	{
		outlineT[state] = thickness;
	}

	// загрузить текстуру кнопки
	void LoadTexture(std::string str, unsigned int state)
	{
		texture[state].loadFromFile(str);
	}

	// установить текстуру кнопки
	virtual void SetTexture(unsigned int state)
	{
		ButtonState = state;
		
		sprite.setFillColor(col[ButtonState]);
		sprite.setOutlineColor(outline[ButtonState]);
		sprite.setOutlineThickness(outlineT[ButtonState]);
		
		sprite.setTexture(&texture[ButtonState]);
	}

	// установить состояние кнопки
	void SetState(unsigned int state)
	{
		ButtonState = state;
	}

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

	// установить текст кнопки
	void SetText(std::string str)
	{
		text.setString(str);
	}

	void SetText(std::wstring str)
	{
		text.setString(str);
	}

	// установить цвет кнопки
	void SetTextFillColor(Color color)
	{
		text.setFillColor(color);
	}

	// установить цвет обводки кнопки
	void SetTextOutlineColor(Color color)
	{
		text.setOutlineColor(color);
	}

	// установить размер обводки кнопки
	void SetTextOutlineThickness(float thickness)
	{
		text.setOutlineThickness(thickness);
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

	// установить local point текста
	void SetTextOrigin(Vector2f Size)
	{
		text.setOrigin(Size);
	}

	// установить local point текста в центр
	void SetTextOriginCenter(Vector2f Size)
	{
		text.setOrigin(Vector2f(Size.x / 2, Size.y / 2));
	}

	// установить поворот текста кнопки
	void SetTextRotation(float Angel)
	{
		text.setRotation(Angel);
	}

	// установить размер шрифта текста кнопки
	void SetTextCharacterSize(unsigned int size)
	{
		text.setCharacterSize(size);
	}

	//видимость
	void SetVisible(bool param)
	{
		visible = param;
	}

	SupEllipseShape getSprite()
	{
		return sprite;
	}

	Texture getTexture(unsigned int state)
	{
		return texture[state];
	}

	// отрисовка кнопки
	virtual void Draw(RenderWindow& window)
	{
		if (visible)
		{
			window.draw(sprite);
			window.draw(text);
		}
	}
};