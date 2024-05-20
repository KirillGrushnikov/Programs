#include <iostream>
#include "SFML/Graphics.hpp"
#include <locale>
#include <time.h>
#include <Windows.h>
#include <iostream>
#include <complex>
#include <vector>

using namespace std;
using namespace sf;
float sch = -0.74543, sch2 = -0.11301, schSpeed = 0.01;
int schTag = -1;
bool grad = false;
void keyboard(float time, float& posx, float& posy, float& speed, float& zoom, float& scale, float& max_iter, int height)
{
    if (GetAsyncKeyState(27)) //esc выход
        exit(0);
    if (Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::Down))
    {
        float inv_scale = 2 - scale;
        if (Keyboard::isKeyPressed(Keyboard::Up))
        {
            zoom *= scale;
            speed *= scale;
        }
        if (Keyboard::isKeyPressed(Keyboard::Down))
        {
            zoom *= inv_scale;
            speed *= inv_scale;
        }
    }
    if (Keyboard::isKeyPressed(Keyboard::Right))
    {
        max_iter += 1;
    }
    if (Keyboard::isKeyPressed(Keyboard::Left))
    {
        max_iter -= 1;
    }
    max_iter = min(max((int)max_iter, 2), 5500);

    if (Keyboard::isKeyPressed(Keyboard::W))
    {
        posy += speed * time;
    }
    if (Keyboard::isKeyPressed(Keyboard::S))
    {
        posy -= speed * time;
    }
    if (Keyboard::isKeyPressed(Keyboard::D))
    {
        posx -= speed * time;
    }
    if (Keyboard::isKeyPressed(Keyboard::A))
    {
        posx += speed * time;
    }

    if (Keyboard::isKeyPressed(Keyboard::Q))
    {
        if(!schTag)
            sch-=schSpeed;
        sch2 -= schSpeed;
    }

    if (Keyboard::isKeyPressed(Keyboard::E))
    {
        if (!schTag)
            sch += schSpeed;
        sch2 += schSpeed;
    }

    if (Keyboard::isKeyPressed(Keyboard::Space))
    {
        max_iter = 30;
        posx = 0;
        posy = 0;
        speed = 5;
        scale = 0.993;
        zoom = 2.5 / height;
        sch = -0.74543;
        sch2 = -0.11301;
    }


    if (Keyboard::isKeyPressed(Keyboard::LShift))
        schSpeed = 0.001;
    else
        schSpeed = 0.01;

    schTag = -1;
    if (Mouse::isButtonPressed(Mouse::Left))
    {
        schTag = 0;
    }
    if (Mouse::isButtonPressed(Mouse::Right))
    {
        schTag = 1;
    }

    if (Keyboard::isKeyPressed(Keyboard::Num1))
        grad = false;
    if (Keyboard::isKeyPressed(Keyboard::Num2))
        grad = true;
}

void Zoom(Event& event, float& speed, float& zoom, float& scale)
{
    switch (event.type)
    {
        // ђращение средней кнопки ÷ колеса мыши
    case sf::Event::MouseWheelMoved:
        // delta ÷ значение в какую сторону вращаем колесо ( 1 up , -1 down )
        float inv_scale = 2 - scale;
        if (event.mouseWheel.delta == 1)
        {
            if (schTag == -1)
            {
                zoom *= scale;
                speed *= scale;
            }
            if (schTag == 0)
                sch += schSpeed;
            if (schTag == 1)
                sch2 += schSpeed;
        }
        if (event.mouseWheel.delta == -1)
        {
            if (schTag == -1)
            {
                zoom *= inv_scale;
                speed *= inv_scale;
            }
            if (schTag == 0)
                sch -= schSpeed;
            if (schTag == 1)
                sch2 -= schSpeed;
        }
        break;
    }
}

std::string GetFrameRate()
{
    float fps;
    static Clock clock;
    static sf::Time previousTime = clock.getElapsedTime();
    static sf::Time currentTime;
    currentTime = clock.getElapsedTime();
    fps = 1.0f / (currentTime.asSeconds() - previousTime.asSeconds()); // the asSeconds returns a float
    previousTime = currentTime;
    return std::to_string((int)fps);
}

