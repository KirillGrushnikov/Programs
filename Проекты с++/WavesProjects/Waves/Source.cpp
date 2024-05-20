#include "SFML/Graphics.hpp"
#include <locale>
#include <time.h>
#include <Windows.h>
#include <iostream>

#include "Simulation.h"
#include "Types.h"
#include "GUI.h"

using namespace sf;

int main()
{
    setlocale(LC_ALL, "Russian");
    srand(std::time(NULL));

    //---INIT SFML---//
    int windowX = 1280;
    int windowY = 720;
    unsigned style = Style::Default;

    RenderWindow window(VideoMode(windowX, windowY), "Waves", style, ContextSettings());
    Event event;
    Vector2f MousePos;

    window.setFramerateLimit(60);
    //---INIT SFML END---//

    DynamicParams dynamic_params;
    Simulation sim(&dynamic_params);

    //---INIT IMGUI---//
    ImGui::SFML::Init(window);
    ImGui::StyleColorsCustom();
    //---INIT IMGUI END---//

    Clock clock, delta_clock;
    float timer = 0;
    double delay = 0.005;

    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == Event::Closed)
                window.close();

            if (GetAsyncKeyState(27)) //esc выход
                exit(0);
        }

        if (window.hasFocus())
        {
            Vector2i pixelPos = Mouse::getPosition(window);//забираем коорд курсора
            MousePos = window.mapPixelToCoords(pixelPos);//переводим их в игровые (уходим от коорд окна)
            dynamic_params.mouse_pos = MousePos;


            ImGuiMenu(window, delta_clock, dynamic_params, sim);

            float time = clock.getElapsedTime().asSeconds();
            clock.restart();
            timer += time;
            if (timer > delay)
            {
                timer = 0;
                sim.update();
            }

            window.clear(sf::Color(30, 30, 30));
            sim.draw(window);
            ImGui::SFML::Render(window);
            window.display();
        }
    }

    ImGui::SFML::Shutdown();
    return 0;
}