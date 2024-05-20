#include "SFML/Graphics.hpp"
#include <locale>
#include <time.h>
#include <Windows.h>
#include <iostream>

#include "Simulation.h"

using namespace sf;

int main()
{
    setlocale(LC_ALL, "Russian");
    srand(std::time(NULL));

    //---INIT SFML---//
    int windowX = 1280;
    int windowY = 720;
    unsigned style = Style::Default;

    RenderWindow window(VideoMode(windowX, windowY), "Simple Collision", style, ContextSettings());
    Event event;
    Vector2f MousePos;

    window.setFramerateLimit(60);
    //---INIT SFML END---//

    Clock clock;
    float timer = 0, timer_pause = 0, timer_clear = 0;
    double delay = 0.005, delay_pause = 0.2, delay_clear= 0.02;
    sf::Clock deltaClock;
    Simulation sim(false);

    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            if (GetAsyncKeyState(27)) //esc выход
                exit(0);

            if (GetAsyncKeyState(32) && timer_pause > delay_pause) //space пауза
            {
                timer_pause = 0;
                sim.start_pause();
            }

            if (GetAsyncKeyState(18) && timer_clear > delay_clear) //alt очистка
            {
                timer_clear = 0;
                sim.clear();
            }
        }

        if (window.hasFocus())
        {
            Vector2i pixelPos = Mouse::getPosition(window);//забираем коорд курсора
            MousePos = window.mapPixelToCoords(pixelPos);//переводим их в игровые (уходим от коорд окна)

            if (GetAsyncKeyState(2)) //ПКМ спавн
            {
                sim.addObjectRGB(MousePos, rand() % 15 + 5);
            }

            float time = clock.getElapsedTime().asSeconds();
            clock.restart();

            timer_pause += time;
            timer_clear += time;

            float dtime = deltaClock.getElapsedTime().asSeconds();
            sim.update(dtime);
            deltaClock.restart();

            window.clear();
            sim.draw(window);
            window.display();
        }
    }
}