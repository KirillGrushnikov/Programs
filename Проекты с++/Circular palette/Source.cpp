#include "SFML/Graphics.hpp"
#include <locale>
#include <time.h>
#include <Windows.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <glut.h>

using namespace std;
using namespace sf;

int windowX = 500;
int windowY = 300;
unsigned style = 4;
bool Rainbow = false;
float X = 0;

RenderWindow window(VideoMode(windowX, windowY), "Circular palette", style, ContextSettings());
Event event;
View view;
Vector2f pos;
Color background(30, 30, 30);

float CountLines = 1440;
float Step = 360/CountLines;
vector<RectangleShape> lines;
RectangleShape line;
RectangleShape White;
VertexArray Triangular(PrimitiveType::Triangles, 3);
CircleShape Midle;
CircleShape User1;
CircleShape User2;

Font MyFont;
Text TRGB[4];

int what_copy = -1;
bool copyed = false, Ctrl = false, Pip = false;

Color UserColor1(255, 0, 0);
Color UserColor2(255, 0, 0);
RectangleShape UserColor;
float Ur = 255, Ug = 0, Ub = 0;
int Select = 0;
int sch = 0, sch2 = 0;
float timer = 0, timer2 = 0;
int SizeG = 60;
VertexArray Triangular2(PrimitiveType::Triangles, 3);

void Start()
{
    window.setFramerateLimit(60);
    Image im;
    im.loadFromFile("palete.png");
    window.setIcon(im.getSize().x, im.getSize().y, im.getPixelsPtr());

    MyFont.loadFromFile("Font\\20930.ttf");
    for(int i = 0; i < 4; i++)
        TRGB[i].setFont(MyFont);
    TRGB[0].setString("255"); TRGB[1].setString("0"); TRGB[2].setString("0");
    TRGB[0].setPosition(300, 155); TRGB[1].setPosition(370, 155); TRGB[2].setPosition(440, 155);
    TRGB[3].setFillColor(Color(0, 0, 0, 0)); TRGB[3].setPosition(300, 200); TRGB[3].setString(L"скопировано");

    line.setPosition(windowY / 2, windowY / 2);
    line.setSize(Vector2f(1, 130));

    float r = 255, g = 0, b = 0;
    for(float i = 0; i < 360; i+= Step)
    {
        float N = 4.25;
        line.setRotation(i + 180);
        if (i <= 60)
            g = i * N;
        if (i > 60 && i <= 120)
            r = (i - 60) * -N;
        if (i > 120 && i <= 180)
            b = (i - 120) * N;
        if (i > 180 && i <= 240)
            g = (i - 180) * -N;
        if (i > 240 && i <= 300)
            r = (i - 240) * N;
        if (i > 300 && i <= 360)
            b = (i - 300) * -N;
        line.setFillColor(Color(r, g, b));
        lines.push_back(line);
    }

    Midle.setFillColor(background);
    Midle.setRadius(110);
    Midle.setOrigin(Midle.getRadius(), Midle.getRadius());
    Midle.setPosition(line.getPosition());
    Midle.setPointCount(1440);

    User1.setFillColor(Color::White); 
    User1.setRadius(2); 
    User1.setOrigin(User1.getRadius(), User1.getRadius()); 
    User1.setPosition(line.getPosition().x, line.getPosition().y - 135);

    int Size = 178;
    User2.setFillColor(Color(0, 0, 0, 0));
    User2.setRadius(6);
    User2.setOrigin(User2.getRadius(), User2.getRadius());
    User2.setOutlineThickness(2);
    User2.setPosition(line.getPosition().x, line.getPosition().y);

    UserColor.setPosition(300, 50);
    UserColor.setSize(Vector2f(190, 100));
    UserColor.setFillColor(UserColor1);

    Triangular[0].color = Color(255, 0, 0);
    Triangular[1].color = Color(0, 0, 0);
    Triangular[2].color = Color(255, 255, 255);
    Triangular[0].position = Vector2f(line.getPosition().x, line.getPosition().y - 103);
    Vector2f c(Triangular[0].position);
    Vector2f p;
    float Angel = 60;
    Angel *= 3.14 / 180;
    p.x = c.x + Size * cos(Angel);
    p.y = c.y + Size * sin(Angel);
    Triangular[1].position = Vector2f(p.x, p.y);
    p.x = c.x + Size * cos(3.14 - Angel);
    p.y = c.y + Size * sin(3.14 - Angel);
    Triangular[2].position = Vector2f(p.x, p.y);

    Triangular2 = Triangular;
    Triangular2[0].position = Vector2f(line.getPosition().x, line.getPosition().y - 109);
    c = Triangular2[0].position;
    Size = 185;
    p.x = c.x + Size * cos(Angel);
    p.y = c.y + Size * sin(Angel);
    Triangular2[1].position = Vector2f(p.x, p.y);
    p.x = c.x + Size * cos(3.14 - Angel);
    p.y = c.y + Size * sin(3.14 - Angel);
    Triangular2[2].position = Vector2f(p.x, p.y);
}

