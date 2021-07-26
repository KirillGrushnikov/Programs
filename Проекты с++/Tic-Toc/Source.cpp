#include "SFML/Graphics.hpp"
#include <locale>
#include <time.h>
#include <Windows.h>
#include <iostream>
#include <vector>
#include "SuperEllipse.hpp"

using namespace std;
using namespace sf;

int windowX = 300;
int windowY = 300;
unsigned style = 7;

RenderWindow window(VideoMode(windowX, windowY), "Tic-Toc", style, ContextSettings());
Event event;
Vector2f pos;
Color background_color(20, 20, 20);

bool tic_toc = true;

int win = -1;

SupEllipseShape tic_add[2];
CircleShape toc_add;

struct mas
{
    SupEllipseShape a[2];
};

RectangleShape Line[4];
RectangleShape WinLine;
vector <mas> tic;
vector <CircleShape> toc;

int Map[3][3];

void Start()
{
    window.setFramerateLimit(60);

    for (int i = 0; i < 4; i++)
    {
        Line[i].setFillColor(Color::White);
        if (i < 2)
        {
            Line[i].setSize(Vector2f(1, windowY));
            Line[i].setPosition(i * 100 + 100, 0);
        }
        else
        {
            Line[i].setSize(Vector2f(windowX, 1));
            Line[i].setPosition(0, (i - 2) * 100 + 100);
        }
    }

    for (int i = 0; i < 2; i++)
    {
        tic_add[i].setFillColor(Color::Blue);
        tic_add[i].setRadius(Vector2f(55, 15));
        tic_add[i].setNM(32, 8);
        tic_add[i].SetPointCount(500);
        tic_add[0].setRotation(45);
        tic_add[1].setRotation(-45);
    }
    toc_add.setFillColor(background_color);
    toc_add.setRadius(35);
    toc_add.setOutlineColor(Color::Red);
    toc_add.setOutlineThickness(10);
    toc_add.setPointCount(500);
    toc_add.setOrigin(toc_add.getRadius(), toc_add.getRadius());

    WinLine.setFillColor(Color::Green);
    WinLine.setSize(Vector2f(425, 3));
    //WinLine.setNM(32, 2);
    //WinLine.setOrigin(WinLine.getRadius().x /2, 1);
    //WinLine.SetPointCount(500);
}

void keyboard()
{
    if (GetAsyncKeyState(27)) //esc выход
        exit(0);
}

void clear()
{
    switch (win)
    {
    case 1:
        Sleep(500);
        tic.clear();
        toc.clear();
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                Map[i][j] = 0;
        win = -1;
        tic_toc = true;
        break;
    case 2:
        Sleep(500);
        tic.clear();
        toc.clear();
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                Map[i][j] = 0;
        win = -1;
        tic_toc = true;
        break;
    default:
        if (tic.size() + toc.size() == 9)
        {
            Sleep(500);
            tic.clear();
            toc.clear();
            for (int i = 0; i < 3; i++)
                for (int j = 0; j < 3; j++)
                    Map[i][j] = 0;
            tic_toc = true;
        }
        break;
    }
}

void chek()
{
    int sch_tic = 0, sch_toc = 0;
    for (int i = 0; i < 3; i++)
    {
        if (Map[0][i] == 2 && Map[1][i] == 2 && Map[2][i] == 2 || Map[i][0] == 2 && Map[i][1] == 2 && Map[i][2] == 2)
        {
            if (Map[0][i] == 2 && Map[1][i] == 2 && Map[2][i] == 2)
            {
                WinLine.setPosition(0, i * 100 + 50);
                WinLine.setRotation(0);
            }
            else
            {
                WinLine.setPosition(i * 100 + 50, 0);
                WinLine.setRotation(90);
            }
            win = 2;
            break;
        }
        if (Map[0][i] == 1 && Map[1][i] == 1 && Map[2][i] == 1 || Map[i][0] == 1 && Map[i][1] == 1 && Map[i][2] == 1)
        {
            if (Map[0][i] == 1 && Map[1][i] == 1 && Map[2][i] == 1)
            {
                WinLine.setPosition(0, i * 100 + 50);
                WinLine.setRotation(0);
            }
            else
            {
                WinLine.setPosition(i * 100 + 50, 0);
                WinLine.setRotation(90);
            }
            win = 1;
            break;
        }
        if (Map[0][0] == 2 && Map[1][1] == 2 && Map[2][2] == 2 || Map[0][2] == 2 && Map[1][1] == 2 && Map[2][0] == 2)
        {
            if (Map[0][0] == 2 && Map[1][1] == 2 && Map[2][2] == 2)
            {
                WinLine.setRotation(45);
                WinLine.setPosition(0, 0);
            }
            else
            {
                WinLine.setRotation(-45);
                WinLine.setPosition(0, windowY);
            }
            win = 2;
            break;
        }
        if (Map[0][0] == 1 && Map[1][1] == 1 && Map[2][2] == 1 || Map[0][2] == 1 && Map[1][1] == 1 && Map[2][0] == 1)
        {
            if (Map[0][0] == 1 && Map[1][1] == 1 && Map[2][2] == 1)
            {
                WinLine.setRotation(45);
                WinLine.setPosition(0, 0);
            }
            else
            {
                WinLine.setRotation(-45);
                WinLine.setPosition(0, windowY);
            }
            win = 1;
            break;
        }
    }
}

void Tick()
{
    for(int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (pos.x > i * 100 && pos.x < i * 100 + 100 && i * 100 + 100 <= windowX && pos.y > j * 100 && pos.y < j * 100 + 100 && j * 100 + 100 <= windowY)
            {
                if (GetAsyncKeyState(1) & 0x1 && Map[i][j] == 0)
                {
                    if (tic_toc)
                    {
                        for (int z = 0; z < 2; z++)
                            tic_add[z].setPosition(i * 100 + 50, j * 100 + 50);
                        Map[i][j] = 1;
                        mas s;
                        s.a[0] = tic_add[0];
                        s.a[1] = tic_add[1];
                        tic.push_back(s);
                    }
                    else
                    {
                        toc_add.setPosition(i * 100 + 50, j * 100 + 50);
                        Map[i][j] = 2;
                        toc.push_back(toc_add);
                    }

                    tic_toc = !tic_toc;
                    for (int i = 0; i < 3; i++)
                    {
                        for (int j = 0; j < 3; j++)
                            cout << Map[i][j] << ' ';
                        cout << endl;
                    }
                    cout << win << endl;
                }
                chek();
            }
}

int main()
{
    setlocale(LC_ALL, "Russian");
    srand(std::time(NULL));
    Start();

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
        for (int i = 0; i < 4; i++)
            window.draw(Line[i]);

        for (int i = 0; i < toc.size(); i++)
            window.draw(toc[i]);

        for (int i = 0; i < tic.size(); i++)
            for(int j = 0; j < 2; j++)
                window.draw(tic[i].a[j]);

        if (win != -1)
            window.draw(WinLine);

        window.display();
        clear();
    }
}