#include "WaveFunctionCollapse2D.h"
#include <iostream>

WaveFunctionCollapse2D::WaveFunctionCollapse2D() : WaveFunctionCollapse2D(0, 0) {}
WaveFunctionCollapse2D::WaveFunctionCollapse2D(int size_x, int size_y) :
	is_complete(false),
	is_drawing(false),
	is_rotation(false),
	random(true)
{
	setSize(size_x, size_y);
	tex_void.loadFromFile("tiles\\void.png");
}

void WaveFunctionCollapse2D::addTile(std::string filename, bool rotation)
{
	Tile tile;
	tile.image_name = filename;
	tile.is_rotate = rotation;
	tile.real_tile_id = count_tiles;
	tiles.push_back(tile);
	count_tiles++;
}

void WaveFunctionCollapse2D::setSize(int wight, int height)
{
	size.x = wight;
	size.y = height;
	grid.resize(wight * height);
}

sf::Vector2i WaveFunctionCollapse2D::getSize()
{
	return size;
}

sf::Vector2i WaveFunctionCollapse2D::getSizeTile()
{
	return sf::Vector2i(drawing_tiles[0].sprite.getSize());
}

void WaveFunctionCollapse2D::setIsDrawing(bool is_drawing, float tile_wight, float tile_height)
{
	this->is_drawing = is_drawing;
	if (!is_drawing)
		return;

	drawing_tiles.resize(size.x * size.y);

	
	for (int i = 0; i < size.x; i++)
		for (int j = 0; j < size.y; j++)
		{
			drawing_tiles[i + j * size.x].sprite.setSize(sf::Vector2f(tile_wight, tile_height));
			drawing_tiles[i + j * size.x].sprite.setOrigin(tile_wight/2, tile_height/2);
			drawing_tiles[i + j * size.x].sprite.setPosition(i * tile_wight + tile_wight/2, j * tile_height + tile_wight / 2);
			drawing_tiles[i + j * size.x].texture = tex_void;
			drawing_tiles[i + j * size.x].sprite.setTexture(&drawing_tiles[i + j * size.x].texture);
		}
}

void WaveFunctionCollapse2D::setRotations(bool is_rotation)
{
	this->is_rotation = is_rotation;
}

void WaveFunctionCollapse2D::setTileInGrid(int tile_id, unsigned x, unsigned y, int rotation)
{
	if (x > size.x)
	{
		std::cout << "Error: " << x << " over grid wight" << std::endl;
		return;
	}
	if (y > size.y)
	{
		std::cout << "Error: " << y << " over grid height" << std::endl;
		return;
	}

	if (tile_id == -1)
	{
		int index = (x + y * size.x);
		std::vector<int> cell_options;
		for (int i = 0; i < tiles.size(); i++)
			cell_options.push_back(i);


		grid[index].collapsed = false;
		grid[index].options = cell_options;

		drawing_tiles[index].texture = tex_void;
		drawing_tiles[index].sprite.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), (sf::Vector2i)drawing_tiles[index].texture.getSize()));
		drawing_tiles[index].is_set = false;
		drawing_tiles[index].sprite.setRotation(0);

		for(int i = 0; i < user_grid.size(); i++)
			if (user_grid[i].id == index)
			{
				user_grid.erase(user_grid.begin() + i);
				break;
			}

		return;
	}

	if (tile_id < tiles.size())
	{
		int index = (x + y * size.x);

		int real_index;
		if (rotation == 0)
			real_index = tile_id;
		else
			real_index = (count_tiles + (tile_id * 3)) + (rotation / 90 - 1);


		if (!grid[index].collapsed)
		{
			grid[index].collapsed = true;

			Cell cell;
			cell.id = index;
			cell.options.push_back(real_index);
			grid[index].options = cell.options;

			cell.options.push_back(tile_id);
			cell.options.push_back(rotation);

			user_grid.push_back(cell);
		}
		else
		{
			for (int i = 0; i < user_grid.size(); i++)
				if (user_grid[i].id == index)
				{
					user_grid[i].options[0] = real_index;
					user_grid[i].options[1] = tile_id;
					user_grid[i].options[2] = rotation;
					break;
				}
		}

		drawing_tiles[index].texture = tiles_texture[tile_id];
		drawing_tiles[index].sprite.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), (sf::Vector2i)drawing_tiles[index].texture.getSize()));
		drawing_tiles[index].sprite.setRotation(rotation);
		drawing_tiles[index].is_set = true;
		return;
	}
	std::cout << "Error: tile - " << tile_id << " not found" << std::endl;
}

