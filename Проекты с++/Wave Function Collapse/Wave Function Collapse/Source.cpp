#include "SFML/Graphics.hpp"
#include <locale>
#include <time.h>
#include <Windows.h>
#include <iostream>
#include <fstream>
#include "WaveFunctionCollapse2D.h"
#include "Button.hpp"
#include <thread>

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
        char* c_str = new char[100];
        file.getline(c_str, 100);
        std::string str = c_str;
        if(str != "" && res.size() < 39)
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
        if (str != "" && res.size() < 8)
            res.push_back(str);
    }
    file.close();
    std::remove("tiles\\list.txt");
    return res;
}

void moveMenu(bool& menu_moving, SupEllipseShape& menu, bool menu_hide)
{
    if (menu_moving)
    {
        int speed = 15;
        if (!menu_hide)
        {
            if (menu.getPosition().x > 1280 - menu.getSize().x)
            {
                menu.setPosition(menu.getPosition().x - speed, 0);
            }
            else
            {
                menu.setPosition(1280 - menu.getSize().x, 0);
                menu_moving = false;
            }
        }
        else
        {
            if (menu.getPosition().x < 1280)
            {
                menu.setPosition(menu.getPosition().x + speed, 0);
            }
            else
            {
                menu.setPosition(1280, 0);
                menu_moving = false;
            }
        }
    }
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
    bool in_menu = false;
    bool menu_moving = false;
    bool menu_hide = false;

    Vector2f menu_pos;

    int selected_tile = -1;
    int rotation_tile = 0;
    Texture sprite_tex;
    RectangleShape sprite;
    sprite.setTexture(&sprite_tex);
    sprite.setFillColor(Color(255, 255, 255, 0));

    SupEllipseShape menu;
    menu.setFillColor(Color(75, 75, 75, 225));
    menu.setSize(Vector2f(400, windowY));
    menu.setNM(menu.getSize().x, menu.getSize().y);
    menu.setPosition(windowX - menu.getSize().x, 0);
    menu_pos = menu.getPosition();

    Font font;
    font.loadFromFile("Font\\font.ttf");
    Text load_tiles;
    load_tiles.setFont(font);
    load_tiles.setPosition(menu_pos.x + 30, menu_pos.y + 20);
    load_tiles.setFillColor(Color(255, 0, 0));
    load_tiles.setString("Tiles not found");

    int button_pos_x = 12;
    int button_pos_y = 70;
    Button button_tile;
    button_tile.SetPosition(Vector2f(menu_pos.x + button_pos_x, menu_pos.y + button_pos_y));
    button_tile.SetSize(Vector2f(60, 60));
    button_tile.SetNM(60, 60);

    button_tile.SetOutlineColor(Color(0, 0, 0), 0);
    button_tile.SetOutlineColor(Color(190, 175, 1), 1);
    button_tile.SetOutlineColor(Color(190, 89, 1), 2);
    button_tile.SetOutlineColor(Color(17, 190, 1), 3);

    for (int i = 0; i < 4; i++)
    {
        button_tile.SetOutlineThickness(2, i);
        button_tile.SetFillColor(Color(255, 255, 255), i);
        button_tile.LoadTexture("tiles\\void.png", i);
    }

    std::vector<Button> buttons_tiles;
    buttons_tiles.push_back(button_tile);

    Button button_file;
    button_file.SetFont("Font\\font.ttf");
    button_file.SetSize(Vector2f(200, 60));
    button_file.SetNM(200, 60);
    button_file.SetTextFillColor(Color(30, 30, 30));

    button_file.SetOutlineColor(Color(0, 0, 0), 0);
    button_file.SetOutlineColor(Color(190, 175, 1), 1);
    button_file.SetOutlineColor(Color(17, 190, 1), 2);
    button_file.SetOutlineColor(Color(17, 190, 1), 3);

    for (int i = 0; i < 4; i++)
    {
        button_file.SetFillColor(Color(130, 85, 64), i);
        button_file.SetOutlineThickness(2, i);
    }
    button_file.SetTexture(0);

    std::vector<Button> buttons_files;


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

    std::vector<std::string> forder_list = getForders();

    for (int i = 0; i < forder_list.size(); i++)
    {
        button_file.SetText(forder_list[i]);
        button_file.SetPosition(Vector2f(menu_pos.x + 30, menu_pos.y + button_pos_y + i * (button_file.GetSize().y + 20)));
        button_file.SetTextPosition(Vector2f(button_file.GetPosition().x + 10, button_file.GetPosition().y + 10));

        buttons_files.push_back(button_file);
    }
    bool is_load = false;

    Vector2i cursor;
    sprite.setSize(Vector2f(map.getSizeTile()));
    sprite.setOrigin(sprite.getSize().x / 2, sprite.getSize().y / 2);

    std::thread th;
    float progress = 0;

    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::MouseWheelMoved)
            {
                if (selected_tile != -1)
                {
                    if (event.mouseWheel.delta > 0)
                    {
                        rotation_tile += 90;
                        if (rotation_tile > 270)
                            rotation_tile = 0;
                    }
                    if (event.mouseWheel.delta < 0)
                    {
                        rotation_tile -= 90;
                        if (rotation_tile < 0)
                            rotation_tile = 270;
                    }
                    sprite.setRotation(rotation_tile);
                }
            }
        }

        if (window.hasFocus())
        {
            Vector2i pixelPos = Mouse::getPosition(window);//забираем коорд курсора
            MousePos = window.mapPixelToCoords(pixelPos);//переводим их в игровые (уходим от коорд окна)

            in_menu = MousePos.x > menu_pos.x;

            float time = clock.getElapsedTime().asSeconds();
            clock.restart();
            timer += time;

            if (progress == 0)
            {
                for (int i = 0; i < buttons_files.size(); i++)
                {
                    if (buttons_files[i].ChekClik(MousePos, event))
                    {
                        std::string forder = "tiles\\" + forder_list[i] + "\\";
                        std::vector<std::string> file_list = getFiles(forder);
                        int x = 1;

                        for (int i = 0, j = 0; i < file_list.size(); i++)
                        {
                            map.addTile(forder + file_list[i]);
                            button_tile.SetPosition(Vector2f(menu_pos.x + button_pos_x + (button_tile.GetSize().x + 20) * x, menu_pos.y + button_pos_y + (button_tile.GetSize().y + 20) * j));
                            buttons_tiles.push_back(button_tile);
                            for (int j = 0; j < 4; j++)
                                buttons_tiles.back().LoadTexture(forder + file_list[i], j);

                            buttons_tiles.back().SetTexture(0);
                            x++;
                            if (x > 4)
                            {
                                j++;
                                x = 0;
                            }
                        }

                        load_tiles.setFillColor(Color(170, 99, 29));

                        th = std::thread(&WaveFunctionCollapse2D::create, std::ref(map), std::ref(progress));
                        buttons_files.clear();
                    }
                }
            }

            if (progress <= 100 && !is_load)
            {
                std::string progress_str = std::to_string(progress);
                int dot = progress_str.find(',');
                progress_str.resize(dot + 3);
                load_tiles.setString("Load tiles complete: " + progress_str + "%");
                if (progress == 100)
                {
                    load_tiles.setFillColor(Color(19, 150, 1));
                    is_load = true;
                }
            }

            if (in_menu)
            {
                for (int i = 0; i < buttons_tiles.size(); i++)
                {
                    if (buttons_tiles[i].ChekClik(MousePos, event))
                    {
                        rotation_tile = 0;
                        if (selected_tile != -1)
                            buttons_tiles[selected_tile].SetTexture(Button::peace);

                        selected_tile = i;
                        buttons_tiles[i].SetTexture(Button::block);
                        sprite_tex = buttons_tiles[i].getTexture(0);
                        sprite.setFillColor(Color(255, 255, 255, 155));
                        sprite.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), (sf::Vector2i)sprite_tex.getSize()));
                        sprite.setRotation(0);
                    }
                }
            }

            if (selected_tile != -1)
            {
                cursor.x = MousePos.x / map.getSizeTile().x;
                cursor.y = MousePos.y / map.getSizeTile().y;

                sprite.setPosition(sprite.getSize().x / 2 + cursor.x * map.getSizeTile().x, sprite.getSize().y / 2 + cursor.y * map.getSizeTile().y);
            }

            if (!in_menu)
            {
                if (Mouse::isButtonPressed(Mouse::Left))
                {
                    map.setTileInGrid(selected_tile-1, cursor.x, cursor.y, rotation_tile);
                }
            }

            if (Mouse::isButtonPressed(Mouse::Right) && timer > delay)
            {
                if (selected_tile != -1)
                {
                    sprite.setFillColor(Color(255, 255, 255, 0));
                    buttons_tiles[selected_tile].SetTexture(Button::peace);
                    selected_tile = -1;
                    rotation_tile = 0;
                    timer = 0;
                }
            }

            if (Keyboard::isKeyPressed(Keyboard::LControl) && timer > delay)
            {
                map.clearPrevTiles();
                map.clear();
                timer = 0;
            }

            if (is_load && Keyboard::isKeyPressed(Keyboard::Space) && timer > delay)
            {
                is_loop = !is_loop;
                timer = 0;
            }

            if (is_load && Keyboard::isKeyPressed(Keyboard::LAlt) && timer > delay)
            {
                map.clear();
                map.update();
                timer = 0;
            }

            if (Keyboard::isKeyPressed(Keyboard::M) && timer > delay)
            {
                menu_moving = true;
                menu_hide = !menu_hide;
                timer = 0;
            }
        }

        if (is_loop)
        {
            if (menu_hide && !menu_moving)
            {
                map.update();
                if (map.is_failed)
                    map.clear();
                if (map.is_complete)
                    is_loop = false;
            }
            else
            {
                menu_hide = true;
                menu_moving = true;
            }
        }
        moveMenu(menu_moving, menu, menu_hide);
        if (menu_moving)
        {
            menu_pos = menu.getPosition();
            load_tiles.setPosition(menu_pos.x + 30, menu_pos.y + 20);
            int x = 0;
            for (int i = 0, j = 0; i < buttons_tiles.size(); i++)
            {
                buttons_tiles[i].SetPosition(Vector2f(menu_pos.x + button_pos_x + (button_tile.GetSize().x + 20) * x, menu_pos.y + button_pos_y + (button_tile.GetSize().y + 20) * j));

                x++;
                if (x > 4)
                {
                    j++;
                    x = 0;
                }
            }

            for (int i = 0; i < buttons_files.size(); i++)
            {
                buttons_files[i].SetPosition(Vector2f(menu_pos.x + 30, menu_pos.y + button_pos_y + i * (button_file.GetSize().y + 20)));
                buttons_files[i].SetTextPosition(Vector2f(buttons_files[i].GetPosition().x + 10, buttons_files[i].GetPosition().y + 10));
            }
        }
        
        window.clear();
        map.draw(window);
        if (selected_tile != -1)
            window.draw(sprite);

        if (!menu_hide || menu_moving)
            window.draw(menu);
        if (!is_load)
        {
            for (Button button : buttons_files)
                button.Draw(window);
        }

        window.draw(load_tiles);
        if(progress == 100)
            for (Button button : buttons_tiles)
                button.Draw(window);
        window.display();

        if (th.joinable())
            th.detach();
    }
}