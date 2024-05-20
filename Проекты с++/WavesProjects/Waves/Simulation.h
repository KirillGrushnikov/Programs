#pragma once
#include "SFML/Graphics.hpp"
#include "Block.h"
#include "Types.h"
#include "SuperEllipse.hpp"
#include "Logs.h"


#include <vector>

#include "sLine.h"

class Simulation
{
private:
	struct Connection
	{
		int index;
		std::vector<int> neighbors;
	};

	std::vector<Block> blocks;
	VertexArray vertex_lines;
	std::vector<sf::LineShape> lines;
	std::vector<sf::CircleShape> points;


	std::vector<Connection> connections;

	int count_blocks;

	DynamicParams* gui_params;

	SupEllipseShape brash;


	bool is_load;
	std::vector<Block> load_blocks;
	std::vector<Connection> load_connections;
	int load_count_blocks;

	void connect(std::vector<Connection>& connections, int index1, int index2);
	void connect(std::vector<Connection>& connections, int index1, std::vector<int> indexs);
public:
	Simulation(DynamicParams* dynamic_params);

	int getCountBlocks() const;

	sf::Vector2f getSelectPosition() const;
	sf::Vector2f getSelectVelosity() const;
	bool getSelectFixed() const;
	sf::Color getSelectColor() const;
	float getSelectMass() const;
	std::vector<int> getSelectNeighbors() const;

	void restart();
	void reset2Default();

	void saveMap(const char* filename) const;
	void loadMap(const char* filename);

	void update();

	void draw(sf::RenderWindow& window);
};

