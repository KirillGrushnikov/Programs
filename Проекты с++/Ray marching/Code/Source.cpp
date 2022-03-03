#include "SFML/Graphics.hpp"
#include <locale>
#include <time.h>
#include <Windows.h>
#include <iostream>

using namespace std;
using namespace sf;

void Zoom(Event& event, View& view)
{
    switch (event.type)
    {
        // ђращение средней кнопки ÷ колеса мыши
    case sf::Event::MouseWheelMoved:
        float zoom = 35;
        float r = view.getSize().x / view.getSize().y;
        // delta ÷ значение в какую сторону вращаем колесо ( 1 up , -1 down )
        if (event.mouseWheel.delta == 1)
        {
            //float Sy = view.getSize().y - zoom;
            float Sy = view.getSize().y * 0.9;
            if (Sy < 270)
                Sy = 270;
            float Sx = Sy * r;
            view.setSize(Sx, Sy);
        }
        if (event.mouseWheel.delta == -1)
        {
            //float Sy = view.getSize().y + zoom;
            float Sy = view.getSize().y * 1.1;
            if (Sy > 1665)
                Sy = 1665;
            float Sx = Sy * r;
            view.setSize(Sx, Sy);
        }
        break;
    }
}

void keyboard(Vector2f &Pos)
{
    float speed = 1;

    if (Keyboard::isKeyPressed(Keyboard::LShift))
        speed = 0.1;
    if (Keyboard::isKeyPressed(Keyboard::A))
        Pos.x -= speed;
    if (Keyboard::isKeyPressed(Keyboard::W))
        Pos.y -= speed;
    if (Keyboard::isKeyPressed(Keyboard::S))
        Pos.y += speed;
    if (Keyboard::isKeyPressed(Keyboard::D))
        Pos.x += speed;

    if (GetAsyncKeyState(27)) //esc выход
        exit(0);
}

Vector2f DiffMouse(Vector2f mouse, Vector2f lastPos)
{
    Vector2f diff(lastPos.x - mouse.x, lastPos.y - mouse.y);
    return diff;
}

int main()
{
    setlocale(LC_ALL, "Russian");
    srand(std::time(NULL));

    int windowX = 1280;
    int windowY = 720;
    unsigned style = 7;

    RenderWindow window(VideoMode(windowX, windowY), "Ray marching", style, ContextSettings());
    Event event;
    View view;
    Vector2f pos, lastPos;

    window.setFramerateLimit(60);
    view.setSize(windowX, windowY);

    Vector2f Wsize(windowX, windowY);

    Clock clock, clock2;
    float timer = 0;
    double delay = 0;

    Shader shader;
    RenderTexture rent;
    rent.create(Wsize.x, Wsize.y);
    Sprite s;
    s.setTexture(rent.getTexture());
    shader.loadFromFile("Shader\\frag.glsl", Shader::Fragment);

    shader.setUniform("u_resolution", Wsize);

    Vector2f Cpos(40, 0);
    Vector2f CamAngel(744, -197);
    Vector2f CamPos(0, 0);

    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            Zoom(event, view);
        }
        keyboard(Cpos);

        Vector2i pixelPos = Mouse::getPosition(window);//забираем коорд курсора
        pos = window.mapPixelToCoords(pixelPos);//переводим их в игровые (уходим от коорд окна)

        float time = clock.getElapsedTime().asSeconds();
        float u_time = clock2.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;
        if (timer > delay)
        { 
            if (GetAsyncKeyState(1))
                CamAngel += DiffMouse(pos, lastPos);
            if (GetAsyncKeyState(2))
                CamPos += DiffMouse(pos, lastPos);

            lastPos = pos;
            shader.setUniform("u_mouse", CamAngel);
            shader.setUniform("u_time", u_time);
            shader.setUniform("u_cube", Cpos); 
            timer = 0;
        }

        window.clear();
        rent.draw(s, &shader);
        window.draw(s);
        window.display();
    }
}