#include "SFML/Graphics.hpp"
#include <time.h>
#include <Windows.h>
#include <iostream>
#include <vector>
#include <thread>
#include <sstream>
#include "Menu.h"

using namespace std;
using namespace sf;

int style = Style::Close;
int windowX = 1280;
int windowY = 720;
std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
RenderWindow window(VideoMode(windowX, windowY, modes[0].bitsPerPixel), "Defense of the planet", style, ContextSettings());

Event event;
View view;
Vector2f pos;

Text score;
Font font;

bool clik = false;
bool stop = false;
bool fire = false;
bool boom = false;
bool gameover = false;

int SCORE = 0;
float speed = 0;
float MAX_speed = 5;
float bspeed = 15;
float Sspeed = 0.1;
float Aspeed = 1.3;
float RandAsBlue = 10;
float RandAsBig = 20;
int HpAs = 2;
int HpAsBlue = 0;
int HpAsBig = 5;


int Ticks = 0, Ticks2 = 0;
const int space_count = 3;
float dist_moon = 350;
float angelAster = 0;
float Angel_moon = 0;
float Anim_Trassa = -1;
int damage = 1;
vector <float> Anim_bum;
vector <float> Anim_dest;
vector <float> countSmoke;
vector <int> HpAsters;
vector <Vector2f> PAsters;

Texture TBum;
Texture Tcross;
Texture Tbullet;
Texture Tsmoke;
Texture Tsmoke2;
Texture Tplayer;
Texture Ttrassa;
Texture TEarth;
Texture Tmoon;
Texture Taster[4];
Texture Tspace;
Texture TAster;
Texture Tshild;
Texture Tdestroi;
CircleShape cross;
RectangleShape space[space_count][space_count];
RectangleShape trassa;
vector <RectangleShape> bullets;
vector <Vector2f> Pbullets;
vector <Vector2f> Psmoke;
vector <CircleShape> Smoke;
CircleShape Adestroi;
RectangleShape shild;

vector <vector<CircleShape>> Asmoke;
vector <vector<Vector2f>> PAsmoke;

vector <CircleShape> Asters;
vector <CircleShape> Bum;
vector <CircleShape> Dest;

vector <int> bum_time;

class Player
{
private:

public:
    float x, y, dx, dy;
    const float massa = 100;
    Vector2f Center;
    Vector2f Size;
    RectangleShape sprite;

    Player(float X, float Y)
    {
        Size.x = X;
        Size.y = Y;
        sprite.setSize(Size);
        sprite.setOrigin(Size.x / 2, Size.y / 2);
    }

    void SetTexture(Texture& image)
    {
        sprite.setTexture(&image);
    }

    void SetPosition(double tx, double ty)
    {
        x = tx;
        y = ty;
        sprite.setPosition(x, y);
    }

    void SetVec(double tx, double ty)
    {
        dx = tx;
        dy = ty;
    }

    void SetSize(double x, double y)
    {
        Size.x = x;
        Size.y = y;
        sprite.setSize(Size);
        sprite.setOrigin(Size.x / 2, Size.y / 2);
    }
    
    void fire()
    {
        
    }

    void update()
    {
        sprite.setOrigin(Size.x / 2, Size.y / 2);
        if (clik)
        {
            Vector2f p(x, y);
            Vector2f pos(cross.getPosition());
            float sk = sqrt(pow(dx - (pos.x), 2) + pow(dy - (pos.y), 2));
            if (sk > MAX_speed + 2)
            {
                dx += (MAX_speed + 2) * (pos.x - dx) / sk; //dx += (speed + 2) * (pos.x - dx) / sk;
                dy += (MAX_speed + 2) * (pos.y - dy) / sk; //dy += (speed + 2) * (pos.y - dy) / sk;
            }

            float ck = sqrt(pow(p.x - dx, 2) + pow(p.y - dy, 2));
            if (ck > speed)
            {
                p.x += speed * ((dx) - p.x) / ck;
                p.y += speed * ((dy) - p.y) / ck;
                sprite.setPosition(p.x, p.y);
            }
            
            x = sprite.getPosition().x;
            y = sprite.getPosition().y;
        }
        else
        {
            Vector2f p(x, y);
            float ck = sqrt(pow(p.x - (dx), 2) + pow(p.y - (dy), 2));
            if (ck > speed)
            {
                Vector2f moveto(speed * ((dx)-p.x) / ck, speed * ((dy)-p.y) / ck);
                p.x += moveto.x;
                p.y += moveto.y;
                sprite.setPosition(p.x, p.y);

                if (ck < 200)
                {
                    dx += moveto.x; //dx += (speed * 2) * ((dx)-p.x) / ck;
                    dy += moveto.y; //dy += (speed * 2) * ((dy)-p.y) / ck;
                }
            }

            x = sprite.getPosition().x;
            y = sprite.getPosition().y;
        }

        float dX = pos.x - x;//вектор , колинеарный прямой, которая пересекает спрайт и курсор //dx - pos.x  // dy - pos.y
        float dY = pos.y - y;//он же, координата y
        float rotation = (atan2(dY, dX)) * 180 / 3.14159265;//получаем угол в радианах и переводим его в градусы
        sprite.setRotation(rotation + 90);//поворачиваем спрайт на эти градусы
    }
};
Player p(51.9, 87.3);