float length(Vector2f p1, Vector2f p2)
{
    float L = pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2);
    return abs(sqrt(L));
}

void Pipette()
{
    keybd_event(VK_SNAPSHOT, 0, 0, 0);

    if (OpenClipboard(NULL))
    {
        POINT Pos;
        GetCursorPos(&Pos);
        HBITMAP hbit = (HBITMAP)GetClipboardData(CF_BITMAP);
        HDC dc = CreateCompatibleDC(NULL);
        SelectObject(dc, hbit);
        COLORREF color = GetPixel(dc, Pos.x, Pos.y);
        UserColor2 = Color(GetRValue(color), GetGValue(color), GetBValue(color));
        UserColor.setFillColor(UserColor2);


        int Ucr = 0, Ucg = 0, Ucb = 0;
        Ucr <<= 8;
        Ucr |= UserColor2.r;
        Ucg <<= 8;
        Ucg |= UserColor2.g;
        Ucb <<= 8;
        Ucb |= UserColor2.b;

        ostringstream Red;
        Red << Ucr;
        ostringstream Green;
        Green << Ucg;
        ostringstream Blue;
        Blue << Ucb;
        TRGB[0].setString(Red.str()); TRGB[1].setString(Green.str()); TRGB[2].setString(Blue.str());
        Ur = Ucr; Ug = Ucg; Ub = Ucb;

        CloseClipboard();
        DeleteDC(dc);
    }
}

void Copy()
{
    string source; //в эту переменную нужно записать текст, который в дальнейшем поместится в буфер обмена

    switch (what_copy)
    {
    case 1:
        source = TRGB[0].getString(); TRGB[3].setFillColor(Color::Red);
        break;
    case 2:
        source = TRGB[1].getString(); TRGB[3].setFillColor(Color::Green);
        break;
    case 3:
        source = TRGB[2].getString(); TRGB[3].setFillColor(Color::Blue);
        break;
    case 4:
        source = TRGB[0].getString() + ", " + TRGB[1].getString() + ", " +  TRGB[2].getString(); TRGB[3].setFillColor(Color::White);
        break;
    default:
        return;
        break;
    }
        //запись текста в буфер обмена
    if (OpenClipboard(NULL) && what_copy > 0)//открываем буфер обмена
    {
        copyed = true;
        HGLOBAL hgBuffer;
        char* chBuffer;
        EmptyClipboard(); //очищаем буфер
        hgBuffer = GlobalAlloc(GMEM_DDESHARE, source.length() + 1);//выделяем память
        chBuffer = (char*)GlobalLock(hgBuffer); //блокируем память
        strcpy_s(chBuffer, source.length() + 1, LPCSTR(source.c_str()));
        GlobalUnlock(hgBuffer);//разблокируем память
        SetClipboardData(CF_TEXT, hgBuffer);//помещаем текст в буфер обмена
        CloseClipboard(); //закрываем буфер обмена
    }
}

bool Chek(Vector2f a, Vector2f b, Vector2f c, Vector2f t)
{
    a.y += 1;
    b.x -= 2; b.y -= 2;
    c.x += 1; c.y -= 2;
    float A = (a.x - t.x) * (b.y - a.y) - (b.x - a.x) * (a.y - t.y);
    float B = (b.x - t.x) * (c.y - b.y) - (c.x - b.x) * (b.y - t.y);
    float C = (c.x - t.x) * (a.y - c.y) - (a.x - c.x) * (c.y - t.y);

    if (A >= 0 && B >= 0 && C >= 0)
       return true;

    return false;
}