void WaveFunctionCollapse2D::clearPrevTiles()
{
	user_grid.clear();
}

void WaveFunctionCollapse2D::create(float& progress)
{
	if (random)
	{
		srand(std::time(NULL));
	}
	else
		srand(seed);

	tiles_texture.resize(tiles.size());
	for (int i = 0; i < tiles.size(); i++)
	{
		tiles_texture[i].loadFromFile(tiles[i].image_name);
	}

	if (is_rotation)
		createRotationsConnections(progress);
	else
		createConnections(progress);

	// create grid
	std::vector<int> cell_options;
	for (int i = 0; i < tiles.size(); i++)
			cell_options.push_back(i);

	for (int i = 0; i < size.x; i++)
		for (int j = 0; j < size.y; j++)
		{
			grid[i + j * size.x].id = (i + j * size.x);
			if (!grid[i + j * size.x].collapsed)
				grid[i + j * size.x].options = cell_options;
		}

	update();
}

void chekValid(std::vector<int>& options, std::vector<int>& valid)
{
	std::vector<int> result;
	for (int i = 0; i < options.size(); i++)
	{
		for (int j = 0; j < valid.size(); j++)
		{
			if (options[i] == valid[j])
			{
				result.push_back(options[i]);
				break;
			}
		}
	}
	options = result;
}


bool chekArr(std::vector<int>& arr, int value)
{
	for (int i = 0; i < arr.size(); i++)
	{
		if (arr[i] == value)
			return true;
	}
	return false;
}

int comp(const void* a, const void* b)
{
	return (static_cast<const Cell*>(a)->options.size() - static_cast<const Cell*>(b)->options.size());
}

void WaveFunctionCollapse2D::update()
{
	if (is_complete)
	{
		return;
	}

	std::vector<Cell> grid_copy;
	for (int i = 0; i < grid.size(); i++)
	{
		if (!grid[i].collapsed)
		{
			grid_copy.push_back(grid[i]);
		}
	}

	if (grid_copy.size() == 0)
	{
		is_complete = true;
		return;
	}

	qsort((void*)grid_copy.data(), grid_copy.size(), sizeof(Cell), comp);

	int len = grid_copy[0].options.size();
	int stop_index = 0;
	for (int i = 1; i < grid_copy.size(); i++)
	{
		if (grid_copy[i].options.size() > len)
		{
			stop_index = i;
			break;
		}
	}

	if (stop_index > 0)
		grid_copy.erase(grid_copy.begin() + stop_index, grid_copy.end());


	Cell cell = grid_copy[rand() % grid_copy.size()];
	cell.collapsed = true;
	if (cell.options.size() != 0)
	{
		int pick = cell.options[rand() % cell.options.size()];
		cell.options = std::vector<int>{ pick };

		grid[cell.id] = cell;
	}
	else
	{
		is_failed = true;
	}

	std::vector<Cell> nextGrid;
	nextGrid.resize(size.x * size.y);

	for (int j = 0; j < size.y; j++)
	{
		for (int i = 0; i < size.x; i++)
		{
			int index = (i + j * size.x);
			if (grid[index].collapsed)
			{
				nextGrid[index] = grid[index];
				if (is_drawing && !drawing_tiles[index].is_set)
				{
					int id = grid[index].options[0];
					drawing_tiles[index].texture = tiles_texture[tiles[id].real_tile_id];
					if(tiles[id].is_rotate)
						drawing_tiles[index].sprite.setRotation(tiles[id].rotaion * 90);
					drawing_tiles[index].sprite.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), (sf::Vector2i)drawing_tiles[index].texture.getSize()));
					drawing_tiles[index].is_set = true;
				}
			}
			else
			{
				std::vector<int> options = grid[index].options;
				
				// Look up
				if (j > 0)
				{
					std::vector<int> valid_options;
					Cell up = grid[i + (j - 1) * size.x];
					for (int option : up.options)
					{
						for(int k = 0; k < tiles[option].connections[2].size(); k++)
							if(!chekArr(valid_options, tiles[option].connections[2][k]))
								valid_options.push_back(tiles[option].connections[2][k]);
					}

					chekValid(options, valid_options);
				}


				// Look right
				if (i < size.x - 1)
				{
					std::vector<int> valid_options;
					Cell right = grid[(i + 1) + j * size.x];
					for (int option : right.options)
					{
						for (int k = 0; k < tiles[option].connections[3].size(); k++)
							if (!chekArr(valid_options, tiles[option].connections[3][k]))
								valid_options.push_back(tiles[option].connections[3][k]);
					}

					chekValid(options, valid_options);
				}

				// Look down
				if (j < size.y - 1)
				{
					std::vector<int> valid_options;
					Cell down = grid[i + (j + 1) * size.x];
					for (int option : down.options)
					{
						for (int k = 0; k < tiles[option].connections[0].size(); k++)
							if (!chekArr(valid_options, tiles[option].connections[0][k]))
								valid_options.push_back(tiles[option].connections[0][k]);
					}

					chekValid(options, valid_options);
				}


				// Look left
				if (i > 0)
				{
					std::vector<int> valid_options;
					Cell left = grid[(i - 1) + j * size.x];
					for (int option : left.options)
					{
						for (int k = 0; k < tiles[option].connections[1].size(); k++)
							if (!chekArr(valid_options, tiles[option].connections[1][k]))
								valid_options.push_back(tiles[option].connections[1][k]);
					}

					chekValid(options, valid_options);
				}

				nextGrid[index].options = options;
				nextGrid[index].collapsed = false;
				nextGrid[index].id = index;
			}

		}
	}

	/*
	for (int i = 0; i < nextGrid.size(); i++)
	{
		printf("cell[%d]: ", i);
		for (int j = 0; j < nextGrid[i].options.size(); j++)
			std::cout << nextGrid[i].options[j] << " ";

		std::cout << std::endl;
	}
	std::cout << std::endl;
	*/
	grid = nextGrid;
}