class object
{
public:
    float dx, dy;
    float massa = 100;
    float radius = 100;
    Vector2f Center;
    Vector2f Scale;
    CircleShape sprite;

    object()
    {
        Scale.x = 1;
        Scale.y = 1;
    }

    void SetTexture(Texture& image)
    {
        sprite.setOrigin(image.getSize().x / 2, image.getSize().y / 2);
        sprite.setTexture(&image);
    }

    void SetPosition(double x, double y)
    {
        dx = x;
        dy = y;
        sprite.setPosition(dx, dy);
    }

    void SetRadius(float rad)
    {
        radius = rad;
        sprite.setRadius(radius);
    }

    void SetScale(double x, double y)
    {
        Scale.x = x;
        Scale.y = y;
        sprite.setScale(Scale.x, Scale.y);
    }

    void SetMassa(float mas)
    {
        massa = mas;
    }

    void update()
    {

    }
};
object E;
object M;

void Start()
{
    Image im;
    im.loadFromFile("Texture\\mars.png");
    window.setIcon(1200, 1200, im.getPixelsPtr());

    window.setFramerateLimit(60);
    menu(window);

    //выбор сложности
    switch (what_diff)
    {
    case 1://лёгкая
        Aspeed = 1.3;
        RandAsBlue = 20;
        RandAsBig = 10;
        HpAs = 2;
        HpAsBlue = 0;
        HpAsBig = 5;
        break;
    case 2://нормальная
        Aspeed = 1.5;
        RandAsBlue = 10;
        RandAsBig = 20;
        HpAs = 2;
        HpAsBlue = 0;
        HpAsBig = 5;
        break;
    case 3://сложная
        Aspeed = 2;
        RandAsBlue = 5;
        RandAsBig = 25;
        HpAs = 2;
        HpAsBlue = 0;
        HpAsBig = 5;
        break;
    default: //по стандарту нормальная
        Aspeed = 1.5;
        RandAsBlue = 10;
        RandAsBig = 20;
        HpAs = 2;
        HpAsBlue = 0;
        HpAsBig = 5;
        break;
    }

    Tspace.loadFromFile("Texture\\space4.png");
    for (int i = 0; i < space_count; i++)
        for (int j = 0; j < space_count; j++)
        {
            space[i][j].setTexture(&Tspace);
            space[i][j].setSize(Vector2f(windowX, windowX)); //windowX * 3.2, windowY * 3.2
            space[i][j].setPosition(i * space[i][j].getSize().x, j * space[i][j].getSize().y);
        }

    if(choose_mars)
        TEarth.loadFromFile("Texture\\Mars.png");
    else
        TEarth.loadFromFile("Texture\\Earth.png");
    E.SetTexture(TEarth);
    E.SetRadius(125);
    E.sprite.setOrigin(E.radius, E.radius);
    E.SetPosition(windowX * space_count / 2, windowX * space_count / 2);

    Tmoon.loadFromFile("Texture\\moon.png");
    M.SetTexture(Tmoon);
    M.SetRadius(20);
    M.sprite.setOrigin(M.radius, M.radius);
    Vector2f pos_moon(M.dx, M.dy);
    pos_moon.x = E.dx + (E.radius + dist_moon) * cos(Angel_moon);
    pos_moon.y = E.dy + (E.radius + dist_moon) * sin(Angel_moon);
    M.SetPosition(pos_moon.x, pos_moon.y);
    if (choose_mars)
        M.sprite.setFillColor(Color(228, 155, 15));

    Tplayer.loadFromFile("Texture\\spacecraft.png");
    p.SetTexture(Tplayer);
    p.SetPosition(E.dx + 200, E.dy + 200);
    p.SetVec(p.x, p.y);
    view.setCenter(p.x, p.y);
    
    Tbullet.loadFromFile("Texture\\bullet.png");
    Tsmoke.loadFromFile("Texture\\smoke.png");
    Tsmoke2.loadFromFile("Texture\\smoke2.png");
    Tcross.loadFromFile("Texture\\crosshair.png");

    TBum.loadFromFile("Texture\\fires.png");

    cross.setTexture(&Tcross);
    cross.setRadius(26);
    cross.setOrigin(cross.getRadius(), cross.getRadius());
    cross.setFillColor(Color::Green);
    cross.setPosition(pos);

    IntRect rect(0, 0, 128, 128);
    Taster[0].loadFromFile("Texture\\asteroids.png", rect);
    rect = { 128, 0, 128, 128 };
    Taster[1].loadFromFile("Texture\\asteroids.png", rect);
    rect = { 0, 128, 128, 128 };
    Taster[2].loadFromFile("Texture\\asteroids.png", rect);
    rect = { 128, 128, 128, 128 };
    Taster[3].loadFromFile("Texture\\asteroids.png", rect);

    Ttrassa.loadFromFile("Texture\\splashs.png");
    trassa.setTexture(&Ttrassa);
    trassa.setSize(Vector2f(30, 30));
    trassa.setOrigin(trassa.getSize().x/2, trassa.getSize().y + 5);

    Tdestroi.loadFromFile("Texture\\destroi.png");

    Tshild.loadFromFile("Texture\\shild.png");
    shild.setTexture(&Tshild);
    shild.setSize(p.Size);
    shild.setOrigin(shild.getSize().x / 2, shild.getSize().y / 2);

    font.loadFromFile("Font\\rus_eng.ttf");
    score.setFont(font);
    score.setCharacterSize(30);
    score.setFillColor(Color::White);

    Loading.setPosition(view.getCenter().x - windowX/2, view.getCenter().x - windowY / 2);
}

