#include "SFML/Graphics.hpp"
#include <iostream>
#include <windows.h>
#include <time.h>
#include <list>
#include <vector>
#include "Maze.h"

using namespace sf;

const int g_Size = 32;
const int windowX = 1056;
const int windowY = 736;
const int mapX = windowX / g_Size;
const int mapY = windowY / g_Size;
Color c_block = Color( 0, 0, 128 );
Event event;
RenderWindow window(VideoMode(windowX, windowY), "Navigator", Style::Default, ContextSettings());
Vector2f pos;

char map[mapX][mapY];
RectangleShape Square[mapX*mapY];
CircleShape Circle_lane[mapX * mapY];

int start = mapY+1;
int finish = (mapX * mapY) - (mapY + 2);
bool clik = false;
bool autorun = false;
bool MAZE = true;
bool chek = false;
int cur_node_run;
int* vis = nullptr;
int x = 1, y = 1;
RectangleShape player;

class Graph
{
	int numVertices;
	std::list<int>* adjLists;
	int* visited;
	int* _VISITED;
	int* prev;
public:
	Graph(int vertices);
	void addEdge(int src, int dest);
	void clearEdge(int src, int dest);
	int chek(int x, int y);
	//Поиск в ширину
	int* BFS(int startVertex, int finish)
	{
		visited = new int[numVertices];
		_VISITED = new int[numVertices];
		for (int i = 0; i < numVertices; i++)
		{
			visited[i] = startVertex;
			_VISITED[i] = startVertex;
		}
		//visited[startVertex] = startVertex;
		//_VISITED[startVertex] = startVertex;

		std::list<int> queue;
		queue.push_back(startVertex);
		std::list<int>::iterator i;
		int j = 1;
		while (!queue.empty())
		{
			int currVertex = queue.front();
			if (currVertex == finish)
				break;
			queue.pop_front();

			for (i = adjLists[currVertex].begin(); i != adjLists[currVertex].end(); ++i)
			{
				int adjVertex = *i;
				int k = 0;
				if (visited[adjVertex] != adjVertex)
				{
					visited[adjVertex] = adjVertex;
					_VISITED[adjVertex] = currVertex;
					queue.push_back(adjVertex);
					j++;
				}
			}
		}
		return _VISITED;
	}
	void VBFS(int x, int y)
	{

	}
};

Graph g(mapX* mapY);

Graph::Graph(int vertices)
{
	numVertices = vertices;
	adjLists = new std::list<int>[vertices];
}

void Graph::addEdge(int src, int dest)
{
	adjLists[src].push_back(dest);
	adjLists[dest].push_back(src);
}

void Graph::clearEdge(int src, int dest)
{
	adjLists[src].clear();
	adjLists[dest].clear();
}

void Set_map()
{
	Vector2f vec(g_Size, g_Size);
	int k = 0;
	if (!MAZE)
	{
		for (int i = 0; i < mapX; i++)
			for (int j = 0; j < mapY; j++)
			{
				if (rand() % 100 < 20)
					map[i][j] = '1';
				else
					map[i][j] = '0';
				Vector2f position(i * g_Size, j * g_Size);
				Square[k].setSize(vec);
				Square[k].setPosition(position);
				Square[k].setOutlineColor(Color::Black);
				Square[k].setOutlineThickness(1);
				Circle_lane[k].setRadius(g_Size / 2);
				Circle_lane[k].setPosition(position);
				Circle_lane[k].setFillColor(Color(0, 0, 0, 0));
				k++;
			}
	}
	else
	{
		CreateMaze(mapX, mapY);
		for (int i = 0; i < mapX; i++)
			for (int j = 0; j < mapY; j++)
			{
				Vector2f position(i * g_Size, j * g_Size);
				Square[k].setSize(vec);
				Square[k].setPosition(position);
				Square[k].setOutlineColor(Color::Black);
				Square[k].setOutlineThickness(1);
				Circle_lane[k].setRadius(g_Size / 2);
				Circle_lane[k].setPosition(position);
				Circle_lane[k].setFillColor(Color(0, 0, 0, 0));
				k++;
			}
	}
	k = 0;
	for (int i = 0; i < mapX; i++)
		for (int j = 0; j < mapY; j++)
		{
			if (maze[i][j] == 0)
				Square[k].setFillColor(c_block);
			else
				Square[k].setFillColor(Color(0, 128, 0));
			k++;
		}
}

