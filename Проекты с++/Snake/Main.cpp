#include "SFML/Graphics.hpp"
#include <iostream>
#include <windows.h>
#include <time.h>
#include <fstream>
#include <cstdlib>
#include <sstream>

using namespace sf;

const int width = 23; // 23
const int height = 41; // 41
int windowX = ((width + 15) * 16); //15 для мини карты
int windowY = (height - 1) * 16;
int size = 16, dir, num = 1, max_num = 0; //длина
int food = 50, food_real = 1;
int b_w = width / 2;
int b_h = height / 2;
int energy = 100;
char v;
float delay = 0.06; //speed чем меньше тем быстрее (0.06 норм)
int random_map = 1;

struct Snake
{
    int x, y;
}  s[1000];

struct Fruit
{
    int x, y;
} f;

bool gameover = TRUE;

// int neiro
/*
int rez[4];
int i, j;
double e = 2.718281828;
double x;
int n_f[11][11];
int n_b[11][11];
double sigmoid = 1 / pow(1 + e, -x);
*/

//переменные для нейронки
int n_dir = 0;
double e = 2.718281828;
double x;
double sigmoid = 1 / pow(1 + e, -x);

/*
void create()
{
    int blok_x, blok_y;
    int mini_map_nei[121];
    for (int i = 0; i < 121; i++)
    {
                blok_x = s[0].x - j;
                blok_y = s[0].y - i;
                if (blok_x == f.x && blok_y == f.y)
                {
                   
                }
    }
}

// нейронка
void neiro()
{
    if (n_dir == 1)
    {

    }
}
*/

void keyboard()
{

    if (GetAsyncKeyState(83) & 0x1 || Keyboard::isKeyPressed(Keyboard::Down)) // S
    {
        if(dir != 3)
            dir = 0;
        if (num == 1)
            dir = 0;
    }

    if (GetAsyncKeyState(65) & 0x1 || Keyboard::isKeyPressed(Keyboard::Left)) // A
    {
        if (dir != 2)
            dir = 1;
        if (num == 1)
            dir = 1;
    }

    if (GetAsyncKeyState(68) & 0x1 || Keyboard::isKeyPressed(Keyboard::Right)) // D
    {
        if (dir != 1)
            dir = 2;
        if(num == 1)
            dir = 2;
    }

    if (GetAsyncKeyState(87) & 0x1 || Keyboard::isKeyPressed(Keyboard::Up)) // W
    {
        if (dir != 0)
            dir = 3;
        if (num == 1)
            dir = 3;
    }

    if (GetAsyncKeyState(78) & 0x1) // N
    {
        if (n_dir == 0)
            n_dir = 1;
        else
            n_dir = 0;
    }

    if (GetAsyncKeyState(82) & 0x1) // R
    {
        v = 'r';
    }

    if (GetAsyncKeyState(27) & 0x1) //esc выход
        exit(0);
}

void spawn()
{
    int w = width - 2;
    int h = height - 3;
    if (random_map == 1)
    {
    Again:
        f.x = rand() % w + 1;
        f.y = rand() % h + 1;
        if (f.x == 7 && f.y >= 1 && f.y <= 9 || f.y == 9 && f.x >= 5 && f.x <= 6) // левая верхняя
            goto Again;
        if (f.x == width - 1 - 7 && f.y >= 1 && f.y <= 9 || f.y == 9 && f.x <= width - 1 - 5 && f.x >= width - 1 - 6) // правая верхняя
            goto Again;
        if (f.x == 7 && f.y <= height - 2 - 1 && f.y >= height - 2 - 9 || f.y == height - 2 - 9 && f.x >= 5 && f.x <= 6) // левая нижняя
            goto Again;
        if (f.x == width - 1 - 7 && f.y <= height - 2 - 1 && f.y >= height - 2 - 9 || f.y == height - 2 - 9 && f.x <= width - 1 - 5 && f.x >= width - 1 - 6) // правая нижняя
            goto Again;
        if (f.x == 11 && f.y <= height - 2 - 5 && f.y >= height - 2 - 7 || f.x == 11 && f.y >= 5 && f.y <= 7)//центральная вертикальная
            goto Again;
        if (f.y == 20 && f.x <= 9 && f.x >= 1 || f.y == 19 && f.x == 9 || f.y == 19 && f.x >= width - 1 - 9 && f.x <= width - 1 || f.y == 20 && f.x == width - 1 - 9)//центральная горизонтальные
            goto Again;
    }
    else
    {
        f.x = rand() % w + 1;
        f.y = rand() % h + 1;
    }
    for (int i = 1; i < num; i++)
    {
        if (f.x == s[i].x && f.y == s[i].y)
        {
            spawn();
        }
    }
}