void DrawGame()
{
    for (int i = 0; i < space_count; i++)
        for (int j = 0; j < space_count; j++)
            window.draw(space[i][j]);

    for (int i = 0; i < bullets.size(); i++)
        window.draw(bullets[i]);

    window.draw(M.sprite);

    window.draw(E.sprite);

    for (int i = 0; i < Asters.size(); i++)
    {
        for (int j = 0; j < Asmoke[i].size(); j++)
            window.draw(Asmoke[i][j]);

        window.draw(Asters[i]);
    }

    window.draw(p.sprite);

    window.draw(trassa);


    for (int i = 0; i < Smoke.size(); i++)
        window.draw(Smoke[i]);

    if (boom)
    {
        for (int i = 0; i < Bum.size(); i++)
            window.draw(Bum[i]);

        for (int i = 0; i < Dest.size(); i++)
            window.draw(Dest[i]);
    }

    window.draw(cross);

    window.draw(score);

    if(isLoading)
        window.draw(Loading);
}

void pause()
{
    Text GO[2];
    Font myfont;
    myfont.loadFromFile("Font\\rus_eng.ttf");
    GO[0].setFont(myfont);
    GO[1].setFont(myfont);
    GO[0].setString("GAME OVER");
    GO[0].setCharacterSize(100);
    GO[0].setFillColor(Color::White);
    GO[1].setFillColor(Color::White);
    GO[0].setPosition(view.getCenter().x - 350, view.getCenter().y - 200);
    GO[1].setPosition(view.getCenter().x - 330, view.getCenter().y + 20);

    std::ostringstream NUM;
    NUM << SCORE;
    GO[1].setString(L"R - Рестарт \t SCORE: " + NUM.str() + L" \t ESC - Выход");
    bool R = false;

    for (int i = 0; i < 2; i++)
        window.draw(GO[i]);
    window.display();

    while (!R)
    {
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            if (GetAsyncKeyState(82) & 0x1)
                R = true;
            if (GetAsyncKeyState(27) & 0x1) //esc выход
                exit(0);
            window.setMouseCursor(cursor);
        }
    }
}

void Restart()
{
    if (gameover)
        pause();
    p.SetPosition(E.dx + 200, E.dy + 200);
    p.SetVec(p.x, p.y);
    view.setCenter(p.x, p.y);
    speed = 0.1;
    Ticks = Ticks2 = 0;
    bullets.clear();
    Asters.clear();
    PAsters.clear();
    Asmoke.clear();
    PAsmoke.clear();
    HpAsters.clear();
    Bum.clear();
    Dest.clear();
    Pbullets.clear();
    Psmoke.clear();
    Smoke.clear();
    clik = false;
    SCORE = 0;
    gameover = false;
}

