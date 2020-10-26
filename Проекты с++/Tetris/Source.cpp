#include "SFML/Graphics.hpp"
#include <iostream>
#include <time.h>
#include <Windows.h>
#include <sstream>
#include <fstream>
#include <cmath>
#include <conio.h>

using namespace sf;
using namespace std;

//variables window and map
int windowX = 700;
int windowY = 700;
const int mapX = 10; //320
const int mapY = 21; //640
RenderWindow window(VideoMode(windowX, windowY), "Tetris");

//variables
int sch_spoiler_block = rand() % 7;
int sch_blocks;
int color_spoiler_block = rand() % 8 + 1;
int color_blocks;
double delay = 0.3;
int Score = 0;
bool gameover = false;
bool Enter = false;
char v;

//matrix map
int game_map[mapY][mapX] = { 0 };

//bloks
int figures[7][4] =
{
    1,3,5,7, // I
    2,4,5,7, // S
    3,5,4,6, // Z
    3,5,4,7, // T
    2,3,5,7, // L
    3,5,7,6, // J
    2,3,4,5, // O
};

struct Point
{
    int x, y;
} a[4], b[4];

struct PointS
{
    int x, y;
} a_s[4], b_s[4];

//create texture
Texture texture;
Texture texture_s;

//Create sprite
Sprite sprite(texture);
Sprite sprite_s(texture);
Sprite back_sprite(texture_s);

//строки в корорых хранятся имена и рекорды
string g_name;
const int sch_names = 21;
string g_names[sch_names];
string g_Scores[sch_names];

Text Names_Score[sch_names];

//Spawn bloks
void Create_bloks()
{
    color_blocks = color_spoiler_block;

color_set_again:
    int color = rand() % 8 + 1;
    if (color == color_spoiler_block)
        goto color_set_again;
    else
        color_spoiler_block = color;

    sch_blocks = sch_spoiler_block;
    sch_spoiler_block = rand() % 7;

    for (int i = 0; i < 4; i++)
    {
        a_s[i].x = figures[sch_spoiler_block][i] % 2;
        a_s[i].y = figures[sch_spoiler_block][i] / 2;
        a[i].x = figures[sch_blocks][i] % 2;
        a[i].y = figures[sch_blocks][i] / 2;
    }

    for (int i = 0; i < 4; i++)
    {
        a[i].x += 4;
        a[i].y -= 4;
    }
}

//drawing bloks
void draw_blocks()
{
    for (int i = 0; i < 4; i++)
    {
        sprite.setPosition(190 + a[i].x * 32, -7 + a[i].y * 32);
        window.draw(sprite);
    }
    for (int i = 0; i < mapY; i++)
        for (int j = 0; j < mapX; j++)
        {
            if (game_map[i][j] == 0) continue;
            sprite.setTextureRect(IntRect(game_map[i][j] * 32, 0, 32, 32));
            sprite.setPosition(j * 32, i * 32);
            sprite.move(190, -7); //Смещение
            window.draw(sprite);
        }
    sprite.setTextureRect(IntRect(32 * color_blocks, 0, 32, 32));
}

//colision bloks
bool colision()
{
    for (int i = 0; i < 4; i++)
        if (a[i].x < 0 || a[i].x >= mapX || a[i].y >= mapY) return 0;
        else if (game_map[a[i].y][a[i].x]) return 0;
    return 1;
}

//clear line
void chek_line()
{
    int k = mapY - 1;
    for (int i = mapY - 1; i > 0; i--)
    {
        int count = 0;
        for (int j = 0; j < mapX; j++)
        {
            if (game_map[i][j]) count++;
            game_map[k][j] = game_map[i][j];
        }
        if (count < mapX) k--;
        else Score += 1;
    }
}

