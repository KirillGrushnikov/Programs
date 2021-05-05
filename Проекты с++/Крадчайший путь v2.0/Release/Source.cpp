#include "SFML/Graphics.hpp"
#include <iostream>
#include <windows.h>
#include <time.h>
#include <list>
#include <vector>


using namespace sf;

const int g_Size = 32;
const int windowX = 1024;
const int windowY = 704;
const int mapX = windowX / g_Size;
const int mapY = windowY / g_Size;
Color c_block = Color( 0, 0, 128 );
Event event;
RenderWindow window(VideoMode(windowX, windowY), "Navigator", Style::Default, ContextSettings());

char map[mapX][mapY];
RectangleShape Square[mapX*mapY];
CircleShape Circle_lane[mapX * mapY];

int start = 0;
int finish = (mapX * mapY)-1;
bool clik = false;

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

int Graph::chek(int x, int y)
{
	if (x > 0 && x < mapX && y > 0 && y < mapY)
		return (x * mapY + y);
	else
		return false;
}

void Set_map()
{
	Vector2f vec(g_Size, g_Size);
	int k = 0;
	for (int i = 0; i < mapX; i++)
		for (int j = 0; j < mapY; j++)
		{
			if(rand() % 100 < 20)
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
	k = 0;
	for (int i = 0; i < mapX; i++)
		for (int j = 0; j < mapY; j++)
		{
			if (map[i][j] == '1')
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

void draw_VBFS()
{
	int start = 0;

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
	Vector2i pixelPos = Mouse::getPosition(window);//забираем коорд курсора
	Vector2f pos = window.mapPixelToCoords(pixelPos);//переводим их в игровые (уходим от коорд окна)
	int k = 0;

	for (int i = 0; i < mapX; i++)
		for (int j = 0; j < mapY; j++)
		{
			if (pos.x > Circle_lane[k].getPosition().x && pos.x < Circle_lane[k].getPosition().x + g_Size && pos.y > Circle_lane[k].getPosition().y && pos.y < Circle_lane[k].getPosition().y + g_Size)
			{
				if (GetAsyncKeyState(2) * 0x1)
					finish = k;
				if (GetAsyncKeyState(1) * 0x1)
					start = k;
			}
			else
			{
				Circle_lane[k].setFillColor(Color(0, 0, 0, 0));
			}
			k++;
		}
}

void draw_lane()
{
	for (int i = 0; i < mapX * mapY; i++)
		window.draw(Circle_lane[i]);
}

int main()
{
	srand(time(0));

	Set_map();
	Create_Graf();
	//draw_VBFS();
	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		mouse();

		//draw_VBFS();
		draw_BFS();

		window.clear();
		draw_map();
		draw_lane();
		window.display();
	}
	return 0;
}