void WaveFunctionCollapse2D::clear()
{
	is_complete = false;
	is_failed = false;
	grid.clear();
	grid.resize(size.x * size.y);
	std::vector<int> cell_options;
	for (int i = 0; i < tiles.size(); i++)
		cell_options.push_back(i);

	for (int i = 0; i < size.x; i++)
		for (int j = 0; j < size.y; j++)
		{
			int index = (i + j * size.x);
			grid[index].collapsed = false;
			grid[index].options = cell_options;
			grid[index].id = index;

			drawing_tiles[index].texture = tex_void;
			drawing_tiles[index].sprite.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), (sf::Vector2i)drawing_tiles[index].texture.getSize()));
			drawing_tiles[index].is_set = false;
			drawing_tiles[index].sprite.setRotation(0);
		}

	for (int i = 0; i < user_grid.size(); i++)
	{
		int index = user_grid[i].id;
		grid[index].options.resize(1);
		grid[index].options[0] = user_grid[i].options[0];
		grid[index].collapsed = true;

		drawing_tiles[index].texture = tiles_texture[user_grid[i].options[1]];
		drawing_tiles[index].sprite.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), (sf::Vector2i)drawing_tiles[index].texture.getSize()));
		drawing_tiles[index].sprite.setRotation(user_grid[i].options[2]);
		drawing_tiles[index].is_set = true;

	}
}

void WaveFunctionCollapse2D::showConnections()
{
	for (int i = 0; i < tiles.size(); i++)
	{
		std::cout << "Tile[" << i << ":" << tiles[i].rotaion * 90 << "]:\n";

		for (int e = 0; e < 4; e++)
			switch (e)
			{
			case 0:
				std::cout << "   UP: [";
				for (int j = 0; j < tiles[i].connections[e].size(); j++)
				{
					std::cout << tiles[i].connections[e][j];
					if (j != tiles[i].connections[e].size() - 1)
						std::cout << ", ";
				}
				std::cout << "]" << std::endl;
				break;
			case 1:
				std::cout << "   RIGHT: [";
				for (int j = 0; j < tiles[i].connections[e].size(); j++)
				{
					std::cout << tiles[i].connections[e][j];
					if (j != tiles[i].connections[e].size() - 1)
						std::cout << ", ";
				}
				std::cout << "]" << std::endl;
				break;
			case 2:
				std::cout << "   DOWN: [";
				for (int j = 0; j < tiles[i].connections[e].size(); j++)
				{
					std::cout << tiles[i].connections[e][j];
					if (j != tiles[i].connections[e].size() - 1)
						std::cout << ", ";
				}
				std::cout << "]" << std::endl;
				break;
			case 3:
				std::cout << "   LEFT: [";
				for (int j = 0; j < tiles[i].connections[e].size(); j++)
				{
					std::cout << tiles[i].connections[e][j];
					if (j != tiles[i].connections[e].size() - 1)
						std::cout << ", ";
				}
				std::cout << "]" << std::endl;
				break;
			}
	}
}

