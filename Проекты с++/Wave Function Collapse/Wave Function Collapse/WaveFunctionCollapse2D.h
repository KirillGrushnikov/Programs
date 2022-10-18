#pragma once
#include "SFML/Graphics.hpp"
#include <vector>

enum class Edge
{
	UP,
	RIGHT,
	DOWN,
	LEFT
};

struct DTile
{
	sf::Texture texture;
	sf::RectangleShape sprite;
	int rotaion = 0;
	bool is_set = false;
};

struct Tile
{
	std::string image_name;
	std::vector<int> connections[4]; // свази (UP: [0 - тайлом, 1, ...], RIGHT: [...], ...)
	int rotaion = 0;
	bool is_rotate = true;
	int real_tile_id;
};


struct Cell
{
	int id;
	bool collapsed;
	std::vector<int> options;

	Cell() : collapsed(false), id(-1) {}
};

class WaveFunctionCollapse2D
{
private:
	std::vector<Cell> grid;
	std::vector<Cell> user_grid;
	sf::Vector2i size;

	std::vector<Tile> tiles;
	std::vector<DTile> drawing_tiles;
	std::vector<sf::Texture> tiles_texture;
	sf::Texture tex_void;

	bool is_drawing;
	bool is_rotation;
	bool random;

	int seed;
	void createConnections(float& progress);
	void createRotationsConnections(float& progress);

	bool load_proggress;
	int count_tiles;
public:
	bool is_complete;
	bool is_failed;
	WaveFunctionCollapse2D();
	WaveFunctionCollapse2D(int, int);

	void addTile(std::string filename, bool rotation = true);

	void setSize(int, int);
	sf::Vector2i getSize();

	sf::Vector2i getSizeTile();

	void setIsDrawing(bool is_drawing, float tile_wight = 0, float tile_height = 0);

	void setRotations(bool);

	void setTileInGrid(int tile_id, unsigned x, unsigned y, int rotation = 0);
	void clearPrevTiles();
	void create(float& progress);
	void update();
	void clear();

	void setSeed(int);
	int getSeed();

	void showLoadProgress(bool);

	int* generateGrid(int attempts = 3);

	void showConnections();

	void draw(sf::RenderWindow&);
};

