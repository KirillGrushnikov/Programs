#include "SFML/Graphics.hpp"
#include <locale>
#include <time.h>
#include <Windows.h>
#include <iostream>
#include <fstream>
#include "WaveFunctionCollapse2D.h"

using namespace sf;


std::vector<std::string> getFiles(std::string path)
{
    std::vector<std::string> res;

    std::string command = "dir " + path + "\\*.png >" + path + "\\list.txt /B";
    system(command.c_str());
    std::ifstream file;

    std::string filename = path + "\\list.txt";
    file.open(filename);
    while (!file.eof())
    {
        std::string str;
        file >> str;
        if(str != "")
            res.push_back(str);
    }
    file.close();
    std::remove(filename.c_str());
    return res;
}

std::vector<std::string> getForders()
{
    std::vector<std::string> res;
    std::string command = "dir tiles\\ /ad >tiles\\list.txt /B";
    system(command.c_str());
    std::ifstream file;
    file.open("tiles\\list.txt");
    while (!file.eof())
    {
        std::string str;
        file >> str;
        if (str != "")
            res.push_back(str);
    }
    file.close();
    std::remove("tiles\\list.txt");
    return res;
}
 
void keyboard()
{
    if (GetAsyncKeyState(27)) //esc выход
        exit(0);
}

int main()
{
    setlocale(LC_ALL, "Russian");

    srand(std::time(NULL));

    //---INIT SFML---//
    int windowX = 1280;
    int windowY = 720;
    unsigned style = Style::Default;

    RenderWindow window(VideoMode(windowX, windowY), "Wave Function Collapse 2D", style, ContextSettings());
    Event event;
    Vector2f MousePos;

    window.setFramerateLimit(60);
    //---INIT SFML END---//

    bool is_loop = false;

    Clock clock;
    float timer = 0;
    double delay = 0.2;

    Clock clock2;
    float timer2 = 0;
    double delay2 = 1.0;

    WaveFunctionCollapse2D map(32, 18);
    map.setIsDrawing(true, 40, 40);
    map.showLoadProgress(true);
    map.setRotations(true);
 
    //map.setSize(5, 3);
    //map.setIsDrawing(true, 240, 240);


    std::vector<std::string> forder_list = getForders();
    for (int i = 0; i < forder_list.size(); i++)
    {
        std::cout << i << ": " << forder_list[i] << std::endl;
    }

    int is_load = -1;

    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            keyboard();
        }

        if (is_load == 0)
        {
            std::cout << "input: ";
            int index;
            std::cin >> index;
            std::string forder = "tiles\\" + forder_list[index] + "\\";
            std::vector<std::string> file_list = getFiles(forder);
            for (int i = 0; i < file_list.size(); i++)
            {
                map.addTile(forder + file_list[i]);
            }

            map.create();
            is_load = 1;
        }
        if (is_load == -1)
            is_load = 0;
        if (window.hasFocus())
        {
            Vector2i pixelPos = Mouse::getPosition(window);//забираем коорд курсора
            MousePos = window.mapPixelToCoords(pixelPos);//переводим их в игровые (уходим от коорд окна)

            float time = clock.getElapsedTime().asSeconds();
            clock.restart();
            timer += time;
            if (Mouse::isButtonPressed(Mouse::Left) && timer > delay)
            {
                //map.clear();
                map.update();
                timer = 0;
            }

            if (Keyboard::isKeyPressed(Keyboard::Space) && timer > delay)
            {
                is_loop = !is_loop;
                timer = 0;
            }

            if (Keyboard::isKeyPressed(Keyboard::LAlt) && timer > delay)
            {
                map.clear();
                map.update();
                timer = 0;
            }

        }
        if (is_loop)
        {
            map.update();
            if (map.is_failed)
                map.clear();
        }

        window.clear();
        map.draw(window);
        window.display();

    }
}