int Chek2(Vector2f a, Vector2f b, Vector2f c, Vector2f t)
{
    float A = (a.x - t.x) * (b.y - a.y) - (b.x - a.x) * (a.y - t.y);
    float B = (b.x - t.x) * (c.y - b.y) - (c.x - b.x) * (b.y - t.y);
    float C = (c.x - t.x) * (a.y - c.y) - (a.x - c.x) * (c.y - t.y);

    if (A > 0 && B > 0 && C > 0)
        return 0;
    if (A < 0 && B > 0 && C > 0)
        return 1;
    if(A > 0 && B < 0 && C > 0)
        return 2;
    if (A > 0 && B > 0 && C < 0)
        return 3;


    return -1;
}

Vector2f ChekUser(Vector2f chek)
{
    float s = 0.5;
    switch (Chek2(Triangular[0].position, Triangular[1].position, Triangular[2].position, chek))
    {
    case 0:
        User2.setPosition(pos);
        break;
    case 1:
        if (chek.y > Triangular[0].position.y && chek.y < Triangular[1].position.y)
            User2.setPosition(Triangular[0].position.x + length(User2.getPosition(), Triangular[0].position) * s, chek.y);
        break;
    case 3:
        if (chek.y > Triangular[0].position.y && chek.y < Triangular[2].position.y)
            User2.setPosition(Triangular[0].position.x - length(User2.getPosition(), Triangular[0].position) * s, chek.y);
        break;
    }
    if(chek.y < Triangular[0].position.y)
        User2.setPosition(Triangular[0].position.x, Triangular[0].position.y + 1);

    if (chek.x > Triangular[2].position.x && chek.x < Triangular[1].position.x && chek.y > Triangular[1].position.y)
        User2.setPosition(chek.x, Triangular[1].position.y - 1);

    return User2.getPosition();
}

void mouse()
{
    if (GetAsyncKeyState(1))
    {
        if (length(line.getPosition(), pos) <= 130 && length(line.getPosition(), pos) >= 110 && Select == 0)
            Select = 1;
        if (Chek(Triangular[0].position, Triangular[1].position, Triangular[2].position, pos) && Select == 0)
            Select = 2;
    }
    else
    {
        what_copy = 0;
        Select = 0;
    }

    if (GetAsyncKeyState(2))
    {
        Vector2f R(TRGB[0].getPosition()); float Rs = TRGB[0].getCharacterSize();
        Vector2f G(TRGB[1].getPosition()); float Gs = TRGB[1].getCharacterSize();
        Vector2f B(TRGB[2].getPosition()); float Bs = TRGB[2].getCharacterSize();
        Vector2f Rgb(UserColor.getPosition()); Vector2f Rgbs(UserColor.getSize());
        if (pos.x > R.x && pos.x < G.x - 10 && pos.y > R.y && pos.y < R.y + Rs)
            what_copy = 1;
        if (pos.x > G.x - 10 && pos.x < B.x - 10 && pos.y > G.y && pos.y < G.y + Gs)
            what_copy = 2;
        if (pos.x > B.x - 10 && pos.x < B.x + 70 && pos.y > B.y && pos.y < B.y + Bs)
            what_copy = 3;
        if (pos.x > Rgb.x && pos.x < Rgb.x + Rgbs.x && pos.y > Rgb.y && pos.y < Rgb.y + Rgbs.y)
            what_copy = 4;

        timer2 = 0;
        Copy();
    }
}

void keyboard()
{
    if (Keyboard::isKeyPressed(Keyboard::Space))
    {
        if (sch == 0)
        {
            if (!Rainbow)
                Rainbow = true;
            else
                Rainbow = false;

            sch++;
        }
    }

    if (GetAsyncKeyState(27)) //esc выход
        exit(0);
}

