#include "SFML/Graphics.hpp"
#include <locale>
#include <time.h>
#include <iostream>
#include <Windows.h>
#include "Button.hpp"
#include "Calculate.h"

using namespace std;
using namespace sf;

const int windowX = 340; //376
const int windowY = 600;
unsigned style = Style::Default;

RenderWindow window(VideoMode(windowX, windowY), L"Калькулятор", style, ContextSettings());
Event event;
View view;
Vector2f MousePos;

const int countButtonNum = 10;
const int countButtonSing = 9;
Button ButtonNum[countButtonNum];
Button ButtonSing[countButtonSing];
Button ButtonEquals, ButtonClear, ButtonErase;

string Example = "0";
Text TExample;
Font FExample;

string Result;
Text TResult;

void Start()
{
    Image im;
    im.loadFromFile("Texture\\calculator.png");
    window.setIcon(im.getSize().x, im.getSize().y, im.getPixelsPtr());
    window.setFramerateLimit(60);
    view.setSize(windowX, windowY);
    string pathFont = "Font\\Andala.otf";

    FExample.loadFromFile(pathFont);
    TExample.setFont(FExample);
    TExample.setString(Example);
    TExample.setCharacterSize(35);
    TExample.setPosition(5, 40);

    TResult.setFont(FExample);
    TResult.setCharacterSize(30);
    TResult.setPosition(5, 75);

    float N = 16, M = 16;
    Vector2f Size = { 80, 80 };
    //кнопки цифры
    for (int i = 0; i < countButtonNum; i++)
    {
        ButtonNum[i].SetSize(Size);
        ButtonNum[i].SetNM(N, M);
        ButtonNum[i].SetPosition(Vector2f(0, 0));
        for (int j = 1; j <= 3; j++)
            ButtonNum[i].LoadTexture("Texture\\Button" + to_string(j) + ".png", j-1);
        ButtonNum[i].SetTexture(0);
        ButtonNum[i].SetFont(pathFont);
        ButtonNum[i].SetText(to_string(i));
        ButtonNum[i].SetTextCharacterSize(35);
        ButtonNum[i].SetTextPosition(ButtonNum[i].GetPosition());
    }

    int sch = 1;
    for (int i = 1; i <= 3; i++)
    {
        ButtonNum[sch].SetSize(Size);
        ButtonNum[0].SetSize(Size);
        ButtonNum[0].SetPosition(Vector2f(4 + Size.x + 4, windowY - (Size.y + 4)));
        ButtonNum[0].SetTextPosition(Vector2f(ButtonNum[0].GetPosition().x + 28, ButtonNum[0].GetPosition().y + 18));
        for (int j = 0; j < 3; j++)
        {
            ButtonNum[sch].SetPosition(Vector2f(4 + j * (Size.x + 4), windowY - (Size.y + 4) - i * (Size.y + 4)));
            ButtonNum[sch].SetTextPosition(Vector2f(ButtonNum[sch].GetPosition().x + 28, ButtonNum[sch].GetPosition().y + 18));
            sch++;
        }
    }
    
    // кнопки операции
    for (int i = 0; i < countButtonSing; i++)
    {
        string sings = { "-,+*/()^%" };
        ButtonSing[i].SetSize(Size);
        ButtonSing[i].SetNM(N, M);
        ButtonSing[i].SetPosition(Vector2f(0, 0));
        for (int j = 1; j <= 3; j++)
            ButtonSing[i].LoadTexture("Texture\\ButtonSing" + to_string(j) + ".png", j - 1);
        ButtonSing[i].SetTexture(0);
        ButtonSing[i].SetFont(pathFont);
        ButtonSing[i].SetText(sings[i]);
        ButtonSing[i].SetTextCharacterSize(35);
        ButtonSing[i].SetTextPosition(ButtonSing[i].GetPosition());
    }

    // минус
    ButtonSing[0].SetSize(Size);
    ButtonSing[0].SetPosition(Vector2f(4, windowY - (Size.y + 4)));
    ButtonSing[0].SetTextPosition(Vector2f(ButtonSing[0].GetPosition().x + 28, ButtonSing[0].GetPosition().y + 18));

    // точка
    ButtonSing[1].SetSize(Size);
    ButtonSing[1].SetPosition(Vector2f(4 + (Size.x + 4) * 2, windowY - (Size.y + 4)));
    ButtonSing[1].SetTextPosition(Vector2f(ButtonSing[1].GetPosition().x + 28, ButtonSing[1].GetPosition().y + 18));


    for (int i = 2; i < 5; i++)
    {
        ButtonSing[i].SetSize(Size);
        ButtonSing[i].SetPosition( Vector2f( 4 + (Size.x + 4) * 3, windowY - i * (Size.y + 4) ) );
        ButtonSing[i].SetTextPosition(Vector2f(ButtonSing[i].GetPosition().x + 28, ButtonSing[i].GetPosition().y + 18));
    }

    for (int i = 5; i < countButtonSing; i++)
    {
        ButtonSing[i].SetSize(Size);
        ButtonSing[i].SetPosition(Vector2f(4 + (i - 5) * (Size.x + 4), windowY - (Size.y + 4)* 5));
        ButtonSing[i].SetTextPosition(Vector2f(ButtonSing[i].GetPosition().x + 28, ButtonSing[i].GetPosition().y + 18));
    }

    //ButtonSing[7].SetTextRotate(180);
    ButtonSing[7].SetTextPosition(Vector2f(ButtonSing[7].GetPosition().x + 28, ButtonSing[7].GetPosition().y + 28));

    // Кнопка очистки
    ButtonClear.SetSize(Vector2f(Size.x, 40));
    ButtonClear.SetNM(16, 8);
    ButtonClear.SetPosition(Vector2f(4, windowY - (Size.y - 4) * 6 - 8));
    for (int j = 1; j <= 3; j++)
        ButtonClear.LoadTexture("Texture\\ButtonSing" + to_string(j) + ".png", j - 1);
    ButtonClear.SetTexture(0);
    ButtonClear.SetFont(pathFont);
    ButtonClear.SetText("C");
    ButtonClear.SetTextCharacterSize(25);
    ButtonClear.SetTextPosition(Vector2f(ButtonClear.GetPosition().x + 28, ButtonClear.GetPosition().y + 3));

    // Кнопка удаления
    ButtonErase = ButtonClear;
    ButtonErase.SetPosition(Vector2f(4 + (Size.x + 4) * 3, windowY - (Size.y - 4) * 6 - 8));
    ButtonErase.SetText("<=");
    ButtonErase.SetTextPosition(Vector2f(ButtonErase.GetPosition().x + 28, ButtonErase.GetPosition().y + 3));
    
    // Кнопка равно
    ButtonEquals.SetSize(Size);
    ButtonEquals.SetNM(N, M);
    ButtonEquals.SetPosition(Vector2f(4 +(Size.x + 4) * 3, windowY - Size.y - 4));
    for (int j = 1; j <= 3; j++)
        ButtonEquals.LoadTexture("Texture\\ButtonEquals" + to_string(j) + ".png", j - 1);
    ButtonEquals.SetTexture(0);
    ButtonEquals.SetFont(pathFont);
    ButtonEquals.SetText("=");
    ButtonEquals.SetTextCharacterSize(60);
    ButtonEquals.SetTextPosition(Vector2f(ButtonEquals.GetPosition().x + 24, ButtonEquals.GetPosition().y + 3));
}