void colision()
{
    if (random_map == 1)
    {
        if(s[0].x == 7 && s[0].y >= 1 && s[0].y <= 9 || s[0].y == 9 && s[0].x >= 5 && s[0].x <= 6) // левая верхняя
            delay = 31536000;
        if(s[0].x == width-1-7 && s[0].y >= 1 && s[0].y <= 9 || s[0].y == 9 && s[0].x <= width - 1 - 5 && s[0].x >= width - 1 - 6) // правая верхняя
            delay = 31536000;
        if (s[0].x == 7 && s[0].y <= height - 2 - 1 && s[0].y >= height - 2 - 9 || s[0].y == height - 2 - 9 && s[0].x >= 5 && s[0].x <= 6) // левая нижняя
            delay = 31536000;
        if (s[0].x == width - 1 - 7 && s[0].y <= height - 2 - 1 && s[0].y >= height - 2 - 9 || s[0].y == height - 2 - 9 && s[0].x <= width - 1 - 5 && s[0].x >= width - 1 - 6) // правая нижняя
            delay = 31536000;
        if(s[0].x == 11 && s[0].y <= height - 2 - 5 && s[0].y >= height - 2 - 7 || s[0].x == 11 && s[0].y >= 5 && s[0].y <= 7)//центральная вертикальная
            delay = 31536000;
        if (s[0].y == 20 && s[0].x <= 9 && s[0].x >= 1 || s[0].y == 19 && s[0].x == 9 || s[0].y == 19 && s[0].x >= width - 1 - 9 && s[0].x <= width - 1 || s[0].y == 20 && s[0].x == width - 1 - 9)//центральная горизонтальные
            delay = 31536000;
    }
}

void Tick()
{
    for (int i = num; i > 0; --i)
    {
        s[i].x = s[i - 1].x;
        s[i].y = s[i - 1].y;
    }

        if (dir == 0) s[0].y += 1;
        if (dir == 1) s[0].x -= 1;
        if (dir == 2) s[0].x += 1;
        if (dir == 3) s[0].y -= 1;

    //колизия
    colision();

    // закольцованный мир
    if (s[0].x == width - 1) s[0].x = 1;  if (s[0].x == 0) s[0].x = width - 2;
    if (s[0].y == height - 2) s[0].y = 1;  if (s[0].y == 0) s[0].y = height - 3;
    
    // не закольцованный мир
    /*
    if (s[0].x == width - 1 || s[0].x == 0 || s[0].y == height - 2 || s[0].y == 0)
    {
        delay = 31536000;
    }
    */

    if ((s[0].x == f.x) && (s[0].y == f.y))
    {
        food_real--;
        energy++;
        num++;
        spawn();
        if (num - 1 > max_num)
            max_num++;
    }

    for (int i = 1; i < num; i++)
        if (s[0].x == s[i].x && s[0].y == s[i].y)  num = i;

}