void WaveFunctionCollapse2D::draw(sf::RenderWindow& window)
{
	if (is_drawing)
	{
		for (int i = 0; i < drawing_tiles.size(); i++)
		{
			window.draw(drawing_tiles[i].sprite);
		}
	}
	else
	{
		std::cout << "Error: drawing set False" << std::endl;
	}
}

void WaveFunctionCollapse2D::setSeed(int seed)
{
	random = false;
	this->seed = seed;
	srand(seed);
}

int WaveFunctionCollapse2D::getSeed()
{
	return seed;
}

int* WaveFunctionCollapse2D::generateGrid(int attempts)
{
	is_drawing = false;
	float progress = 0;
	create(progress);

	while (!is_complete)
	{
		if (!is_failed)
			update();
		else
		{
			clear();
			attempts--;
			if (attempts < 0)
				return nullptr;
		}
	}
	
	int* res_grid = new int[grid.size()];
	for (int i = 0; i < grid.size(); i++)
		res_grid[i] = grid[i].options[0];

	return res_grid;
}

/// private ///

void getPixels(Edge edge, std::string& filename, std::vector<sf::Color>& pixels, int rotates = 0)
{
	sf::Image image;
	image.loadFromFile(filename);
	pixels.clear();
	switch (edge)
	{
	case Edge::UP:
		if (rotates != 3 && rotates != 2)
		{
			for (int p_x = 0; p_x < image.getSize().x; p_x++)
				pixels.push_back(image.getPixel(p_x, 0));
		}
		else
		{
			for (int p_x = image.getSize().x-1; p_x >= 0; p_x--)
				pixels.push_back(image.getPixel(p_x, 0));
		}
		break;
	case Edge::RIGHT:
		if (rotates != 2)
		{
			for (int p_y = 0; p_y < image.getSize().y; p_y++)
				pixels.push_back(image.getPixel(image.getSize().x - 1, p_y));
		}
		else
		{
			for (int p_y = image.getSize().y - 1; p_y >= 0; p_y--)
				pixels.push_back(image.getPixel(image.getSize().x - 1, p_y));
		}
		break;
	case Edge::DOWN:
		if (rotates != 3 && rotates != 2)
		{
			for (int p_x = 0; p_x < image.getSize().x; p_x++)
				pixels.push_back(image.getPixel(p_x, image.getSize().y - 1));
		}
		else
		{
			for (int p_x = image.getSize().x - 1; p_x >= 0; p_x--)
				pixels.push_back(image.getPixel(p_x, image.getSize().y - 1));
		}
		break;
	case Edge::LEFT:
		if (rotates != 1 && rotates != 2)
		{
			for (int p_y = 0; p_y < image.getSize().y; p_y++)
				pixels.push_back(image.getPixel(0, p_y));
		}
		else
		{
			for (int p_y = image.getSize().y - 1; p_y >= 0; p_y--)
				pixels.push_back(image.getPixel(0, p_y));
		}
		break;
	}
}

void getPixels(int edge, std::string& filename, std::vector<sf::Color>& pixels, int rotates = 0)
{
	if (edge < 0)
		edge = 4 + edge;

	switch (edge)
	{
	case 0:
		return getPixels(Edge::UP, filename, pixels, rotates);
		break;
	case 1:
		return getPixels(Edge::RIGHT, filename, pixels, rotates);
		break;
	case 2:
		return getPixels(Edge::DOWN, filename, pixels, rotates);
		break;
	case 3:
		return getPixels(Edge::LEFT, filename, pixels, rotates);
		break;
	}
}

