#include "SFML/Graphics.hpp"
#include <locale>
#include <time.h>
#include <Windows.h>
#include <iostream>
#include <vector>
#include <sstream>
#include "Button.hpp"
#include "sfml_resources.h"

using namespace std;
using namespace sf;

int windowX = 500;
int windowY = 300;
unsigned style = 4;
bool Rainbow = false, sharp = true;
float X = 0;

RenderWindow window(VideoMode(windowX, windowY), "Circular palette", style, ContextSettings());
Event event;
View view;
Vector2f pos;
Color background(30, 30, 30);

HBITMAP hbit;

float CountLines = 1440;
double Step = 360/CountLines;
vector<RectangleShape> lines;
RectangleShape line;
RectangleShape White;
VertexArray Triangular(PrimitiveType::Triangles, 3);
CircleShape Midle;
CircleShape User1;
CircleShape User2;

Font MyFont;
Text TRGB[4];
Button buttons_colors[3];
Button button_copy;

int what_copy = 4;
bool copyed = false, Ctrl = false, Pip = false;

Color UserColor1(255, 0, 0);
Color UserColor2(255, 0, 0);
float Ur = 255, Ug = 0, Ub = 0;
int Select = 0;
int sch = 0, sch2 = 0;
float timer = 0, timer2 = 0;
int SizeG = 60;
VertexArray Triangular2(PrimitiveType::Triangles, 3);

static bool windowIsActive(HWND hWnd_window)
{
    HWND hWnd;
    hWnd = GetFocus();
    if (hWnd == hWnd_window)
        return true;
    else
        return false;
}

HMODULE GCM()
{
    HMODULE hmodule = NULL;
    GetModuleHandleEx(
        GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
        (LPCTSTR)GCM,
        &hmodule);
    return hmodule;
}


void Start()
{
    HMODULE handle = GCM();
    HRSRC rc = FindResource(handle, MAKEINTRESOURCE(ICON), MAKEINTRESOURCE(ICONFILE));
    HGLOBAL rcData = LoadResource(handle, rc);
    DWORD size = SizeofResource(handle, rc);
    const char* data = static_cast<const char*>(LockResource(rcData));

    Image im;
    im.loadFromMemory(data, size);

    window.setFramerateLimit(60);
    window.setIcon(im.getSize().x, im.getSize().y, im.getPixelsPtr());

    rc = FindResource(handle, MAKEINTRESOURCE(FONT), MAKEINTRESOURCE(FONTFILE));
    rcData = LoadResource(handle, rc);
    size = SizeofResource(handle, rc);
    data = static_cast<const char*>(LockResource(rcData));

    MyFont.loadFromMemory(data, size);

    for(int i = 0; i < 4; i++)
        TRGB[i].setFont(MyFont);
    TRGB[0].setString("255"); TRGB[1].setString("0"); TRGB[2].setString("0");

    int x = 300, y = 155;
    TRGB[0].setPosition(x, y); x += 70; TRGB[1].setPosition(x, y); x += 70; TRGB[2].setPosition(x, y);
    TRGB[0].setOutlineThickness(1); TRGB[1].setOutlineThickness(1); TRGB[2].setOutlineThickness(1);

    TRGB[3].setFillColor(Color(0, 0, 0, 0)); TRGB[3].setPosition(300, 200); TRGB[3].setString(L"скопировано");


    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            buttons_colors[i].SetFillColor(Color(30, 30, 30, 0), j);
            buttons_colors[i].SetOutlineThickness(2, j);
        }

        buttons_colors[i].SetOutlineColor(Color(30, 30, 30), 0);
        buttons_colors[i].SetOutlineColor(Color(125, 125, 125), 1);
        buttons_colors[i].SetOutlineColor(Color(255, 255, 255), 2);
        buttons_colors[i].SetOutlineColor(Color(0, 0, 0, 0), 3);

        buttons_colors[i].SetPosition(Vector2f(300 + i * 70, 165));
        buttons_colors[i].SetNM(buttons_colors[i].GetSize().x, buttons_colors[i].GetSize().y);
        buttons_colors[i].SetFont(MyFont);
        buttons_colors[i].SetTextFillColor(Color::White);
        buttons_colors[i].SetTextPosition(Vector2f(buttons_colors[i].GetPosition().x, buttons_colors[i].GetPosition().y - 9));
    }


    button_copy.SetPosition(Vector2f(300, 50));
    button_copy.SetSize(Vector2f(190, 100));
    button_copy.SetNM(button_copy.GetSize().x, button_copy.GetSize().y);
    for (int i = 0; i < 3; i++)
    {
        button_copy.SetFillColor(UserColor1, i);
        button_copy.SetOutlineThickness(2, i);
    }
    button_copy.SetOutlineColor(Color(30, 30, 30), 0);
    button_copy.SetOutlineColor(Color(125, 125, 125), 1);
    button_copy.SetOutlineColor(Color(255, 255, 255), 2);
    button_copy.SetOutlineColor(Color(0, 0, 0, 0), 3);

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