//reeding keyboard
void keyboard()
{
    int move;
    if (GetAsyncKeyState(65) & 0x1) // A
    {
        move = -1;
        for (int i = 0; i < 4; i++) {
            b[i] = a[i];
            a[i].x += move;
        }

        if (!colision()) {
            for (int i = 0; i < 4; i++)
                a[i] = b[i];
        }
    }

    if (GetAsyncKeyState(68) & 0x1) // D
    {
        move = 1;
        for (int i = 0; i < 4; i++) {
            b[i] = a[i];
            a[i].x += move;
        }

        if (!colision()) {
            for (int i = 0; i < 4; i++)
                a[i] = b[i];
        }
    }

    if (GetAsyncKeyState(83) & 0x1) // S
    {
        delay = 0.05;

        if (!colision())
        {
            for (int i = 0; i < 4; i++)
                a[i] = b[i];
        }
    }

    if (GetAsyncKeyState(32) & 0x1) // Space
    {
        if (sch_blocks == 0)
        {
            Point p = {a[1].x, a[2].y};
            for (int i = 0; i < 4; i++)
            {
                int x = a[i].y - p.y; // y - y0
                int y = a[i].x - p.x; // x - x0
                a[i].x = p.x - x;
                a[i].y = p.y + y;
            }
        }
        else
            if (sch_blocks == 6) {}
            else
            {
                Point p = a[1];
                for (int i = 0; i < 4; i++)
                {
                    int x = a[i].y - p.y; // y - y0
                    int y = a[i].x - p.x; // x - x0
                    a[i].x = p.x - x;
                    a[i].y = p.y + y;
                }
            }
            if (!colision())
            {
                for (int i = 0; i < 4; i++)
                    a[i] = b[i];
            }
    }

    if (GetAsyncKeyState(82) & 0x1) // R
    {
        v = 'r';
    }


    if (GetAsyncKeyState(27) & 0x1) //esc выход
        exit(0);
}

//reeding keyboard
void keyboard_nik()
{
    int temp;
    string letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (int i = 65; i < 91; i++)
        if (GetAsyncKeyState(i) & 0x1) // A
        {
            temp = i - 65;
            if (g_name.size() < 15)
                g_name += letters[temp];
        }

    if (GetAsyncKeyState(8) & 0x1) // <---
    {
        if (g_name.size() >= 1)
            g_name.erase(g_name.size() - 1);
    }
}

//draw map(background, map)
void Draw_map()
{
    RectangleShape background;
    RectangleShape map;
    RectangleShape leaderboard;
    Vector2f l_Size(170, 640);
    Vector2f m_Size(320, 640);
    Vector2f v_wall(700, 700);

    background.setFillColor(Color::Color(30, 0, 71));
    background.setPosition(0, 0);
    background.setSize(v_wall);

    leaderboard.setFillColor(Color::Color(75, 0, 130));
    leaderboard.setPosition(10, 25);
    leaderboard.setSize(l_Size);
    leaderboard.setOutlineColor(Color::Black);
    leaderboard.setOutlineThickness(2);

    map.setFillColor(Color::Color(200, 200, 200));
    map.setPosition(190, 25);
    map.setSize(m_Size);

    window.draw(background);
    window.draw(leaderboard);
    window.draw(map);
}

void draw_roof()
{
    RectangleShape roof;
    Vector2f v_roof(320, 25);
    roof.setFillColor(Color::Color(30, 0, 71));
    roof.setPosition(190, 0);
    roof.setSize(v_roof);

    window.draw(roof);
}

//drawing next bloks
void draw_spoiler()
{
    RectangleShape wall;
    RectangleShape minimap;
    Vector2f mm_Size(32*4 - 4, 32*5 - 4);
    Vector2f mw_Size(32*4, 32 * 5);

    wall.setFillColor(Color::Black);
    wall.setPosition(542, 25);
    wall.setSize(mw_Size);
    minimap.setFillColor(Color::Color(75, 0, 130));
    minimap.setPosition(544, 27);
    minimap.setSize(mm_Size);

    window.draw(wall);
    window.draw(minimap);
    sprite_s.setTextureRect(IntRect(32 * color_spoiler_block, 0, 32, 32));
    back_sprite.setTextureRect(IntRect(0, 0, 34, 34));
    for (int i = 0; i < 4; i++)
    {
        if (sch_spoiler_block == 0)
        {
            sprite_s.setPosition(558 + a_s[i].x * 32, 40 + a_s[i].y * 32);
            back_sprite.setPosition(557 + a_s[i].x * 32, 39 + a_s[i].y * 32);
        }
        else
            if (sch_spoiler_block == 6)
            {
                sprite_s.setPosition(574 + a_s[i].x * 32, (25 + 25 / 2) + a_s[i].y * 32);
                back_sprite.setPosition(573 + a_s[i].x * 32, (24 + 25 / 2) + a_s[i].y * 32);
            }
            else
            {
                sprite_s.setPosition(574 + a_s[i].x * 32, 25 + a_s[i].y * 32);
                back_sprite.setPosition(573 + a_s[i].x * 32, 24 + a_s[i].y * 32);
            }
        window.draw(back_sprite);
        window.draw(sprite_s);
    }
}