float length(float x, float y, float x2, float y2)
{
    float L = pow(x2 - x, 2) + pow(y2 - y, 2);
    return sqrt(L);
}

float angel(Vector2f p1, Vector2f p2)
{
    float dX = p1.x - p2.x;//вектор , колинеарный прямой, которая пересекает спрайт и курсор //dx - pos.x  // dy - pos.y
    float dY = p1.y - p2.y;//он же, координата y
    float rotation = (atan2(dY, dX)) * 180 / 3.14159265;//получаем угол в радианах и переводим его в градусы
    rotation += 90;
    return rotation;
}

void anim_fire()
{
    if(Anim_Trassa != -1)
    {
        IntRect rec;
        Anim_Trassa += 1;
        if (Anim_Trassa < 5)
            rec = { int(Anim_Trassa) * 192, 0, 192, 192 };
        if (Anim_Trassa > 5 && Anim_Trassa < 10)
            rec = { (int(Anim_Trassa) - 5) * 192, 192, 192, 192 };
        if (Anim_Trassa > 10 && Anim_Trassa < 15)
            rec = { (int(Anim_Trassa) - 10) * 192, 384, 192, 192 };
        if (Anim_Trassa > 15)
            rec = { (int(Anim_Trassa) - 15) * 192, 576, 192, 192 };
        if (Anim_Trassa >= 20)
            Anim_Trassa = -1;

        trassa.setTextureRect(rec);
        trassa.setPosition(p.x, p.y);
        trassa.setRotation(p.sprite.getRotation());
    }
}

bool collision(float fault, CircleShape Obj, RectangleShape& Bull)
{
    Bull.setOrigin(Bull.getSize().x / 2, 0);
    float ck = sqrt(pow(Obj.getPosition().x - Bull.getPosition().x, 2) + pow(Obj.getPosition().y - Bull.getPosition().y, 2));
    if (ck <= Obj.getRadius() - fault)
        return true;
    else
    {
        Bull.setOrigin(Bull.getSize().x / 2, Bull.getSize().y);
        return false;
    }
}

bool collision(CircleShape Obj, RectangleShape Bull, float speed)
{
    float ck = sqrt(pow(Obj.getPosition().x - Bull.getPosition().x, 2) + pow(Obj.getPosition().y - Bull.getPosition().y, 2));
    if (ck < Obj.getRadius() - speed)
        return true;
    else
        return false;
}

bool collision(CircleShape Obj, CircleShape Obj2, float speed)
{
    float ck = sqrt(pow(Obj.getPosition().x - Obj2.getPosition().x, 2) + pow(Obj.getPosition().y - Obj2.getPosition().y, 2));
    if (ck < Obj.getRadius() - speed)
        return true;
    else
        return false;
}

void spawn_bum(RectangleShape bull, int index)
{
    Vector2f b(bull.getPosition());
    float bull_size = bull.getSize().y;
    CircleShape bum;
    bum.setTexture(&TBum);
    bum.setRadius(20);
    bum.setOrigin(bum.getRadius(), bum.getRadius());
    bum.setRotation(bull.getRotation());
    bum.setPosition(b.x, b.y);
    Bum.push_back(bum);
    Anim_bum.push_back(0);

    bullets.erase(bullets.begin() + index);
    Pbullets.erase(Pbullets.begin() + index);
    boom = true;
}

void spawn_boom(Vector2f pos, int index)
{
    CircleShape bum;
    bum.setTexture(&Tdestroi);
    bum.setRadius(Asters[index].getRadius() + 5);
    bum.setFillColor(Asters[index].getFillColor());
    bum.setOrigin(bum.getRadius(), bum.getRadius());
    bum.setPosition(pos.x, pos.y);
    Dest.push_back(bum);
    Anim_dest.push_back(0);
    boom = true;
}

void spawn_Asters(Vector2f pos, float radius, int hp, Color c, Vector2f pos2)
{
    CircleShape A;
    A.setTexture(&Taster[rand() % 4]);
    A.setRadius(radius);
    A.setFillColor(c);
    A.setOrigin(A.getRadius(), A.getRadius());
    A.setPosition(pos);
    Asters.push_back(A);
    if (c == Color(130, 200, 255))
        HpAsters.push_back(hp + HpAsBlue);
    else
        HpAsters.push_back(hp);
    PAsters.push_back(pos2);
}

void spawn_Asmoke(Vector2f pos, int index)
{
    CircleShape smoke;
    smoke.setTexture(&Tsmoke2);
    smoke.setRadius(Asters[index].getRadius());
    smoke.setFillColor(Asters[index].getFillColor());
    smoke.setOrigin(smoke.getRadius(), smoke.getRadius());
    smoke.setPosition(pos.x, pos.y);
    Asmoke[index].push_back(smoke);
    PAsmoke[index].push_back(pos);
}