void SetColor()
{
    Vector2f fin(User2.getPosition());
    HDC dc = GetDC(window.getSystemHandle());
    COLORREF color = GetPixel(dc, fin.x, fin.y);
    ReleaseDC(window.getSystemHandle(), dc);
    UserColor2 = Color(GetRValue(color), GetGValue(color), GetBValue(color));
    UserColor.setFillColor(UserColor2);

    int Ucr = 0, Ucg = 0, Ucb = 0;
    Ucr <<= 8;
    Ucr |= UserColor2.r;
    Ucg <<= 8;
    Ucg |= UserColor2.g;
    Ucb <<= 8;
    Ucb |= UserColor2.b;

    ostringstream Red;
    Red << Ucr;
    ostringstream Green;
    Green << Ucg;
    ostringstream Blue;
    Blue << Ucb;
    TRGB[0].setString(Red.str()); TRGB[1].setString(Green.str()); TRGB[2].setString(Blue.str());
    Ur = Ucr; Ug = Ucg; Ub = Ucb;
}

void Round()
{
    if (Rainbow)
    {
        Vector2f c(line.getPosition());
        Vector2f fin(400 * cos(X), 400 * sin(X));
        float ck = sqrt(pow(c.x - (fin.x), 2) + pow(c.y - (fin.y), 2));
        fin.x = c.x + 135 * (fin.x - c.x) / ck;
        fin.y = c.y + 135 * (fin.y - c.y) / ck;
        User1.setPosition(fin);

        fin.x = 400 * cos(X); fin.y = 400 * sin(X);
        ck = sqrt(pow(c.x - (fin.x), 2) + pow(c.y - (fin.y), 2));
        fin.x = c.x + 120 * (fin.x - c.x) / ck;
        fin.y = c.y + 120 * (fin.y - c.y) / ck;
        HDC dc = GetDC(window.getSystemHandle());
        COLORREF color = GetPixel(dc, fin.x, fin.y);
        ReleaseDC(NULL, dc);
        UserColor1 = Color(GetRValue(color), GetGValue(color), GetBValue(color));
        Triangular[0].color = Color(GetRValue(color), GetGValue(color), GetBValue(color));
        Triangular2[0].color = Triangular[0].color;
        X+=0.1;
        if (X > 360)
            X = 0;
    }
}

void Tick()
{
    if (Select == 1)
    {
        SetColor();
        Vector2f c(line.getPosition());
        Vector2f fin;
        float ck = sqrt(pow(c.x - (pos.x), 2) + pow(c.y - (pos.y), 2));
        fin.x = c.x + 135 * (pos.x - c.x) / ck;
        fin.y = c.y + 135 * (pos.y - c.y) / ck;
        User1.setPosition(fin);

        fin.x = c.x + 120 * (pos.x - c.x) / ck;
        fin.y = c.y + 120 * (pos.y - c.y) / ck;
        HDC dc = GetDC(window.getSystemHandle());
        COLORREF color = GetPixel(dc, fin.x, fin.y);
        ReleaseDC(NULL, dc);
        UserColor1 = Color(GetRValue(color), GetGValue(color), GetBValue(color));
        Triangular[0].color = Color(GetRValue(color), GetGValue(color), GetBValue(color));
        Triangular2[0].color = Triangular[0].color;

    }

    if (Select == 2)
    {
        SetColor();
        ChekUser(pos);
    }
}

int main()
{
    setlocale(LC_ALL, "Russian");
    srand(std::time(NULL));
    Start();

    Clock clock;
    double delay = 0.005;

    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            //ShowCursor(false);
            keyboard();

            Vector2i pixelPos = Mouse::getPosition(window);//забираем коорд курсора
            pos = window.mapPixelToCoords(pixelPos);//переводим их в игровые (уходим от коорд окна)

            mouse();
        }

        if (GetAsyncKeyState(17) && GetAsyncKeyState(16))
            Pipette();

        Tick();

        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;
        if (timer > delay) { timer = 0; sch = 0; sch2 = 0; Round(); }
        if(copyed) timer2 += time;
        if (timer2 > 1) { timer2 = 0; copyed = false; TRGB[3].setFillColor(Color(0, 0, 0, 0)); }

        //window.setView(view);
        window.clear(background);
        for (int i = 0; i < lines.size(); i++)
            window.draw(lines[i]);
        window.draw(Midle);
        window.draw(Triangular2);
        window.draw(Triangular);
        window.draw(User1);
        window.draw(User2);
        window.draw(UserColor);
        for(int i = 0; i < 4; i++)
            window.draw(TRGB[i]);
        window.display();
    }
}