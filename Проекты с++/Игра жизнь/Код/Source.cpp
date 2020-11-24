#include "SFML/Graphics.hpp"
#include <iostream>
#include <windows.h>
#include <time.h>

using namespace sf;

// переменные для создания окна
const int g_Size = 16;
const int windowX = 1024;
const int windowY = 704;
const int mapX = windowX / g_Size;
const int mapY = windowY / g_Size;

RenderWindow window(VideoMode(windowX, windowY), "Live game");

//необходимые переменные и масивы
float Limit_time = 999999999999;
bool space = false;
bool alt = true;
RectangleShape Square[mapX*mapY];
char map[mapX][mapY];
int map_sch[mapX][mapY];
Color c_block = {30, 30, 30};
Color c_map = { 0, 128, 0 };

//функция создания карты
void Set_map()
{
    Vector2f vec(g_Size, g_Size);
    int k = 0;
    for (int i = 0; i < mapX; i++)
        for (int j = 0; j < mapY; j++)
        {
            map[i][j] = '0';
            Vector2f position(i * g_Size, j * g_Size);
            Square[k].setSize(vec);
            Square[k].setPosition(position);
            Square[k].setFillColor(c_map);
            Square[k].setOutlineColor(Color::Black);
            Square[k].setOutlineThickness(1);
            k++;
        }
}

//главная фунция в которой прописана локика игры
void Tick()
{
    int ways[8][2] = { -1, -1,   -1, 0,   -1, 1,   0, -1,   0, 1,   1, -1,   1, 0,   1, 1 };
    for (int i = 0; i < mapX; i++)
        for (int j = 0; j < mapY; j++)
        {
            map_sch[i][j] = 0;
            for (int w = 0; w < 8; w++)
            {
            if (map[i + ways[w][0]][j + ways[w][1]] == '1' && i + ways[w][0] >= 0 && i + ways[w][0] < mapX && j + ways[w][1] >= 0 && j + ways[w][1] < mapY)
                map_sch[i][j] += 1;
            if (i + ways[w][0] < 0 && map[mapX + ways[w][0]][j + ways[w][1]] == '1')
                map_sch[i][j] += 1;
            if (i + ways[w][0] == mapX && map[0][j + ways[w][1]] == '1')
                map_sch[i][j] += 1;
            if (j + ways[w][1] < 0 && map[i + ways[w][0]][mapY + ways[w][1]] == '1')
                map_sch[i][j] += 1;
            if (j + ways[w][1] == mapY && map[i + ways[w][0]][0] == '1')
                map_sch[i][j] += 1;
            }
        }

    for (int i = 0; i < mapX; i++)
        for (int j = 0; j < mapY; j++)
        {
            if (map_sch[i][j] == 3)
            {
                Square[i * mapY + j].setFillColor(c_block);
                map[i][j] = '1';
            }
            if (map_sch[i][j] > 3 || map_sch[i][j] < 2)
            {
                Square[i * mapY + j].setFillColor(c_map);
                map[i][j] = '0';
            }
        }
}

// фукция обновления карты
void update()
{
    for (int i = 0; i < mapX; i++)
        for (int j = 0; j < mapY; j++)
        {
            if (map[i][j] == '1')
                Square[i * mapY + j].setFillColor(c_block);
            else
                Square[i * mapY + j].setFillColor(c_map);
        }
}

void keyboad()
{
    if (GetAsyncKeyState(1) & 0x1) //событие нажатия левой кнопки мыши
    {
        Vector2i pixelPos = Mouse::getPosition(window);//забираем коорд курсора
        Vector2f pos = window.mapPixelToCoords(pixelPos);//переводим их в игровые (уходим от коорд окна)

        for (int i = 0; i < mapX; i++)
            for (int j = 0; j < mapY; j++)
                if (pos.x < Square[i * mapY + j].getPosition().x + g_Size && pos.x > Square[i * mapY + j].getPosition().x && pos.y < Square[i * mapY + j].getPosition().y + g_Size && pos.y > Square[i * mapY + j].getPosition().y)
                    if (Square[i * mapY + j].getFillColor() != c_block)
                    {
                        Square[i * mapY + j].setFillColor(c_block);
                        map[i][j] = '1';
                    }
                    else
                    {
                        Square[i * mapY + j].setFillColor(c_map);
                        map[i][j] = '0';
                    }
    }

    if(GetAsyncKeyState(32) & 0x1) //событие нажатия space
    {
        if (space)
        {
            Limit_time = 999999999999;
            space = false;
        }
        else
        {
            Limit_time = 0.1;
            space = true;
        }
    }

    if (GetAsyncKeyState(17) & 0x1) //событие нажатия ctr
    {
        Tick();
    }

    if (GetAsyncKeyState(18) & 0x1) //событие нажатия alt
    {
        if (alt)
        {
            for (int i = 0; i < mapX; i++)
                for (int j = 0; j < mapY; j++)
                    if (rand() % 100 < 30)
                        map[i][j] = '1';
            alt = false;
        }
        else
        {
            for (int i = 0; i < mapX; i++)
                for (int j = 0; j < mapY; j++)
                        map[i][j] = '0';
            alt = true;
        }

        update();
    }
}

int main()
{
    srand(time(0));

    Clock clock;
    float timer = 0;
    Event event;

    Set_map(); // создаём карту

    //главный цикл
    while (window.isOpen())
    {
        while (window.pollEvent(event)) //событие закрытие окна
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        keyboad(); // функция считывание клавиатуры

        float time = clock.getElapsedTime().asSeconds();

        timer += time;
        if (timer > Limit_time) { timer = 0; Tick(); } //основная функция тик срабатывает каждые Limit_time = 0.1 секунд
        clock.restart();

        window.clear(); // очистка окна

        for(int i = 0 ; i < mapX * mapY; i++)
            window.draw(Square[i]);

        window.display(); // вывод на экран
    }

    return 0;
}