#include "SFML/Graphics.hpp"
#include <locale>
#include <time.h>
#include <Windows.h>
#include <sstream>
#include <iostream>

using namespace std;
using namespace sf;

int windowX = 300;
int windowY = 300;
unsigned style = 7;

RenderWindow window(VideoMode(windowX, windowY), "15", style, ContextSettings());
Event event;
Vector2f pos;
Color background_color(20, 20, 20);

RectangleShape Map;
RectangleShape button[15];
Text buttonText[15];

Font MyFont;

bool Win = false;

Vector2f VoidPos;
vector <Vector2f> win;
vector <string> number;
vector <string> numberwin;

Vector2f Temp;
bool Move = false;
int Itemp = -1;

void Start()
{
    window.setFramerateLimit(60);


    Map.setFillColor(background_color);
    Map.setSize(Vector2f(287, 287));
    Map.setPosition(6, 6);
    Map.setOutlineColor(Color(200, 100, 200));
    Map.setOutlineThickness(1);

    MyFont.loadFromFile("Font\\8277.ttf");

    for (int i = 0; i < 15; i++)
    {
        std::ostringstream NUM;
        NUM << i + 1;
        number.push_back(NUM.str());
        numberwin.push_back(NUM.str());
    }


    int sch = 0;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
        {
            if (sch < 15)
            {
                button[sch].setFillColor(background_color);
                button[sch].setOutlineColor(Color::Cyan);
                button[sch].setOutlineThickness(1);
                button[sch].setSize(Vector2f(69, 69));
                button[sch].setPosition(Map.getPosition().x + 3 + j * (button[sch].getSize().x + 1), Map.getPosition().y + 3 + i * (button[sch].getSize().y + 1));


                buttonText[sch].setFont(MyFont);
                buttonText[sch].setFillColor(Color::Cyan);
                //int temp = rand() % number.size();
                ostringstream NUM;
                NUM << sch + 1;
                buttonText[sch].setString(NUM.str());
                buttonText[sch].setPosition(button[sch].getPosition().x + button[sch].getSize().x / 2, button[sch].getPosition().y + button[sch].getSize().y / 2);
                win.push_back(buttonText[sch].getPosition());
                buttonText[sch].setCharacterSize(30);
                //if (number[temp].length() < 2)
                if (sch < 9)
                    buttonText[sch].setOrigin(8, 20);
                else
                    buttonText[sch].setOrigin(15, 20);

                //number.erase(number.begin() + temp);
            }
            else
            {
                VoidPos.x = Map.getPosition().x + 3 + j * 70;
                VoidPos.y = Map.getPosition().y + 3 + i * 70;
            }
            sch++;
        }
}

void keyboard()
{

    if (GetAsyncKeyState(27)) //esc выход
        exit(0);
}

bool chek(Vector2f B, Vector2f V, int Size)
{
    if (B.x == V.x)
    {
        if (B.y == V.y + Size || B.y == V.y - Size)
            return true;
    }
    else
        if (B.y == V.y)
        {
            if (B.x == V.x + Size || B.x == V.x - Size)
                return true;
        }

    return false;
}

void chekWin(Text& Bt)
{
    for (int j = 0; j < 15; j++)
    {
        if (Bt.getString() == numberwin[j] && win[j] == Bt.getPosition())
        {
            Bt.setFillColor(Color::Green);
        }
    }
}

void Randomis()
{
    for (int i = 0; i < rand()%10000 + 100; i++)
    {
        for (int j = 0; j < 15; j++)
        {
            if (chek(button[j].getPosition(), VoidPos, button[j].getSize().x + 1))
            {
                Vector2f temp(VoidPos);
                VoidPos = button[j].getPosition();
                button[j].setPosition(temp);
                buttonText[j].setPosition(button[j].getPosition().x + button[j].getSize().x / 2, button[j].getPosition().y + button[j].getSize().y / 2);
            }
        }
    }
}

void RandomisNew()
{
    for (int i = 0; i < rand() % 10000 + 100; i++)
    {
        for (int j = 0; j < 15; j++)
        {
            if (chek(button[j].getPosition(), VoidPos, button[j].getSize().x + 1))
            {
                Vector2f temp(VoidPos);
                VoidPos = button[j].getPosition();
                button[j].setPosition(temp);
                buttonText[j].setPosition(button[j].getPosition().x + button[j].getSize().x / 2, button[j].getPosition().y + button[j].getSize().y / 2);
            }
        }

        window.clear(background_color);
        window.draw(Map);
        for (int z = 0; z < 15; z++)
        {
            window.draw(button[z]);
            window.draw(buttonText[z]);
        }

        window.display();
    }

    for (int i = 0; i < 15; i++)
    {
        button[i].setOutlineColor(Color::Cyan);
        buttonText[i].setFillColor(Color::Cyan);
    }
    Win = false;
}

void Tick()
{
    int schWin = 0;
    for (int i = 0; i < 15; i++)
    {
        if (!Win && pos.x > button[i].getPosition().x && pos.x < button[i].getPosition().x + button[i].getSize().x && pos.y > button[i].getPosition().y && pos.y < button[i].getPosition().y + button[i].getSize().y)
        {
            buttonText[i].setFillColor(Color::Red);
            if (GetAsyncKeyState(1) & 0x1 && !Move)
            {
                if (chek(button[i].getPosition(), VoidPos, button[i].getSize().x + 1))
                {
                    Move = true;
                    Temp = button[i].getPosition();
                    Itemp = i;
                }
            }
        }
        else
        {
            buttonText[i].setFillColor(Color::Cyan);
            chekWin(buttonText[i]);
            if (buttonText[i].getFillColor() == Color::Green)
                schWin++;
            if (schWin == 15)
            {
                Win = true;
                for (int j = 0; j < 15; j++)
                {
                    button[j].setOutlineColor(Color(200, 100, 200));
                    buttonText[j].setFillColor(Color(200, 100, 200));
                }
            }
        }

        if (Move)
        {
            if(VoidPos.x == button[Itemp].getPosition().x)
                if(VoidPos.y > button[Itemp].getPosition().y)
                    button[Itemp].move(0, 1);
                else
                    button[Itemp].move(0, -1);
            else
                if (VoidPos.x > button[Itemp].getPosition().x)
                    button[Itemp].move(1, 0);
                else
                    button[Itemp].move(-1, 0);

            buttonText[Itemp].setPosition(button[Itemp].getPosition().x + button[Itemp].getSize().x / 2, button[Itemp].getPosition().y + button[Itemp].getSize().y / 2);

            if (button[Itemp].getPosition() == VoidPos)
            {
                Move = false;
                VoidPos = Temp;
            }
        }
    }
}

int main()
{
    setlocale(LC_ALL, "Russian");
    srand(std::time(NULL));
    Start();
    Randomis();

    Clock clock;
    float timer = 0;
    double delay = 0.005;

    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            keyboard();
        }

        Vector2i pixelPos = Mouse::getPosition(window);//забираем коорд курсора
        pos = window.mapPixelToCoords(pixelPos);//переводим их в игровые (уходим от коорд окна)


        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;
        if (timer > delay) { timer = 0; Tick(); }

        window.clear(background_color);
        window.draw(Map);
        for (int i = 0; i < 15; i++)
        {
            window.draw(button[i]);
            window.draw(buttonText[i]);
        }

        window.display();

        if (Win)
        {
            if(GetAsyncKeyState(32) & 0x1)
                RandomisNew();
        }
    }
}