//drawing grid
void draw_line()
{
    RectangleShape line[2][21];

    Vector2f Lv_Size(1, 640);
    Vector2f Lg_Size(320, 1);

    for (int i = 0; i < 11; i++)
    {
        line[0][i].setSize(Lv_Size);
        line[0][i].setPosition(190 + (i * 32), 25);
    }
    for (int j = 0; j < 21; j++)
    {
        line[1][j].setSize(Lg_Size);
        line[1][j].setPosition(190, 25 + (j * 32));
    }

    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 21; j++)
        {
            line[i][j].setFillColor(Color::Black);
            window.draw(line[i][j]);
        }
}

//Вызываемая функция при рестарте
void Restart()
{
    for(int i = -8; i < mapY; i++)
        for (int j = 0; j < mapX; j++)
            game_map[i][j] = 0;

    Create_bloks();

    Score = 0;

    gameover = false;
    Enter = false;
    delay = 0.3;
    v = '0';
}

//функция шифрофки и дешифровки
string cipher(string text)
{
    //Алфовит
    string letters = "abcdefghijklmnopqrstuvwxyz";
    string Bletters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string number = "0123456789";
    string sign = "*%^:#$@&!?";

    string secret;

    for (int i = 0; i < text.size(); i++)
        for (int j = 0; j < 27; j++)
        {
            if (letters[j] == text[i])
                secret += letters[letters.size() - 1 - j];
            if (Bletters[j] == text[i])
                secret += Bletters[Bletters.size() - 1 - j];
        }

    for (int i = 0; i < text.size(); i++)
        for (int j = 0; j < 11; j++)
        {
            if(sign[j] == text[i])
                secret += number[number.size() - 1 - j];
            if (number[j] == text[i])
                secret += sign[sign.size() - 1 - j];
        }

    return secret;
}

//отрисовка gameover окна
void draw_gameover_window()
{
    RectangleShape window;
    Vector2f w_Size(192, 70);
    RectangleShape str_line;
    Vector2f str_Size(172, 30);

    window.setSize(w_Size);
    window.setFillColor(Color::Color(0, 0, 255));
    window.setPosition(254, 355);
    window.setOutlineColor(Color::Black);
    window.setOutlineThickness(1);

    str_line.setSize(str_Size);
    str_line.setFillColor(Color::Color(150, 150, 150));
    str_line.setPosition(264, 365);
    str_line.setOutlineColor(Color::Black);
    str_line.setOutlineThickness(1);

    ::window.draw(window);
    ::window.draw(str_line);
}

//считывание из файла
void Reed_file()
{
    string Sign;
    ifstream Log;
    Log.open("Log.txt");
    Log >> Sign; //Sing это зашифрованное значение количества имён и рекордов
    Log.close();

    int sch_line = 0;
    string buff;
    int i = 0;
    ifstream file;
    if (Sign == '!') //Если Sing равен одному(т.е там только одно имя)
    {
        file.open("config.txt"); // открыли файл для чтения
        for (file >> buff; !file.eof(); file >> buff)
        {
            sch_line++;
            if (sch_line == 9) //доходим до 9 строчки т.к с 1 по 8 мусор
            {
                g_names[0] = cipher(buff);
            }
        }
        file.close();
        sch_line = 0;
        file.open("Log.txt"); // открыли файл для чтения
        for (file >> buff; !file.eof(); file >> buff)
        {
            sch_line++;
            if (sch_line == 9) //доходим до 9 строчки т.к с 1 по 8 мусор
            {
                g_Scores[0] = cipher(buff);
            }
        }
        file.close();
    }
    else
    {
        int sch = 0;
        for(int i = 0; i < 15; i++)
            sch += Sign[i] * (10, Sign.size() - 1 - i);

        for (file >> buff; !file.eof(); file >> buff)
        {
            sch_line++;
            if (sch_line > 8 && sch_line <= sch_line + sch) //читаем с 9 sch слов (sch количество имён(расшифрованное))
            {
                g_names[i] = cipher(buff);// напечатали это слово
                i++;
            }
        }
        file.close(); // закрываем файл

        file.open("Log.txt"); // открыли файл для чтения
        for (file >> buff; !file.eof(); file >> buff)
        {
            sch_line++;
            if (sch_line > 8 && sch_line <= sch_line + sch)  //читаем с 9 sch слов (sch количество имён(расшифрованное))
            {
                g_Scores[i] = cipher(buff);// напечатали это слово
                i++;
            }
        }
        file.close(); // закрываем файл
    }
}