void keyboard()
{
    if (GetAsyncKeyState(1) & 0x1)
    {
        shoot.play();
        fire = true;
        RectangleShape bullet;
        bullet.setTexture(&Tbullet);
        bullet.setSize(Vector2f(16, 40));
        bullet.setOrigin(bullet.getSize().x / 2, bullet.getSize().y);
        bullet.setRotation(p.sprite.getRotation());
        bullet.setPosition(p.x, p.y);
        Pbullets.push_back(cross.getPosition());
        bullets.push_back(bullet);

        Anim_Trassa = 0;
    }

    if (GetAsyncKeyState(87)) //W
    {
        clik = true;
    }
    else
        clik = false;

    if (GetAsyncKeyState(32)) //space
    {
        stop = true;
    }
    else
        stop = false;

    if (GetAsyncKeyState(32) && GetAsyncKeyState(87))
    {
        clik = true;
        stop = false;
    }


    if (GetAsyncKeyState(122) & 0x1) //f11
    {
        if (!fullscreen)
        {
            fullscreen = true;
            std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
            int windowX = modes[0].width;
            int windowY = modes[0].height;
            window.create(VideoMode(windowX, windowY, modes[0].bitsPerPixel), "Defense of the planet", 8, ContextSettings());
            window.setFramerateLimit(60);
        }
        else
        {
            fullscreen = false;
            std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
            int windowX = 1280;
            int windowY = 720;
            window.create(VideoMode(windowX, windowY, modes[0].bitsPerPixel), "Defense of the planet", style, ContextSettings());
            window.setFramerateLimit(60);
        }
    }

    if (GetAsyncKeyState(27) & 0x1) //esc
        gameover = true;
}