bool chekPixels(std::vector<sf::Color>& first, std::vector<sf::Color>& second)
{
	for (int i = 0; i < first.size(); i++)
	{
		if (first[i] != second[i])
			return false;
	}
	return true;
}

void WaveFunctionCollapse2D::createConnections(float& progress)
{
	float _progress = 0;
	for (int i = 0; i < tiles.size(); i++)
	{
		for (int j = 0; j < tiles.size(); j++)
		{
			_progress++;
			// create UP //
			std::vector<sf::Color> up_pixels, down_pixels, right_pixels, left_pixels;

			getPixels(Edge::UP, tiles[i].image_name, up_pixels);
			getPixels(Edge::DOWN, tiles[j].image_name, down_pixels);

			if (chekPixels(up_pixels, down_pixels))
				tiles[i].connections[0].push_back(j);

			// create RIGHT //
			getPixels(Edge::RIGHT, tiles[i].image_name, right_pixels);
			getPixels(Edge::LEFT, tiles[j].image_name, left_pixels);

			if (chekPixels(right_pixels, left_pixels))
				tiles[i].connections[1].push_back(j);

			// create DOWN //
			getPixels(Edge::DOWN, tiles[i].image_name, down_pixels);
			getPixels(Edge::UP, tiles[j].image_name, up_pixels);

			if (chekPixels(down_pixels, up_pixels))
				tiles[i].connections[2].push_back(j);

			// create LEFT //
			getPixels(Edge::LEFT, tiles[i].image_name, left_pixels);
			getPixels(Edge::RIGHT, tiles[j].image_name, right_pixels);

			if (chekPixels(left_pixels, right_pixels))
				tiles[i].connections[3].push_back(j);

			progress = _progress / float(tiles.size() * tiles.size()) * 100.0;
		}
	}
}

void WaveFunctionCollapse2D::createRotationsConnections(float& progress)
{
	int tile_count = tiles.size();
	for (int i = 0; i < tile_count; i++)
	{
		Tile tile;
		if (tiles[i].is_rotate)
		{
			for (int r = 1; r < 4; r++)
			{
				tile.image_name = tiles[i].image_name;
				tile.rotaion = r;
				tile.real_tile_id = i;
				tiles.push_back(tile);
			}
		}
	}

	float _progress = 0;
	for (int i = 0; i < tiles.size(); i++)
	{
		for (int j = 0; j < tiles.size(); j++)
		{
			_progress++;
			int i_rotation = tiles[i].rotaion;
			int j_rotation = tiles[j].rotaion;
			// create UP //
			std::vector<sf::Color> up_pixels, down_pixels, right_pixels, left_pixels;
			int up = 0, right = 1, down = 2, left = 3;

			getPixels(up - i_rotation, tiles[i].image_name, up_pixels, i_rotation);
			getPixels(down - j_rotation, tiles[j].image_name, down_pixels, j_rotation);

			if (chekPixels(up_pixels, down_pixels))
				tiles[i].connections[0].push_back(j);

			// create RIGHT //
			getPixels(right - i_rotation, tiles[i].image_name, right_pixels, i_rotation);
			getPixels(left - j_rotation, tiles[j].image_name, left_pixels, j_rotation);

			if (chekPixels(right_pixels, left_pixels))
				tiles[i].connections[1].push_back(j);

			// create DOWN //
			getPixels(down - i_rotation, tiles[i].image_name, down_pixels, i_rotation);
			getPixels(up - j_rotation, tiles[j].image_name, up_pixels, j_rotation);

			if (chekPixels(down_pixels, up_pixels))
				tiles[i].connections[2].push_back(j);

			// create LEFT //
			getPixels(left - i_rotation, tiles[i].image_name, left_pixels, i_rotation);
			getPixels(right - j_rotation, tiles[j].image_name, right_pixels, j_rotation);

			if (chekPixels(left_pixels, right_pixels))
				tiles[i].connections[3].push_back(j);

			progress = _progress / float(tiles.size() * tiles.size()) * 100.0;
		}
	}
}

void WaveFunctionCollapse2D::showLoadProgress(bool load)
{
	load_proggress = load;
}