int main()
{
    srand(time(0));

    RenderWindow window(VideoMode(windowX, windowY), "Snake game");

    //random_map = rand()%5;

        Texture texture_map;
        texture_map.loadFromFile("Texture\\map.png");
        Sprite s0(texture_map);

        Texture texture_level_1;
        texture_level_1.loadFromFile("Texture\\level 1.png");
        Sprite l1(texture_level_1);

    Texture texture_body;
    texture_body.loadFromFile("Texture\\body.png");

    Texture texture_head;
    texture_head.loadFromFile("Texture\\Head.png");

    Texture texture_eat;
    texture_eat.loadFromFile("Texture\\Eat.png");

    Texture texture_blue;
    texture_blue.loadFromFile("Texture\\azure.png");

    Texture texture_minimap;
    texture_minimap.loadFromFile("Texture\\minimap.png");

    Sprite s2(texture_body);
    Sprite s1(texture_head);
    Sprite s3(texture_eat);
    Sprite s4(texture_minimap);
    Sprite s5(texture_blue);

    Font Myfont;
    Myfont.loadFromFile("Font\\a_AlternaNr.ttf");
    Text text("", Myfont, 30);
    Text max_score("", Myfont, 30);
    Text name;
    Text control;
    Text restart;
    Text exit;

    control.setFont(Myfont);
    control.setCharacterSize(30);
    control.setFillColor(Color::Black);
    control.setString("Control: W, A, S, D");
    control.setPosition(384, 302);

    restart.setFont(Myfont);
    restart.setCharacterSize(30);
    restart.setFillColor(Color::Black);
    restart.setString("Restart: R");
    restart.setPosition(384, 350);

    exit.setFont(Myfont);
    exit.setCharacterSize(30);
    exit.setFillColor(Color::Black);
    exit.setString("Exit: esc");
    exit.setPosition(384, 398);

    Font MyfontName;
    MyfontName.loadFromFile("Font\\Baskerville Bold Italic.ttf");
    name.setFont(MyfontName);
    name.setCharacterSize(20);
    name.setFillColor(Color::Blue);
    name.setString("Created: NeFFeX");
    name.setPosition(448, 608);

    Clock clock;
    float timer = 0;
    while (window.isOpen())
    {
        num = 1;
        s[0].y = 20;
        s[0].x = 11;
        if (random_map == 1)
        {
            Again:
            f.x = rand() % (width - 2) + 1;
            f.y = rand() % (height - 3) + 1;
            if (f.x == 7 && f.y >= 1 && f.y <= 9 || f.y == 9 && f.x >= 5 && f.x <= 6) // левая верхняя
                goto Again;
            if (f.x == width - 1 - 7 && f.y >= 1 && f.y <= 9 || f.y == 9 && f.x <= width - 1 - 5 && f.x >= width - 1 - 6) // правая верхняя
                goto Again;
            if (f.x == 7 && f.y <= height - 2 - 1 && f.y >= height - 2 - 9 || f.y == height - 2 - 9 && f.x >= 5 && f.x <= 6) // левая нижняя
                goto Again;
            if (f.x == width - 1 - 7 && f.y <= height - 2 - 1 && f.y >= height - 2 - 9 || f.y == height - 2 - 9 && f.x <= width - 1 - 5 && f.x >= width - 1 - 6) // правая нижняя
                goto Again;
            if (f.x == 11 && f.y <= height - 2 - 5 && f.y >= height - 2 - 7 || f.x == 11 && f.y >= 5 && f.y <= 7)//центральная вертикальная
                goto Again;
            if (f.y == 20 && f.x <= 9 && f.x >= 1 || f.y == 19 && f.x == 9 || f.y == 19 && f.x >= width - 1 - 9 && f.x <= width - 1 || f.y == 20 && f.x == width - 1 - 9)//центральная горизонтальные
                goto Again;
        }
        else
        {
            f.x = rand() % (width - 2) + 1;
            f.y = rand() % (height - 3) + 1;
        }
        v = '0';
        delay = 0.08; //0.06(быстро)
        if(dir == 1 || dir == 3)
        dir = 0;
        else
        dir = 3;
        while (v != 'r' && window.isOpen())
        {
            std::ostringstream NUM;
            std::ostringstream MAX_NUM;
            NUM << num - 1;
            MAX_NUM << max_num;
            text.setFillColor(Color::Red);
            text.setString("Score: " + NUM.str()); //text
            text.setPosition(384, 232);
            text.setStyle(Text::Bold);

            max_score.setFillColor(Color::Red);
            max_score.setString("Max Score: " + MAX_NUM.str()); //max_score
            max_score.setPosition(384, 264);
            max_score.setStyle(Text::Bold);
            float time = clock.getElapsedTime().asSeconds();
            clock.restart();
            timer += time;

            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            keyboard();

            if (timer > delay) { timer = 0; Tick(); }
            window.clear();

            View viev(FloatRect(0, 0, windowX, windowY));
            View minimap(FloatRect(0, 0, 176, 176));
            viev.setViewport(FloatRect(0, 0, 1, 1));
            minimap.setCenter(8 + s[0].x * size, 8 + s[0].y * size);
            minimap.setViewport(FloatRect(0.658f, 0.05f, 0.29f, 0.275f));

            window.setView(viev);

            if (random_map == 1)
            {
                l1.setPosition(0, 0);
                window.draw(l1);
            }
            else
            {
                s0.setPosition(0, 0);
                window.draw(s0);
            }
            s4.setPosition(368, 0);
            window.draw(s4);

            s1.setPosition(s[0].x * size, s[0].y * size);
            window.draw(s1);

            for (int i = 1; i < num; i++)
            {
                s2.setPosition(s[i].x * size, s[i].y * size);
                window.draw(s2);
            }

            s3.setPosition(f.x * size, f.y * size);

            window.draw(s3);

            window.draw(text);
            window.draw(max_score);
            window.draw(name);
            window.draw(control);
            window.draw(restart);
            window.draw(exit);

            window.setView(minimap);

            //отрисовка карты
            if (random_map == 1)
            {
                l1.setPosition(0, 0);
                window.draw(l1);
            }
            else
            {
                s0.setPosition(0, 0);
                window.draw(s0);
            }

            s1.setPosition(s[0].x * size, s[0].y * size);
            window.draw(s1);

            for (int i = 1; i < num; i++)
            {
                s2.setPosition(s[i].x * size, s[i].y * size);
                window.draw(s2);
            }

            s3.setPosition(f.x * size, f.y * size);

            window.draw(s3);

            window.display();
        }
    }
    return 0;
}

