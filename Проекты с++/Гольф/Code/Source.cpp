#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include <locale>
#include <time.h>
#include <Windows.h>
#include <iostream>
#include "level.h"
#include "Player.h"
#include "Menu.h"
#include "LoadSettings.h"

using namespace sf;

void DrawGameOver(RenderWindow& window, int& AllStrokes, float& MeanCountStrokes)
{

    static Texture TexBg;
    static RectangleShape Background;
    static Font font;
    static Text TextGameOver, GameOverInfo;
    static bool LoadGameOver = false, TextAngelUp = true;
    static Clock clock;

    float time = clock.getElapsedTime().asSeconds();
    float TextAngel = 3 * sin(2 * time);
    float TextSpeed = 0.5;
    if (!LoadGameOver)
    {
        TexBg.loadFromFile("image\\end.png");
        Background.setTexture(&TexBg);
        Background.setSize(Vector2f(window.getSize()));
        Background.setPosition(0, 0);

        font.loadFromFile("Font\\font.ttf");
        TextGameOver.setFont(font);
        TextGameOver.setCharacterSize(200);
        TextGameOver.setPosition(640, 180);
        TextGameOver.setOrigin(340, 100);
        TextGameOver.setString("Game Over");
        TextGameOver.setOutlineColor(Color::Black);
        TextGameOver.setOutlineThickness(5);

        GameOverInfo.setFont(font);
        GameOverInfo.setCharacterSize(50);
        GameOverInfo.setPosition(TextGameOver.getPosition().x - 340, TextGameOver.getPosition().y + 220);
        GameOverInfo.setString("Info: \nAll Strokes: " + std::to_string(AllStrokes) + "\nMean count strokes : " + std::to_string(int(MeanCountStrokes)));
        GameOverInfo.setOutlineColor(Color::Black);
        GameOverInfo.setOutlineThickness(3);

        LoadGameOver = true;
    }

    TextGameOver.setPosition(TextGameOver.getPosition().x, TextGameOver.getPosition().y + TextSpeed * sin(2 * time));
    TextGameOver.setRotation(TextAngel);

    window.draw(Background);
    window.draw(TextGameOver);
    window.draw(GameOverInfo);
}