inline void maldebrot(int w, int h, float* offset, float zoom, int max_iter, float posx, float posy, Image IMgradient, Image& IMdraw)
{
    for (int x = 0; x < w; x++)
    {
        for (int y = 0; y < h; y++)
        {
            complex<float> c((x - offset[0]) * zoom - posx, (y - offset[1]) * zoom - posy);
            complex<float> z = 0;
            int num_iter = 0;
            for (int i = 0; i < max_iter; i++)
            {
                z = pow(z, 2) + c;
                if (pow(z.real(), 2) + pow(z.imag(), 2) > 4)
                    break;
                num_iter++;
            }
            int colx = ((IMgradient.getSize().x - 1) * num_iter / max_iter);
            int coly = ((IMgradient.getSize().y - 1) * num_iter / max_iter);
            Color color = IMgradient.getPixel(colx, coly);
            IMdraw.setPixel(x, y, color);
        }
    }
}

inline void Julea(int w, int h, float* offset, float zoom, int max_iter, float posx, float posy, Vector2f C, Image IMgradient, Image& IMdraw)
{
    for (int x = 0; x < w; x++)
    {
        for (int y = 0; y < h; y++)
        {
            complex<float> c(C.x, C.y);
            complex<float> z((x - offset[0]) * zoom - posx, (y - offset[1]) * zoom - posy);
            int num_iter = 0;
            for (int i = 0; i < max_iter; i++)
            {
                z = pow(z, 2) + c;
                if (pow(z.real(), 2) + pow(z.imag(), 2) > 4)
                    break;
                num_iter++;
            }
            int colx = ((IMgradient.getSize().x - 1) * num_iter / max_iter);
            int coly = ((IMgradient.getSize().y - 1) * num_iter / max_iter);
            Color color = IMgradient.getPixel(colx, coly);
            IMdraw.setPixel(x, y, color);
        }
    }
}

int main()
{
    setlocale(LC_ALL, "Russian");
    srand(std::time(NULL));

    int windowX = 1920;
    int windowY = 1080;
    unsigned style = 7;

    RenderWindow window(VideoMode(windowX, windowY), "", style, ContextSettings());
    Event event;
    View view;
    Vector2f pos;
    //view.setSize(windowX, windowY);

    int wigth = windowX; 
    int height = windowY; 
    float max_iter = 30;
    float zoom = 2.5 / height; //2.5 / height
    float offset[2] = { 0.5 * wigth, 0.5 * height };
    float posx = 0, posy = 0, speed = 5, scale = 0.993;

    view.setCenter(wigth / 2, height / 2);
    view.setSize(wigth, height);

    Image image, gradient, gradient2;
    gradient.loadFromFile("gradient.png");
    gradient2.loadFromFile("gradient2.png");
    image.create(wigth, height);

    Texture texture, Tgradient, Tgradient2;
    Tgradient.loadFromImage(gradient);
    Tgradient2.loadFromImage(gradient2);
    Sprite sprite;

    Clock clock;
    float timer = 0;
    double delay = 0.02;
    
    Shader shader;
    RenderTexture RenT;
    RenT.create(wigth, height);

    Sprite s;
    s.setTexture(RenT.getTexture());
    s.setColor(Color(255, 255, 255));

    //shader.loadFromFile("ShaderMal.frag", Shader::Fragment);
    shader.loadFromFile("ShaderMal.frag", Shader::Fragment);
    //maldebrot(wigth, height, offset, zoom, max_iter, posx, posy, gradient, image);

    //Julea(wigth, height, offset, zoom, max_iter, posx, posy, Vector2f(0, 0), gradient, image);

    std::string fps = "0";

    while (window.isOpen())
    {
        keyboard(clock.getElapsedTime().asSeconds(), posx, posy, speed, zoom, scale, max_iter, height);
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            //ShowCursor(false);
            Zoom(event, speed, zoom, scale);
        }


        Vector2i pixelPos = Mouse::getPosition(window);//забираем коорд курсора
        pos = window.mapPixelToCoords(pixelPos);//переводим их в игровые (уходим от коорд окна)
        
        shader.setUniform("u_pos", Vector2f(posx, posy));
        shader.setUniform("u_offset", Vector2f(offset[0], offset[1]));
        shader.setUniform("u_max_iter", max_iter);
        shader.setUniform("u_zoom", zoom);
        shader.setUniform("u_C", Vector2f(sch, sch2));
        if(!grad)
            shader.setUniform("u_gradient", Tgradient);
        else
            shader.setUniform("u_gradient", Tgradient2);
        
        //maldebrot(wigth, height, offset, zoom, max_iter, posx, posy, gradient, image);

        fps = GetFrameRate();
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;
        if (timer > delay) { timer = 0; window.setTitle(fps); }

        window.setView(view);
        texture.loadFromImage(image);
        sprite.setTexture(texture);

        window.clear();
        
        RenT.draw(s, &shader);
        window.draw(s);
        
        //window.draw(sprite);
        window.display();
    }
}