//сортировка
void Sort_Score(int Score_new, string names_new, int sch)
{
    int sch_line = 0;
    ifstream file;
    string Numbers;
    int i = 0;
    file.open("Log.txt");
        for (file >> Numbers; !file.eof(); file >> Numbers)
        {
            sch_line++;
            if (sch_line > 8)
            {
                g_Scores[i] = Numbers;// напечатали это слово
                i++;
            }
        }

        g_Scores[i] = cipher(g_Scores[i]);

        int Scores_i[sch_names];

        for (int i = 0; i < sch; i++)
        {
            for (int j = 0; j < g_Scores[i].size(); j++)
            {
                Scores_i[i] += g_Scores[i][j] * (10, g_Scores[i].size() - 1 - j);
            }
            if (Score_new >= Scores_i[i])
            {
                for (int z = 20; z > i; z--)
                {
                    Scores_i[z] = Scores_i[z--];
                    g_names[z] = g_names[z--];
                }
                Scores_i[i] = Score_new;
                g_names[i] = names_new;
            }
            else
            {
                if (sch < 21)
                {
                    Scores_i[sch] = Score_new;
                    g_names[sch] = names_new;
                }
            }
        }
        for (int i = 0; i < sch; i++)
        {
            ostringstream NUM;
            NUM << Scores_i[i];
            g_Scores[i] = NUM.str();
        }
}

//запись в файл (text это новое имя)
void Write_file(string text)
{
    string w_letters = "abcdefghijklmnopqrstuvwxyz";
    string w_sign = "*%^:#$@&!?";
    string w_number = "0123456789";
    int spam = 8;
    char text_spam[25][15];
    string W_Sign;
    string buff;
    ifstream file;
    ifstream Log;
    Log.open("Log.txt");
    Log >> W_Sign;
        string new_sign = cipher(W_Sign); //Записываем в new_sing Расшифрованное колличество имён
        int num_sign = 0;
        for (int i = 0; i < new_sign.size(); i++)
        {
            if (w_number[i] == new_sign[i])
                num_sign += i * pow(10, new_sign.size() - 1 - i); //перевод new_sing в num_sing(из текста в int)
        }
    Log.close();
        file.open("config.txt"); // открыли файл для чтения
        int i = 0;
        for (file >> buff; !file.eof(); file >> buff)
        {
            spam++;
            if (spam > 8 && spam < spam + num_sign) //Записываем num_sing имён
            {
                g_names[i] = buff;  //сохранили это слово
                i++;
            }
        }
        file.close(); // закрываем файл

        ofstream Log_out("Log.txt", ios_base::trunc);
        ofstream file_out("config.txt", ios_base::trunc); // открыли файл для записи очистили и записали спам
        for (int i = 0; i < spam; i++)
        {
            for (int j = 0; j <( rand() % 10 + 4); j++)
                text_spam[i][j] = w_letters[rand() % 26];
            file_out << text_spam[i];
        }

        ostringstream NUM;
        NUM << num_sign + 1;
        Log_out << NUM.str();

        for (int i = 0; i < spam - 1; i++)
        {
            for (int j = 0; j < rand() % 10; j++)
                text_spam[i][j] = w_sign[rand() % 10];
            Log_out << text_spam[i];
        }

        Sort_Score(Score, text, num_sign);

        for (int i = 0; i < sch_names; i++) //ввели в файл имена
        {
            file_out << cipher(g_names[i]);
            Log_out << cipher(g_Scores[i]);
        }

        for (int i = 0; i < 25; i++) // снова добовляем спам
        {
            for (int j = 0; j < (rand() % 10 + 4); j++)
                text_spam[i][j] = w_letters[rand() % 26];
            file_out << text_spam[i];
        }

        for (int i = 0; i < 25; i++) // снова добовляем спам
        {
            for (int j = 0; j < rand() % 10; j++)
                text_spam[i][j] = w_sign[rand() % 10];
            Log_out << text_spam[i];
        }

        file_out.close(); // закрываем файлы
        Log_out.close();
}