void Tick()
{
    Ticks++;
    Ticks2++;

    //дым игрока
    if (clik && Smoke.size() < 100)
    {
        //Ticks = 0;
        CircleShape smoke;
        smoke.setTexture(&Tsmoke);
        smoke.setFillColor(Color(0, 100, 150)); // Color(0, 100, 150)
        smoke.setRadius(10);
        smoke.setOrigin(smoke.getRadius(), -10);
        smoke.setPosition(p.x, p.y);
        smoke.setRotation(p.sprite.getRotation());
        Smoke.push_back(smoke);
        Psmoke.push_back(pos);
    }

    //логика дыма игрока
    for (int i = 0; i < Smoke.size(); i++)
    {
            Vector2f s(Smoke[i].getPosition());
            Vector2f fin(Psmoke[i]);

            //float cks = sqrt(pow(s.x - (p.x), 2) + pow(s.y - (p.y), 2));
            float ck = sqrt(pow(s.x - (fin.x), 2) + pow(s.y - (fin.y), 2));
            s.x -= Sspeed * ((fin.x) - s.x) / ck;
            s.y -= Sspeed * ((fin.y) - s.y) / ck;
            Smoke[i].setPosition(s.x, s.y);

            Smoke[i].setTexture(&Tsmoke);
            float step = 100;
            Color c = Smoke[i].getFillColor();
            if (c.a > 0)
            {
                c.a -= 255 / step;
                
                if (c.b < 230 && c.g < 220 && c.r < 120)
                {
                    c.b += 90 / step;
                    c.g += 120 / step;
                    c.r += 120 / step;
                }
                
            }
            Smoke[i].setFillColor(c);

            if (Smoke[i].getRadius() + 50 / step < 60) //if(Smoke[i].getRadius() + 50 / step < 60)
                Smoke[i].setRadius(Smoke[i].getRadius() + 50 / step);

            if (Smoke[i].getFillColor().a <= 0)
            {
                Smoke.erase(Smoke.begin() + i);
                Psmoke.erase(Psmoke.begin() + i);
            }
    }

    //пули
    for (int i = 0; i < bullets.size(); i++)
    {
        Vector2f b(bullets[i].getPosition());
        Vector2f fin(Pbullets[i]);

        float ck = sqrt(pow(b.x - (fin.x), 2) + pow(b.y - (fin.y), 2));
        b.x += bspeed * ((fin.x) - b.x) / ck;
        b.y += bspeed * ((fin.y) - b.y) / ck;
        bullets[i].setPosition(b.x, b.y);


        Pbullets[i].x += bspeed * ((fin.x) - b.x) / ck;
        Pbullets[i].y += bspeed * ((fin.y) - b.y) / ck;
        float bull_size = bullets[i].getSize().y;

        if (collision(20, E.sprite, bullets[i]) || collision(0, M.sprite, bullets[i]))
        {
            spawn_bum(bullets[i], i);
        }

        if (b.x < view.getCenter().x - (view.getSize().x / 2 + bull_size + 10) || b.x > view.getCenter().x + (view.getSize().x / 2 + bull_size + 10) || b.y < view.getCenter().y - (view.getSize().y / 2 + bull_size + 10) || b.y > view.getCenter().y + (view.getSize().y / 2 + bull_size + 10))
        {
            bullets.erase(bullets.begin() + i);
            Pbullets.erase(Pbullets.begin() + i);
        }
    }

    //скорость игрока
    {
        float Temp = MAX_speed / 200;
        float ck = sqrt(pow(p.x - (p.dx), 2) + pow(p.y - (p.dy), 2));
        speed = Temp * ck;
    }

    //торможение
    if(stop)
    {
        float sk = sqrt(pow(p.dx - (p.x), 2) + pow(p.dy - (p.y), 2));
        if (sk > MAX_speed)
        {
            p.dx += (MAX_speed) * (p.x - p.dx) / sk; //dx += (speed + 2) * (pos.x - dx) / sk;
            p.dy += (MAX_speed) * (p.y - p.dy) / sk; //dy += (speed + 2) * (pos.y - dy) / sk;
        }
        else
            speed = 0;
    }

    p.update();
    view.setCenter(p.x, p.y);
    std::ostringstream NUM;
    NUM << SCORE;
    score.setPosition(view.getCenter().x + view.getSize().x / 2 - 300, view.getCenter().y - view.getSize().y / 2 + 25);
    score.setRotation(view.getRotation());
    score.setString("Score: " + NUM.str());


    //граници мира
    float ck = sqrt(pow(p.x - (E.dx), 2) + pow(p.y - (E.dy), 2));
    if (ck > windowX)
    {
        Vector2f c(p.x, p.y);
        Vector2f fin(p.dx, p.dy);

        float ck = sqrt(pow(c.x - (fin.x), 2) + pow(c.y - (fin.y), 2));
        c.x = p.x - 200 * (fin.x - c.x) / ck;
        c.y = p.y - 200 * (fin.y - c.y) / ck;
        
        p.SetVec(c.x, c.y);
    }
    /////////////////////////////

    anim_fire();
    angelAster += 0.3;
    if (angelAster > 360)
        angelAster = 0;
    //спавн астероидов
    if (Ticks > 100 && Asters.size() < 10)
    {
        Ticks = 0;
        CircleShape A;
        A.setTexture(&Taster[rand() % 4]);
        if (rand() % 100 < RandAsBig)
        {
            A.setRadius(25);
            HpAsters.push_back(HpAsBig);
        }
        else
        {
            A.setRadius(15);
            HpAsters.push_back(HpAs);
        }
        if (rand() % 100 < RandAsBlue)
        {
            A.setFillColor(Color(130, 200, 255));
            HpAsters[HpAsters.back()] += HpAsBlue;
        }
        A.setOrigin(A.getRadius(), A.getRadius());

        Vector2f pos_A(E.dx + windowX * space_count / 2, E.dy + windowX * space_count / 2);
        int Angel_spawn = rand() % 360;
        pos_A.x = E.dx + (windowX * space_count / 2) * cos(Angel_spawn);
        pos_A.y = E.dy + (windowX * space_count / 2) * sin(Angel_spawn);
        A.setPosition(pos_A);
        Asters.push_back(A);
        PAsters.push_back(Vector2f(E.dx, E.dy));

        //спавн дыма астероидов
        Asmoke.resize(Asmoke.size() + 1);
        PAsmoke.resize(PAsmoke.size() + 1);
    }
    //логика астероидов
    for (int i = 0; i < Asters.size(); i++)
    {
        //движение астероидов
        Vector2f pA(Asters[i].getPosition());
        Vector2f fin(PAsters[i]);
        float ck = sqrt(pow(pA.x - (fin.x), 2) + pow(pA.y - (fin.y), 2));
        pA.x += Aspeed * (fin.x - pA.x) / ck;
        pA.y += Aspeed * (fin.y - pA.y) / ck;
        Asters[i].setPosition(pA);
        Asters[i].setRotation(angelAster);

        //Столновение с игроком
        if (collision(Asters[i], p.sprite, -speed))
            gameover = true;
        else//Улетел слишком далеко
            if (length(Asters[i].getPosition().x, Asters[i].getPosition().y, PAsters[i].x, PAsters[i].y) <= 20)
            {
                Asters.erase(Asters.begin() + i);
                PAsters.erase(PAsters.begin() + i);
                HpAsters.erase(HpAsters.begin() + i);
            }
            else//Столновение с землёй
                if (collision(E.sprite, Asters[i], 20))
                {
                    if (Asters[i].getRadius() == 25)
                        SCORE -= 7;
                    else
                        SCORE -= 3;
                    spawn_boom(Asters[i].getPosition(), i);
                    Asters.erase(Asters.begin() + i);
                    PAsters.erase(PAsters.begin() + i);
                    HpAsters.erase(HpAsters.begin() + i);
                }
                else//Столновение с луной
                    if (collision(M.sprite, Asters[i], 0))
                    {
                        spawn_boom(Asters[i].getPosition(), i);
                        Asters.erase(Asters.begin() + i);
                        PAsters.erase(PAsters.begin() + i);
                        HpAsters.erase(HpAsters.begin() + i);
                        //SCORE -= 1;
                    }
                    else//Столновение с пулей
                        for (int j = 0; j < bullets.size(); j++)
                            if (collision(0, Asters[i], bullets[j]))
                            {
                                spawn_bum(bullets[j], j);
                                HpAsters[i] -= damage;
                                if (HpAsters[i] <= 0)
                                {
                                    spawn_boom(Asters[i].getPosition(), i);
                                    if (Asters[i].getRadius() == 25)
                                    {
                                        bool Y;
                                        if (rand() % 10 < 5)
                                            Y = true;
                                        else
                                            Y = false;

                                        Vector2f Pa(Asters[i].getPosition());
                                        Vector2f Paf;
                                        float AN = rand() % 180;
                                        Paf.x = Pa.x + 2000 * cos(AN);
                                        Paf.y = Pa.y + 2000 * sin(AN);
                                        if (Y)
                                            spawn_Asters(Vector2f(Pa.x, Pa.y + 10), 15, 2, Asters[i].getFillColor(), Paf);
                                        else
                                            spawn_Asters(Vector2f(Pa.x + 10, Pa.y), 15, 2, Asters[i].getFillColor(), Paf);
                                        AN = rand() % -180;
                                        Paf.x = Pa.x + 2000 * cos(AN);
                                        Paf.y = Pa.y + 2000 * sin(AN);
                                        if (Y)
                                            spawn_Asters(Vector2f(Pa.x, Pa.y - 10), 15, 2, Asters[i].getFillColor(), Paf);
                                        else
                                            spawn_Asters(Vector2f(Pa.x - 10, Pa.y), 15, 2, Asters[i].getFillColor(), Paf);
                                        //спавн дыма астероидов
                                        Asmoke.resize(Asmoke.size() + 1);
                                        PAsmoke.resize(PAsmoke.size() + 1);

                                        //спавн дыма астероидов
                                        Asmoke.resize(Asmoke.size() + 1);
                                        PAsmoke.resize(PAsmoke.size() + 1);
                                    }
                                    else
                                        if (Asters[i].getFillColor() == Color(130, 200, 255))
                                            SCORE += 5;
                                        else
                                            SCORE += 1;
                                    Asters.erase(Asters.begin() + i);
                                    PAsters.erase(PAsters.begin() + i);
                                    HpAsters.erase(HpAsters.begin() + i);
                                    break;
                                }
                            }
    }

    //спавн дыма астероидов
    if (Ticks2 > 10)
    {
        for (int i = 0; i < Asters.size(); i++)
            if (Asmoke[i].size() < 10)
            {
                spawn_Asmoke(Asters[i].getPosition(), i);
            }
        Ticks2 = 0;
    }
    //логика дыма астероидов
    for (int i = 0; i < Asters.size(); i++)
        for (int j = 0; j < Asmoke[i].size(); j++)
        {
            Vector2f s(Asmoke[i][j].getPosition());
            Vector2f fin(PAsmoke[i][j]);

            float L = length(s.x, s.y, fin.x, fin.y);

            float step = 100;
            Color c = Asmoke[i][j].getFillColor();
            if (c.a > 0)
                c.a -= 255 / step;
            Asmoke[i][j].setFillColor(c);

            if (Asmoke[i][j].getRadius() - 5 / step > 0)
                Asmoke[i][j].setRadius(Asmoke[i][j].getRadius() - 5 / step);

            if (Asmoke[i][j].getFillColor().a <= 0)
            {
                Asmoke[i].erase(Asmoke[i].begin() + j);
                PAsmoke[i].erase(PAsmoke[i].begin() + j);

                if (Asmoke[i].empty())
                    Asmoke.erase(Asmoke.begin() + i);
                if (PAsmoke[i].empty())
                    PAsmoke.erase(PAsmoke.begin() + i);
            }
        }

    //анимация разрушения
    for (int i = 0; i < Dest.size(); i++)
    {
        IntRect rec;
        Anim_dest[i] += 0.5;
        if (Anim_dest[i] < 10)
            rec = { int(Anim_dest[i]) * 128, 0, 128, 128 };

        Dest[i].setTextureRect(rec);
        if (Anim_dest[i] >= 10)
        {
            Dest.erase(Dest.begin() + i);
            Anim_dest.erase(Anim_dest.begin() + i);
        }
    }

    //анимация взрыва
    for (int i = 0; i < Bum.size(); i++)
    {
        IntRect rec;
        Anim_bum[i] += 1;
        if (Anim_bum[i] < 5)
            rec = { int(Anim_bum[i]) * 192, 0, 192, 192 };
        if(Anim_bum[i] > 5 && Anim_bum[i] < 10)
            rec = { (int(Anim_bum[i]) - 5) * 192, 192, 192, 192 };
        if (Anim_bum[i] > 10 && Anim_bum[i] < 15)
            rec = { (int(Anim_bum[i]) - 10) * 192, 384, 192, 192 };
        if (Anim_bum[i] > 15)
            rec = { (int(Anim_bum[i]) - 15) * 192, 576, 192, 192 };

        Bum[i].setTextureRect(rec);
        if (Anim_bum[i] >= 20)
        {
            Bum.erase(Bum.begin() + i);
            Anim_bum.erase(Anim_bum.begin() + i);
        }
    }

    if (Anim_dest.empty() && Anim_bum.empty())
        boom = false;

    //луна
    Angel_moon += 0.001;
    if (Angel_moon > 360)
        Angel_moon = 0;

    //Столновение луны и земли с игроком
    if (collision(M.sprite, p.sprite, -speed) || collision(E.sprite, p.sprite, speed + 10))
        gameover = true;

    Vector2f pos_moon(M.dx, M.dy);
    pos_moon.x = E.dx + (E.radius + dist_moon) * cos(Angel_moon);
    pos_moon.y = E.dy + (E.radius + dist_moon) * sin(Angel_moon);
    M.SetPosition(pos_moon.x, pos_moon.y);

    E.sprite.setRotation(E.sprite.getRotation() + 0.01);

    chekMusic();
}

