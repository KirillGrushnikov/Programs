#include "SFML/Graphics.hpp"
#include <locale>
#include <time.h>
#include <Windows.h>
#include <iostream>

#include <vector>
#include <bitset>

#include "Grid.h"


#define COUNT_CIRCLES 15

#if defined _DEBUG
    #define CELL_SIZE 40
#else
    #define CELL_SIZE 20
#endif

using namespace sf;

int random(int min, int max)
{
    return rand() % (max - min) + min;
}

float hueToRgb(float v1, float v2, float vH) {
    if (vH < 0)
        vH += 1;

    if (vH > 1)
        vH -= 1;

    if ((6 * vH) < 1)
        return (v1 + (v2 - v1) * 6 * vH);

    if ((2 * vH) < 1)
        return v2;

    if ((3 * vH) < 2)
        return (v1 + (v2 - v1) * ((2.0f / 3) - vH) * 6);

    return v1;
}

sf::Color hslToRgb(int h, float s, float l) {
    unsigned char r = 0;
    unsigned char g = 0;
    unsigned char b = 0;

    if (s == 0)
    {
        r = g = b = (unsigned char)(l * 255);
    }
    else
    {
        float v1, v2;
        float hue = (float)h / 360;

        v2 = (l < 0.5) ? (l * (1 + s)) : ((l + s) - (l * s));
        v1 = 2 * l - v2;

        r = (unsigned char)(255 * hueToRgb(v1, v2, hue + (1.0f / 3)));
        g = (unsigned char)(255 * hueToRgb(v1, v2, hue));
        b = (unsigned char)(255 * hueToRgb(v1, v2, hue - (1.0f / 3)));
    }

    return sf::Color(r, g, b);
}

int main()
{
    setlocale(LC_ALL, "Russian");
    srand(std::time(NULL));

    //---INIT SFML---//
    int windowX = 1280;
    int windowY = 720;
    unsigned style = Style::Default;

    RenderWindow window(VideoMode(windowX, windowY), "Metaballs(CPU)", style, ContextSettings());
    Event event;
    Vector2f MousePos;

    window.setFramerateLimit(60);
    //---INIT SFML END---//


    std::vector<CircleShape> circles;
    std::vector<sf::Vector2f> vec;



    for (int i = 0; i < COUNT_CIRCLES; i++)
    {
        CircleShape circle;
        float vx = random(-2, 2), vy = random(-2, 2);
        while (vx == 0) vx = random(-2, 2);
        while (vy == 0) vy = random(-2, 2);
        vec.push_back(Vector2f(vx, vy));
        circle.setPosition(Vector2f(random(5, windowX), random(5, windowY)));
        circle.setRadius(random(20, 50));
        circle.setOrigin(circle.getRadius(), circle.getRadius());
        circle.setFillColor(Color(0, 0, 0, 0));
        circle.setOutlineThickness(1);
        circle.setOutlineColor(hslToRgb(random(0, 360), 1, 0.5f));

        circles.push_back(circle);
    }

    Grid grid(Vector2i(windowX, windowY), CELL_SIZE, false);

    Clock clock;
    float timer = 0;
    double delay = 0.005;

    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            if (GetAsyncKeyState(27)) //esc выход
                exit(0);
        }

        if (window.hasFocus())
        {
            Vector2i pixelPos = Mouse::getPosition(window);//забираем коорд курсора
            MousePos = window.mapPixelToCoords(pixelPos);//переводим их в игровые (уходим от коорд окна)


            float time = clock.getElapsedTime().asSeconds();
            clock.restart();
            timer += time;
            if (timer > delay)
            {
                timer = 0;
                
                for (int i = 0; i < COUNT_CIRCLES; i++)
                {
                    Vector2f pos = circles[i].getPosition();
                    pos.x += vec[i].x;
                    pos.y += vec[i].y;

                    circles[i].setPosition(pos);

                    if (pos.x < 0 || pos.x > windowX) vec[i].x *= -1;
                    if (pos.y < 0 || pos.y > windowY) vec[i].y *= -1;
                }
                
                grid.life(circles);
                grid.update();
            }

            window.clear();
            #if defined _DEBUG
            for (int i = 0; i < COUNT_CIRCLES; i++)
                window.draw(circles[i]);
            #endif
            grid.draw(window);
            window.display();
        }
    }
}