void draw_names_Score()
{
    for (int i = 0; i < sch_names; i++)
    {
        window.draw(Names_Score[i]);
    }
}

int main()
{
    srand(time(0));

    //загружаем шрифт
    Font Myfont;
    Myfont.loadFromFile("Font\\a_AlternaNr.ttf");
    Text t_Score("", Myfont, 20);

    Text GameOver("Game Over", Myfont, 100);
    GameOver.setPosition(180, 250);
    GameOver.setFillColor(Color::Red);
    GameOver.setOutlineColor(Color::Black);
    GameOver.setOutlineThickness(1);

    Text NAME("", Myfont, 26);
    NAME.setPosition(265, 364);
    NAME.setFillColor(Color::Black);

    //draw poisoning
    const int word = 6;
    Text Poisoning[word];
    for (int i = 0; i < word; i++)
    {
        Poisoning[i].setFont(Myfont);
        Poisoning[i].setCharacterSize(30);
        Poisoning[i].setFillColor(Color::Black);
        Poisoning[i].setPosition(540, 210 + 40 * i);
        if(i == 4 || i == 5)
            Poisoning[i].setOutlineColor(Color::Red);
        else
            Poisoning[i].setOutlineColor(Color::Green);
        Poisoning[i].setOutlineThickness(1);
    }
    Poisoning[0].setString("Left: A");
    Poisoning[1].setString("Right: D");
    Poisoning[2].setString("Down: S");
    Poisoning[3].setString("Rotation: Space");
    Poisoning[4].setString("Restart: R");
    Poisoning[5].setString("Exit: esc");

    //считывание и отрисовка рекордов
    Reed_file();
    for (int i = 0; i < sch_names; i++)
    {
        Names_Score[i].setFont(Myfont);
        Names_Score[i].setFillColor(Color::Black);
        Names_Score[i].setCharacterSize(30);
        Names_Score[i].setString(g_names[i] + ": " + g_Scores[i]);
        Names_Score[i].setPosition(11, 26 + (i * 30));
    }

    Clock clock;
   double timer = 0;

    texture_s.loadFromFile("images\\gray.png");
    texture.loadFromFile("images\\tiles_neww.png");
    sprite.setTextureRect(IntRect(32 * color_blocks, 0, 32, 32));

    //главный цикл
    while (window.isOpen())
    {
        Restart();

        while (v != 'r' && window.isOpen())
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            ostringstream NUM;
            NUM << Score;
            t_Score.setFillColor(Color::Red);
            t_Score.setString("Score: " + NUM.str()); //Score = X
            t_Score.setPosition(310, 2);

            float timec = clock.getElapsedTime().asSeconds();
            clock.restart();
            timer += timec;

            if (gameover)
            {
                delay = 99999999999999;
                if (!Enter)
                {
                    keyboard_nik();
                    NAME.setString(g_name);
                    if (GetAsyncKeyState(13) & 0x1) // Enter
                    {
                        Write_file(g_name);
                        g_name.clear();
                        Enter = true;
                    }
                }
                if (GetAsyncKeyState(82) & 0x1) // R
                {
                    v = 'r';
                }

                if (GetAsyncKeyState(27) & 0x1) //esc выход
                    exit(0);
            }
            else
                keyboard();

            if (timer > delay)
            {
                for (int i = 0; i < 4; i++)
                {
                    if (game_map[1][b[i].x] != 0)
                        gameover = true;
                }
                for (int i = 0; i < 4; i++)
                {
                    b[i] = a[i];
                    a[i].y += 1;
                }
                if (!colision())
                {
                    for (int i = 0; i < 4; i++)
                        game_map[b[i].y][b[i].x] = color_blocks;
                    Create_bloks();
                }
                timer = 0;
                delay = 0.3;
            }

            chek_line();

            window.clear();
            Draw_map();
            draw_spoiler();
            draw_blocks();
            draw_roof();
            draw_line();
            if (gameover) 
            {
                window.draw(GameOver);
                if (!Enter)
                {
                    draw_gameover_window();
                    window.draw(NAME);
                }
            }
            window.draw(t_Score);
            for (int i = 0; i < word; i++)
                window.draw(Poisoning[i]);
            draw_names_Score();
            window.display();
        }
    }
}