int main()
{
    setlocale(LC_ALL, "Russian");
    srand(std::time(NULL));
    Start();

    Clock clock;
    float timer = 0;
    double delay = 0.005;

    bool Res = false;

    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
            // цифры
            for (int i = 0; i < countButtonNum; i++)
            {
                if (ButtonNum[i].ChekClik(MousePos, event) || GetAsyncKeyState(96 + i) & 0x1 || GetAsyncKeyState(48 + i) & 0x1)
                {
                    if (Res)
                    {
                        Result.clear();
                        TResult.setString(Result);
                        Example.pop_back();
                        Res = false;
                    }
                    if (Example.size() == 1 && Example[0] == '0')
                    {
                        Example.pop_back();
                        Example += to_string(i);
                    }
                    else
                        if(Example.size() < 15)
                            Example += to_string(i);
                    TExample.setString(Example);
                    TExample.setFillColor(Color::White);
                    TExample.setCharacterSize(35);
                }
            }

            // операции
            for (int i = 0; i < countButtonSing; i++)
            {
                if (ButtonSing[i].ChekClik(MousePos, event))
                {
                    if (Res)
                    {
                        if (Result != "Error" && Result != "Pain")
                            Example = Result;
                        else
                            Example = "0";
                        Result.clear();
                        TResult.setString(Result);
                        Res = false;
                    }
                    if (Example.size() == 1 && Example[0] == '0' && i == 5)
                    {
                        Example.pop_back();
                        Example += ButtonSing[i].GetTextString();
                    }
                    else
                        if (Example.size() < 15)
                            Example += ButtonSing[i].GetTextString();
                    TExample.setString(Example);
                    TExample.setFillColor(Color::White);
                    TExample.setCharacterSize(35);
                }
            }

            // очистка
            if (ButtonClear.ChekClik(MousePos, event) || GetAsyncKeyState(46) & 0x1)
            {
                Result.clear();
                TResult.setString(Result);
                Example = "0";
                TExample.setString(Example);
                TExample.setFillColor(Color::White);
                TExample.setCharacterSize(35);
                Res = false;
            }

            // удаление
            if (ButtonErase.ChekClik(MousePos, event) || GetAsyncKeyState(8) & 0x1)
            {
                Result.clear();
                TResult.setString(Result);
                if(Example.size() > 1)
                    Example.pop_back();
                else
                    Example = "0";
                TExample.setString(Example);
                TExample.setFillColor(Color::White);
                TExample.setCharacterSize(35);
                Res = false;
            }

            // равно
            if (ButtonEquals.ChekClik(MousePos, event) || GetAsyncKeyState(13) & 0x1)
            {
                if (!Res)
                {
                    Result = Calculate(Example);
                    TResult.setString(Result);
                    Example += "=";
                    TExample.setString(Example);
                    TExample.setFillColor(Color(106, 105, 105));
                    TExample.setCharacterSize(20);
                    Res = true;
                }
            }

        }

        Vector2i pixelPos = Mouse::getPosition(window);//забираем коорд курсора
        MousePos = window.mapPixelToCoords(pixelPos);//переводим их в игровые (уходим от коорд окна)

        ButtonClear.anim(MousePos);

        //window.setView(view);
        window.clear(Color(30, 30, 30));
        for (int i = 0; i < countButtonNum; i++)
            ButtonNum[i].Draw(window);

        for (int i = 0; i < countButtonSing; i++)
            ButtonSing[i].Draw(window);

        ButtonEquals.Draw(window);
        ButtonClear.Draw(window);
        ButtonErase.Draw(window);
        window.draw(TExample);
        window.draw(TResult);
        window.display();
    }
}