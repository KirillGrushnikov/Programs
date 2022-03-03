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

	// ������� ��������
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

	// �������� ����� �� ������
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

	// �������� ��������� �� ������
	virtual bool ChekMouse(Vector2f MousePos)
	{
		if (MousePos.x > pos.x && MousePos.x < pos.x + size.x && MousePos.y > pos.y && MousePos.y < pos.y + size.y)
			return true;
		else
			return false;
	}

	// ���������� ������ ������
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

	// �������� ������ ������
	Vector2f GetSize()
	{
		return size;
	}

	// ���������� ������� ������
	void SetPosition(Vector2f position)
	{
		pos = position;
		sprite.setPosition(pos);
	}

	// �������� ������� ������
	Vector2f GetPosition()
	{
		return pos;
	}

	// ��������� �������� ������
	void LoadTexture(std::string str, unsigned int state)
	{
		texture[state].loadFromFile(str);
	}

	// ���������� �������� ������
	void SetTexture(unsigned int state)
	{
		ButtonState = state;
		sprite.setTexture(&texture[ButtonState]);
	}

	// ���������� ��������� ������
	void SetState(unsigned int state)
	{
		ButtonState = state;
	}

	// �������� ��������� ������
	unsigned int GetState()
	{
		return ButtonState;
	}

	// ���������� ����� ������ ������
	virtual void SetFont(std::string str)
	{
		font.loadFromFile(str);
		text.setFont(font);
	}

	void SetTextRotate(float angel)
	{
		text.setRotation(angel);
	}

	// ���������� ����� ������
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

	// �������� ����� ������
	std::string GetTextString()
	{
		return text.getString();
	}

	// ���������� ������� ������ ������
	void SetTextPosition(Vector2f position)
	{
		TextPos = position;
		text.setPosition(TextPos);
	}

	// �������� ������� ������ ������
	Vector2f GetTextPosition()
	{
		return TextPos;
	}

	// ���������� ������ ������ ������ ������
	void SetTextCharacterSize(unsigned int size)
	{
		text.setCharacterSize(size);
	}

	// ��������� ������
	virtual void Draw(RenderWindow& window)
	{
		window.draw(sprite);

		window.draw(dots);

		window.draw(text);
	}
};