void draw_map()
{
	for (int i = 0; i < mapX*mapY; i++)
			window.draw(Square[i]);
}

void get_next(int k)
{
	int ways[4][2] = { -mapY, 0,   0, -1,   mapY, 0,   0, 1};
	for (int i = 0; i < 4; i++)
		if (k + ways[i][0] >= 0 && k + ways[i][1] >= 0 && k + ways[i][0] <= mapX * mapY - 22 && k + ways[i][1] <= mapY * mapX - 1 && Square[k + ways[i][0]].getFillColor() != c_block && Square[k + ways[i][1]].getFillColor() != c_block)
			g.addEdge(k + ways[i][0], k + ways[i][1]);
	for (int i = mapY * 2 - 1; i <= mapX * mapY - mapY - 1; i += mapY)
	{
		g.clearEdge(i, i + 1);
		if (Square[i].getFillColor() != c_block)
		{
			g.addEdge(i, i - mapY);
			g.addEdge(i - 1, i);
		}
		if (Square[i + 1].getFillColor() != c_block)
		{
			g.addEdge(i - (mapY - 1), i + 1);
			g.addEdge(i + 1, i + 2);
		}
	}

	for (int i = mapX * mapY - mapY; i < mapX * mapY - 1; i++)
	{
		if (Square[i].getFillColor() != c_block)
			g.addEdge(i, i + 1);
	}

	g.clearEdge(mapY - 1, mapY);
	g.clearEdge(0, mapY);
	if (Square[mapY].getFillColor() != c_block)
		g.addEdge(0, mapY);
	if (Square[mapY + 1].getFillColor() != c_block)
		g.addEdge(mapY, mapY + 1);
	if (Square[mapY * 2].getFillColor() != c_block)
		g.addEdge(mapY, mapY * 2);
	if (Square[0].getFillColor() != c_block)
		g.addEdge(0, 1);
	if (Square[mapY - 1].getFillColor() != c_block)
		g.addEdge(mapY - 2, mapY - 1);
	if (Square[mapY * 2 - 1].getFillColor() != c_block)
		g.addEdge(mapY - 1, mapY * 2 - 1);
	if (Square[mapX * mapY - 1].getFillColor() != c_block)
		g.addEdge(mapX * mapY - mapY - 1, mapX * mapY - 1);
}

void Create_Graf()
{
	for (int i = 0; i < mapX*mapY; i++)
		get_next(i);
}

//отрисовка результата поиск в ширину
void draw_BFS()
{
	int* visited;
	int k = 0;

	visited = g.BFS(start, finish);

	int cur_node = finish;
	while (cur_node != start)
	{
		//std::cout << cur_node << "--->";
		cur_node = visited[cur_node];
		Circle_lane[cur_node].setFillColor(Color::White);
	}
	//std::cout << cur_node << " ";
	Circle_lane[start].setFillColor(Color(240, 0, 150));
	Circle_lane[finish].setFillColor(Color::Red);
}

void mouse()
{
	int k = 0;

	for (int i = 0; i < mapX; i++)
		for (int j = 0; j < mapY; j++)
		{
			if (pos.x > Circle_lane[k].getPosition().x && pos.x < Circle_lane[k].getPosition().x + g_Size && pos.y > Circle_lane[k].getPosition().y && pos.y < Circle_lane[k].getPosition().y + g_Size)
			{
				if (GetAsyncKeyState(2) & 0x1 && !autorun)
					start = k;
				if (GetAsyncKeyState(1) & 0x1)
				{
					if (autorun)
					{
						start = k;
						finish = y + x * mapY;
						vis = g.BFS(start, finish);
						cur_node_run = finish;
					}
					else
						finish = k;
				}
			}
			else
			{
				Circle_lane[k].setFillColor(Color(0, 0, 0, 0));
			}
			k++;
		}
}