string DecHex(int ch)
{
    string s;
    int d;
    char m[17] = { "0123456789ABCDEF" };
    if (ch == 0) { s = "00"; return(s); }
    if (ch < 16) 
    { 
        d = ch % 16; 
        s = "0";
        s += m[d];
        return(s);
    }
    while (ch)
    {
        d = ch % 16;
        ch /= 16;
        s = m[d] + s;
    }
    return(s);
}

string NormCol(int ch)
{
    string s;
    float k = 1.0 / 255.0;
    float res = float(ch) * k;
    s = to_string(res);
    s.resize(4);
    s[1] = '.';
    return s;
}


HBITMAP ScreenDC(HWND DC)
{
    HDC hScreenDC = GetDC(DC);
    HDC hMemoryDC = CreateCompatibleDC(hScreenDC);
    int width = GetDeviceCaps(hScreenDC, HORZRES);
    int height = GetDeviceCaps(hScreenDC, VERTRES);
    HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, width, height);
    HBITMAP hOldBitmap = static_cast<HBITMAP>(SelectObject(hMemoryDC, hBitmap));
    BitBlt(hMemoryDC, 0, 0, width, height, hScreenDC, 0, 0, SRCCOPY);
    hBitmap = static_cast<HBITMAP>(SelectObject(hMemoryDC, hOldBitmap));
    DeleteDC(hMemoryDC);
    DeleteDC(hScreenDC);
    return hBitmap;
}

void SaveBitMapToClipboard(const HBITMAP& hBitmap)
{
    //запись в буфер обмена
    if (OpenClipboard(NULL))//открываем буфер обмена
    {
        EmptyClipboard(); //очищаем буфер
        SetClipboardData(CF_BITMAP, hBitmap);//помещаем в буфер обмена
        CloseClipboard(); //закрываем буфер обмена
    }
}

void SaveBitMap(const HBITMAP& hBitmap)
{
    hbit = hBitmap;
}

void Pipette()
{
    //keybd_event(VK_SNAPSHOT, 0, 0, 0);
    POINT Pos;
    GetCursorPos(&Pos);
    HDC dc = CreateCompatibleDC(NULL);
    SelectObject(dc, hbit);
    COLORREF color = GetPixel(dc, Pos.x, Pos.y);
    UserColor2 = Color(GetRValue(color), GetGValue(color), GetBValue(color));

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

    Ur = Ucr; Ug = Ucg; Ub = Ucb;
    DeleteDC(dc);

    /*
    if (OpenClipboard(NULL))
    {
        POINT Pos;
        GetCursorPos(&Pos);
        HBITMAP hbit = (HBITMAP)GetClipboardData(CF_BITMAP);
        HDC dc = CreateCompatibleDC(NULL);
        SelectObject(dc, hbit);
        COLORREF color = GetPixel(dc, Pos.x, Pos.y);
        UserColor2 = Color(GetRValue(color), GetGValue(color), GetBValue(color));


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
        
        Ur = Ucr; Ug = Ucg; Ub = Ucb;

        CloseClipboard();
        DeleteDC(dc);
    }
    */
}

void Copy(int copy = -1)
{
    string source; //в эту переменную нужно записать текст, который в дальнейшем поместится в буфер обмена

    switch (copy)
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
    case 5:
        if (sharp)
        {
            source = "#" + DecHex(Ur) + DecHex(Ug) + DecHex(Ub); TRGB[3].setFillColor(Color::White);
        }
        else
        {
            source = DecHex(Ur) + DecHex(Ug) + DecHex(Ub); TRGB[3].setFillColor(Color::White);
        }
        break;
    case 6:
        source = NormCol(Ur) + ", " + NormCol(Ug) + ", " + NormCol(Ub); TRGB[3].setFillColor(Color::White);
        break;
    default:
        return;
        break;
    }
        //запись текста в буфер обмена
    if (OpenClipboard(NULL) && copy > 0)//открываем буфер обмена
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
        Select = 0;
    }
    
}