int main()
{
    srand(time(NULL));
    Start();

    Clock clock;
    float timer = 0;
    float timerLoading = 0;
    double delay = 0.005;
    
    while (window.isOpen())
    {
        Restart();
        while (window.isOpen() && !gameover)
        {
            while (window.pollEvent(event))
            {
                if (event.type == Event::Closed)
                    window.close();
                if (event.type == Event::Resized)
                {
                    fullscreen = true;
                    bullets.clear();
                    Pbullets.clear();
                    int windowX = modes[0].width;
                    int windowY = modes[0].height;
                    window.create(VideoMode(windowX, windowY, modes[0].bitsPerPixel), "Defense of the planet", 8, ContextSettings());
                    window.setFramerateLimit(60);
                }
                if (!isLoading)
                    keyboard();
                window.setMouseCursor(cursor);
            }

            if (!isLoading)
            {
                Vector2i pixelPos = Mouse::getPosition(window);//забираем коорд курсора
                pos = window.mapPixelToCoords(pixelPos);//переводим их в игровые (уходим от коорд окна)
                Vector2f c(p.x, p.y);
                Vector2f fin(pos);
                float ck = sqrt(pow(c.x - (fin.x), 2) + pow(c.y - (fin.y), 2));
                c.x = p.x + 200 * (fin.x - c.x) / ck;
                c.y = p.y + 200 * (fin.y - c.y) / ck;

                cross.setPosition(c);

                float time = clock.getElapsedTime().asSeconds();
                clock.restart();
                timer += time;
                if (timer > delay) { timer = 0; Tick(); }
            }
            else
            {
                float time = clock.getElapsedTime().asSeconds();
                clock.restart();
                timerLoading += time;
                double delay = 0.005;
                if (timerLoading > delay)
                {
                    timerLoading = 0;
                    ColorLoading -= 3;
                    Loading.setFillColor(Color(0, 0, 0, ColorLoading));
                }
                if (ColorLoading <= 0)
                    isLoading = false;
            }

            view.setSize(windowX, windowY);
            window.setView(view);
            window.clear();
            DrawGame();
            window.display();
        }
    }
}