void keyboard()
{

	if (GetAsyncKeyState(83) & 0x1 || Keyboard::isKeyPressed(Keyboard::Down)) // S
	{
		if (maze[x][y + 1] != 0)
		{
			y += 1;
			player.setPosition(x * g_Size, y * g_Size);
		}
	}

	if (GetAsyncKeyState(65) & 0x1 || Keyboard::isKeyPressed(Keyboard::Left)) // A
	{
		if (maze[x - 1][y] != 0)
		{
			x -= 1;
			player.setPosition(x * g_Size, y * g_Size);
		}
	}

	if (GetAsyncKeyState(68) & 0x1 || Keyboard::isKeyPressed(Keyboard::Right)) // D
	{
		if (maze[x + 1][y] != 0)
		{
			x += 1;
			player.setPosition(x * g_Size, y * g_Size);
		}
	}

	if (GetAsyncKeyState(87) & 0x1 || Keyboard::isKeyPressed(Keyboard::Up)) // W
	{
		if (maze[x][y - 1] != 0)
		{
			y -= 1;
			player.setPosition(x * g_Size, y * g_Size);
		}
	}


	if (GetAsyncKeyState(32) & 0x1)
	{
		if (!clik)
			clik = true;
		else
			clik = false;
	}

	if (GetAsyncKeyState(17) & 0x1)
	{
		if (!autorun)
			autorun = true;
		else
		{
			autorun = false;
			chek = false;
			int temp = finish;
			finish = start;
			start = temp;
		}
	}

	if (GetAsyncKeyState(27)) //esc выход
		exit(0);
}

void draw_lane()
{
	for (int i = 0; i < mapX * mapY; i++)
		window.draw(Circle_lane[i]);
}

void Tick()
{
	if (autorun)
	{
		if (!chek)
		{
			start = finish;
			finish = y + x * mapY;
			vis = g.BFS(start, finish);
			cur_node_run = finish;
			chek = true;
		}

		if(cur_node_run != start)
		{
			x = Circle_lane[cur_node_run].getPosition().x / g_Size;
			y = Circle_lane[cur_node_run].getPosition().y / g_Size;
			cur_node_run = vis[cur_node_run];
			player.setPosition(x * g_Size, y * g_Size);
			finish = y + x * mapY;
		}
		else
		{
			x = Circle_lane[cur_node_run].getPosition().x / g_Size;
			y = Circle_lane[cur_node_run].getPosition().y / g_Size;
			player.setPosition(x * g_Size, y * g_Size);
			finish = y + x * mapY;
			autorun = false;
			chek = false;
		}
	}
}

int main()
{
	srand(std::time(NULL));
	window.setFramerateLimit(60);
	Image im;
	im.loadFromFile("Maze.png");
	window.setIcon(im.getSize().x, im.getSize().y, im.getPixelsPtr());

	player.setFillColor(Color::Red);
	player.setSize(Vector2f(g_Size, g_Size));
	player.setPosition(x * g_Size, y * g_Size);

	Set_map();
	Create_Graf();

	Clock clock;
	float timer = 0;
	double delay = 0.05;

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			switch (event.type)
			{
				// ¬ращение средней кнопки Ц колеса мыши
			case sf::Event::MouseWheelMoved:
				// delta Ц значение в какую сторону вращаем колесо ( 1 up , -1 down )
				if (event.mouseWheel.delta == 1)
				{
					delay *= 0.7;
					if (delay < 0.005) delay = 0.005;
				}
				else if (event.mouseWheel.delta == -1)
				{
					delay *= 1.5;
					if (delay > 0.3) delay = 0.3;
				}

				break;
			}
			keyboard();
			Vector2i pixelPos = Mouse::getPosition(window);//забираем коорд курсора
			pos = window.mapPixelToCoords(pixelPos);//переводим их в игровые (уходим от коорд окна)
		}
		mouse();

		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;
		if (timer > delay) { timer = 0; Tick(); }

		draw_BFS();

		window.clear();
		draw_map();
		if (clik)
			draw_lane();

		window.draw(player);
		window.display();
	}
	return 0;
}