void keyboard(Button &button_sharp)
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

    if (GetAsyncKeyState(49))
    {
        what_copy = 4;
        int x = 300, y = 165;
        for (int i = 0; i < 3; i++)
        {
            buttons_colors[i].SetPosition(Vector2f(x + i * 70, y));
            buttons_colors[i].SetTextPosition(Vector2f(buttons_colors[i].GetPosition().x, buttons_colors[i].GetPosition().y - 9));
        }
        button_sharp.SetTexture(Button::block);
        button_sharp.SetVisible(false);
    }

    if (GetAsyncKeyState(50))
    {
        what_copy = 5;
        int x = 310, y = 165;
        for (int i = 0; i < 3; i++)
        {
            buttons_colors[i].SetPosition(Vector2f(x + i * 70, y));
            buttons_colors[i].SetTextPosition(Vector2f(buttons_colors[i].GetPosition().x, buttons_colors[i].GetPosition().y - 9));
        }
        button_sharp.SetTexture(Button::peace);
        button_sharp.SetVisible(true);
    }

    if (GetAsyncKeyState(51))
    {
        what_copy = 6;
        int x = 285, y = 165;
        for (int i = 0; i < 3; i++)
        {
            buttons_colors[i].SetPosition(Vector2f(x + i * 70, y));
            buttons_colors[i].SetTextPosition(Vector2f(buttons_colors[i].GetPosition().x, buttons_colors[i].GetPosition().y - 9));
        }
        button_sharp.SetTexture(Button::block);
        button_sharp.SetVisible(false);
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
    switch (what_copy)
    {
    case 4:
        TRGB[0].setString(to_string(UserColor2.r)); TRGB[1].setString(to_string(UserColor2.g)); TRGB[2].setString(to_string(UserColor2.b));
        break;
    case 5:
        TRGB[0].setString(DecHex(UserColor2.r)); TRGB[1].setString(DecHex(UserColor2.g)); TRGB[2].setString(DecHex(UserColor2.b));
        break;
    case 6:
        TRGB[0].setString(NormCol(UserColor2.r)); TRGB[1].setString(NormCol(UserColor2.g)); TRGB[2].setString(NormCol(UserColor2.b));
        break;
    }

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

    Button button_sharp;
    button_sharp.SetPosition(Vector2f(288, 165));
    button_sharp.SetSize(Vector2f(18, 20));
    button_sharp.SetNM(button_sharp.GetSize().x, button_sharp.GetSize().y);
    for (int i = 0; i < 3; i++)
    {
        button_sharp.SetFillColor(Color(0,0,0,0), i);
        button_sharp.SetOutlineThickness(2, i);
    }
    button_sharp.SetOutlineColor(Color(30, 30, 30), 0);
    button_sharp.SetOutlineColor(Color(125, 125, 125), 1);
    button_sharp.SetOutlineColor(Color(255, 255, 255), 2);
    button_sharp.SetOutlineColor(Color(0, 0, 0, 0), 3);
    button_sharp.SetFont(MyFont);
    button_sharp.SetText("#");
    button_sharp.SetTextFillColor(Color::White);
    button_sharp.SetTextPosition(Vector2f(button_sharp.GetPosition().x, button_sharp.GetPosition().y - 9));
    button_sharp.SetVisible(false);

    Clock clock;
    double delay = 0.005;

    bool button_release = true;

    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            //ShowCursor(false);
            keyboard(button_sharp);

            Vector2i pixelPos = Mouse::getPosition(window);//забираем коорд курсора
            pos = window.mapPixelToCoords(pixelPos);//переводим их в игровые (уходим от коорд окна)

            mouse();
        }
        if (windowIsActive(window.getSystemHandle()))
        {
            if (GetAsyncKeyState(17) && GetAsyncKeyState(16))
            {
                if (!button_release)
                    SaveBitMap(ScreenDC(NULL));
                button_release = true;
                Pipette();
            }
            else
                button_release = false;

            for (int i = 0; i < 3; i++)
                button_copy.SetFillColor(UserColor2, i);

            string col_r = TRGB[0].getString();
            string col_g = TRGB[1].getString();
            string col_b = TRGB[2].getString();


            if (col_r.size() < 4)
            {
                buttons_colors[0].SetSize(Vector2f(18 * col_r.size(), 20));
                buttons_colors[1].SetSize(Vector2f(18 * col_g.size(), 20));
                buttons_colors[2].SetSize(Vector2f(18 * col_b.size(), 20));
            }
            else
            {
                buttons_colors[0].SetSize(Vector2f(18 * col_r.size()-9, 20));
                buttons_colors[1].SetSize(Vector2f(18 * col_g.size()-9, 20));
                buttons_colors[2].SetSize(Vector2f(18 * col_b.size()-9, 20));
            }

            for (int i = 0; i < 3; i++)
                buttons_colors[i].SetNM(buttons_colors[i].GetSize().x, buttons_colors[i].GetSize().y);

            buttons_colors[0].SetText(col_r);
            buttons_colors[1].SetText(col_g);
            buttons_colors[2].SetText(col_b);
            
            for (int i = 0; i < 3; i++)
                if(buttons_colors[i].ChekClik(pos, event))
                    Copy(i+1);

            if (button_copy.ChekClik(pos, event))
                Copy(what_copy);

            if (button_sharp.ChekClik(pos, event))
                sharp = !sharp;

            if (sharp)
            {
                button_sharp.SetTextFillColor(Color::White);
            }
            else
            {
                button_sharp.SetTextFillColor(Color(100, 100, 100));
            }
            Tick();

            float time = clock.getElapsedTime().asSeconds();
            clock.restart();
            timer += time;
            if (timer > delay) { timer = 0; sch = 0; sch2 = 0; Round(); }
            if (copyed) timer2 += time;
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
            button_copy.Draw(window);

            for (int i = 0; i < 3; i++)
                buttons_colors[i].Draw(window);

            button_sharp.Draw(window);

            window.draw(TRGB[3]);
            window.display();
        }
    }
}