int main()
{
    setlocale(LC_ALL, "ru");
    srand(std::time(NULL));
    int windowX = 1280;
    int windowY = 960;
    bool win[2] = { false, false };
    int whatLevel, levelTry = 0;
    int strokes = 0, Allstrokes = 0;
    float MeanCountStrokes = strokes;
    bool GameOver = false;

    RenderWindow window(VideoMode(windowX, windowY), "Pair-Golf", Style::Close, ContextSettings());
    Event event;
    Vector2f MousePos;
    menu(window);

    Settings settings("Settings\\Settings.info");
    whatLevel = settings.GetParam("LastLevel").param;

    Texture TexLoading;
    TexLoading.loadFromFile("image\\Loading.png");
    RectangleShape SpriteLoading;
    SpriteLoading.setPosition(-100, 0);
    SpriteLoading.setSize(Vector2f(window.getSize().x + 200, window.getSize().y));
    SpriteLoading.setTexture(&TexLoading);
    bool IsLoading = true;

    Texture tex, TexBg;
    TexBg.loadFromFile("image\\bg.png");
    tex.loadFromFile("image\\levelText_bg.png");
    RectangleShape strokesBg, holeBg, Bg;
    Bg.setTexture(&TexBg);
    Bg.setSize(Vector2f(window.getSize()));

    strokesBg.setRotation(180);
    strokesBg.setTexture(&tex);
    strokesBg.setSize(Vector2f(190, 60));
    strokesBg.setOrigin(strokesBg.getSize().x / 2, strokesBg.getSize().y);
    strokesBg.setPosition(windowX / 2, 0);

    holeBg.setTexture(&tex);
    holeBg.setSize(Vector2f(140, 60));
    holeBg.setOrigin(holeBg.getSize().x / 2, holeBg.getSize().y);
    holeBg.setPosition(windowX / 2, windowY);
    Font font;
    font.loadFromFile("Font\\font.ttf");
    Text text, holes;
    text.setFont(font);
    text.setString("Strokes: " + std::to_string(strokes));
    text.setPosition(strokesBg.getPosition().x - 70, strokesBg.getPosition().y + 7);
    text.setFillColor(Color::White);
    text.setCharacterSize(35);

    holes = text;
    holes.setString("Hole: " + std::to_string(whatLevel));
    holes.setPosition(holeBg.getPosition().x - 45, holeBg.getPosition().y - 53);


    SoundBuffer buffer[3];
    Sound swing, shole, charge;
    bool holePlay = false;
    float volume = 5;

    buffer[0].loadFromFile("Sound\\swing.ogg");
    buffer[1].loadFromFile("Sound\\hole.ogg");
    buffer[2].loadFromFile("Sound\\charge.ogg");
    swing.setBuffer(buffer[0]);
    swing.setVolume(volume);
    shole.setBuffer(buffer[1]);
    shole.setVolume(volume);
    charge.setBuffer(buffer[2]);
    charge.setVolume(volume);

    Clock clock;
    float timer = 0;
    double delay = 1;

    Level level;
    level.LoadFromFile("Levels\\level_" + std::to_string(whatLevel) + ".tmx");

    CircleShape hole[2], shadowHole[2];
    Player player[2];
    Player::SetFactor(1);

    for (int i = 0; i < 2; i++)
    {
        hole[i].setFillColor(Color::Black);
        hole[i].setRadius(16);
        hole[i].setOrigin(hole[i].getRadius(), hole[i].getRadius());
        Vector2f SpawnHole = Vector2f(level.GetObject2("SpawnHole" + std::to_string(i)).rect.left, level.GetObject2("SpawnHole" + std::to_string(i)).rect.top);
        hole[i].setPosition(SpawnHole.x + 32, SpawnHole.y + 32);

        shadowHole[i] = hole[i];
        shadowHole[i].setPosition(hole[i].getPosition().x, hole[i].getPosition().y - 5);
        shadowHole[i].setFillColor(Color(80, 80, 80));

        player[i].LoadObjectsFromLevel(level);
        player[i].SetId(i);
        player[i].SetRadius(6);
        player[i].SetOutlineThickness(6);
        Vector2f SpawnBall = Vector2f(level.GetObject2("SpawnBall" + std::to_string(i)).rect.left, level.GetObject2("SpawnBall" + std::to_string(i)).rect.top);
        player[i].SetPosition(Vector2f(SpawnBall.x + 32, SpawnBall.y + 32));
    }

    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                settings.Save();
                window.close();
            }

            if (!GameOver)
            {
                for (int i = 0; i < 2; i++)
                {
                    switch (player[i].OnCliked(event, MousePos, charge))
                    {
                    case 1:
                        player[!i].SetPoint(player[i].GetPoint());
                        break;
                    case 2:
                        player[!i].SetVelocity(player[i].GetVec(), player[i].GetVelocity());
                        swing.play();
                        strokes++;
                        Allstrokes++;
                        settings.GetParam("AllStrokes").param++;
                        settings.GetParam("MeamCountStrokes").param = Allstrokes / (levelTry + 1);
                        text.setString("Strokes: " + std::to_string(strokes));
                        break;
                    }
                }
            }
        }

        Vector2i pixelPos = Mouse::getPosition(window);//забираем коорд курсора
        MousePos = window.mapPixelToCoords(pixelPos);//переводим их в игровые (уходим от коорд окна)

        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;
        if (timer > delay && !GameOver)
        {
            if (IsLoading)
            {
                delay = 0.005;
                SpriteLoading.setPosition(SpriteLoading.getPosition().x - 15, 0);
                if (SpriteLoading.getPosition().x <= -1480)
                {
                    SpriteLoading.setPosition(1380, 0);
                    IsLoading = false;
                }
            }
            else
            {
                for (int i = 0; i < 2; i++)
                {
                    player[i].Ubdate();

                    if (player[i].ChekWin(hole[i].getPosition(), hole[i].getRadius()) && !win[i])
                    {
                        win[i] = true;
                        if (!holePlay)
                        {
                            shole.play();
                            holePlay = 1;
                        }
                    }

                    if (win[i])
                        player[i].Win(hole[i].getPosition());
                }
                if (player[0].GetRadius() > 0.2 || player[1].GetRadius() > 0.2)
                    holePlay = 0;

                if (player[0].GetRadius() == 0 && player[1].GetRadius() == 0)
                {
                    holePlay = 0;
                    strokes = 0;
                    whatLevel++;
                    text.setString("Strokes: " + std::to_string(strokes));
                    holes.setString("Hole: " + std::to_string(whatLevel));

                    if (whatLevel == 8)
                        Player::SetFactor(-1);
                    else
                        Player::SetFactor(1);

                    level.LoadFromFile("Levels\\level_" + std::to_string(whatLevel) + ".tmx");
                    if(whatLevel % 10 == 0)
                        settings.GetParam("LastLevel").param = whatLevel;

                    for (int i = 0; i < 2; i++)
                    {
                        win[i] = false;

                        player[i].LoadObjectsFromLevel(level);
                        player[i].SetColor(Color::White);
                        player[i].SetRadius(6);
                        player[i].SetOutlineThickness(6);
                        player[i].SetVelocity();
                        Vector2f SpawnBall = Vector2f(level.GetObject2("SpawnBall" + std::to_string(i)).rect.left, level.GetObject2("SpawnBall" + std::to_string(i)).rect.top);
                        player[i].SetPosition(Vector2f(SpawnBall.x + 32, SpawnBall.y + 32));


                        Vector2f SpawnHole = Vector2f(level.GetObject2("SpawnHole" + std::to_string(i)).rect.left, level.GetObject2("SpawnHole" + std::to_string(i)).rect.top);
                        hole[i].setPosition(SpawnHole.x + 32, SpawnHole.y + 32);
                        shadowHole[i].setPosition(hole[i].getPosition().x, hole[i].getPosition().y - 5);
                    }
                }
                else if (strokes == 12 && player[0].GetVelocity() == 0 && player[1].GetVelocity() == 0)
                {
                    if (!win[0] || !win[1])
                    {
                        GameOver = true;
                        MeanCountStrokes = Allstrokes / (levelTry + 1);
                    }
                }
            }
        }

        window.clear();
        window.draw(Bg);
        level.Draw(window);
        for (int i = 0; i < 2; i++)
        {
            window.draw(shadowHole[i]);
            window.draw(hole[i]);
            player[i].Draw(window);
        }

        if (!GameOver)
        {
            window.draw(strokesBg);
            window.draw(text);
            window.draw(holeBg);
            window.draw(holes);
        }
        else
            DrawGameOver(window, Allstrokes, MeanCountStrokes);

        if (IsLoading)
            window.draw(SpriteLoading);
        window.display();
    }
}