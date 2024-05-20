#include "Simulation.h"
#include <fstream>

#define PI 3.1415926535

void Simulation::connect(std::vector<Connection>& connections, int index1, int index2)
{
	if (index1 < 0 || index1 > blocks.size())
		return;

	if (index2 < 0 || index2 > blocks.size())
		return;

	connections[index1].neighbors.push_back(index2);
	connections[index2].neighbors.push_back(index1);
}

void Simulation::connect(std::vector<Connection>& connections, int index1, std::vector<int> indexs)
{
	for (int i = 0; i < indexs.size(); i++)
		connect(connections, index1, indexs[i]);
}

Simulation::Simulation(DynamicParams* dynamic_params) : gui_params(dynamic_params)
{
	restart();

	brash.setOutlineThickness(1);
	brash.setOutlineColor(sf::Color(200, 200, 200));
	brash.setFillColor(sf::Color(255, 255, 255, 100));
	vertex_lines.setPrimitiveType(sf::PrimitiveType::LinesStrip);
}



int Simulation::getCountBlocks() const
{
	return count_blocks;
}

sf::Vector2f Simulation::getSelectPosition() const
{
	return blocks[gui_params->block_select_id].getPosition();
}

sf::Vector2f Simulation::getSelectVelosity() const
{
	return blocks[gui_params->block_select_id].getVelosity();
}

bool Simulation::getSelectFixed() const
{
	return blocks[gui_params->block_select_id].isFixed();
}

sf::Color Simulation::getSelectColor() const
{
	return blocks[gui_params->block_select_id].getColor();
}

float Simulation::getSelectMass() const
{
	return blocks[gui_params->block_select_id].getMass();
}

std::vector<int> Simulation::getSelectNeighbors() const
{
	return connections[gui_params->block_select_id].neighbors;
}



void Simulation::restart()
{
	logPrint("Simulation restart\n");
	if (is_load)
	{
		blocks = load_blocks;
		connections = load_connections;
		count_blocks = load_count_blocks;
		return;
	}

	points.clear();
	blocks.clear();
	connections.clear();
	count_blocks = 182;
	for (int i = 0; i < count_blocks; i++)
	{
		Block block;
		block.setRenderSize(5, 5);
		block.setPosition(50 + i * (block.getRenderSize().x), 360);
		
		/*
		// Wave >>
		if (i >= 1 && i <= 10)
		{
			float f = (2*-PI)/20;
			float s = 200 / 10;
			//block.setPosition(50 + i * (block.getRenderSize().x), 360 + sin(i*f)*100); // Sigmoid
			block.setNewVelosity(0, sin(i * f) * i* s);
		}
		
		
		// Wave <<
		if (i >= count_blocks-11 && i <= count_blocks-2)
		{
			float t = i - (count_blocks - 1);
			float f = (2 * PI) / 20;
			float s = 100 / 10;
			block.setNewVelosity(0, sin(t * f) * t * s);
		}
		*/
		
		if (i == 0 || i == count_blocks - 1)
			block.setFixed(true);

		block.setMass(3);

		blocks.push_back(block);

		Connection connect;
		connect.index = i;
		connections.push_back(connect);
	}

	for (int i = 0; i < count_blocks - 1; i++)
		connect(connections, i, i + 1);



}

void Simulation::reset2Default()
{
	is_load = false;
	restart();
	logPrint("Simulation reset to default\n");
}



void Simulation::saveMap(const char* filename) const
{
	std::ofstream file;
	string path = "Saves\\";
	string sfilename = filename;
	file.open(path + sfilename, std::ios::out);
	if (!file.is_open())
	{
		logPrint("File: \"%s\" is not open\n", filename);
		gui_params->show_console = true;
		return;
	}

	file << count_blocks << "\n";
	for (int i = 0; i < count_blocks; i++)
	{
		sf::Vector2f pos = blocks[i].getPosition();
		sf::Vector2f vec = blocks[i].getVelosity();
		sf::Vector2f new_vec = blocks[i].getNewVelosity();
		sf::Vector2f size = blocks[i].getRenderSize();
		float mass = blocks[i].getMass();
		float is_fixed = blocks[i].isFixed();
		file << i << "\n";
		file << pos.x << " " << pos.y << "\n";
		file << vec.x << " " << vec.y << "\n";
		file << new_vec.x << " " << new_vec.y << "\n";
		file << size.x << " " << size.y << "\n";
		file << mass << "\n";
		file << is_fixed << "\n";
		
		file << connections[i].neighbors.size() << "\n";
		for (int j = 0; j < connections[i].neighbors.size(); j++)
			file << connections[i].neighbors[j] << " ";
		file << "\n";
	}
	file.close();

	logPrint("Save map complete\n");
}

void Simulation::loadMap(const char* filename)
{
	std::ifstream file;
	string path = "Saves\\";
	string sfilename = filename;
	file.open(path + sfilename, std::ios::out);
	if (!file.is_open())
	{
		logPrint("File: \"%s\" is not open\n", filename);
		gui_params->show_console = true;
		return;
	}

	load_blocks.clear();
	load_connections.clear();
	file >> load_count_blocks;
	load_connections.resize(load_count_blocks);
	while (!file.eof())
	{
		int block_id;
		sf::Vector2f pos;
		sf::Vector2f vec;
		sf::Vector2f new_vec;
		sf::Vector2f size;
		float mass;
		float is_fixed;
		file >> block_id;
		file >> pos.x >> pos.y;
		file >> vec.x >> vec.y;
		file >> new_vec.x >> new_vec.y;
		file >> size.x >> size.y;
		file >> mass;
		file >> is_fixed;

		Block b;
		b.setPosition(pos);
		b.setVelosity(vec);
		b.setNewVelosity(new_vec);
		b.setMass(mass);
		b.setFixed(is_fixed);
		
		load_blocks.push_back(b);

		int count_neighbors;
		file >> count_neighbors;
		std::vector<int> conn;
		for (int j = 0; j < count_neighbors; j++)
		{
			int n_id;
			file >> n_id;
			conn.push_back(n_id);
		}
		connect(load_connections, block_id, conn);
	}
	file.close();

	logPrint("Load map complete\n");
	is_load = true;
	restart();
}



void Simulation::update()
{
	if (gui_params->mouse_mode == 1)
	{
		if (gui_params->block_select_id != -1)
			blocks[gui_params->block_select_id].setSelect(true);

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			for (int i = 0; i < blocks.size(); i++)
			{
				sf::Vector2f bp = blocks[i].getPosition();
				sf::Vector2f bs = blocks[i].getRenderSize();

				if (gui_params->mouse_pos.x > bp.x - bs.x / 2 && gui_params->mouse_pos.x < bp.x + bs.x / 2 &&
					gui_params->mouse_pos.y > bp.y - bs.y / 2 && gui_params->mouse_pos.y < bp.y + bs.y / 2)
				{
					if (gui_params->block_select_id != -1)
						blocks[gui_params->block_select_id].setSelect(false);
					gui_params->block_select = true;
					gui_params->block_select_id = i;
					blocks[i].setSelect(true);
					break;
				}
			}
		}
	}
	else
		blocks[gui_params->block_select_id].setSelect(false);


	if (gui_params->mouse_mode == 2)
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			if(!gui_params->block_drag)
				for (int i = 0; i < blocks.size(); i++)
				{
					sf::Vector2f bp = blocks[i].getPosition();
					sf::Vector2f bs = blocks[i].getRenderSize();

					if (gui_params->mouse_pos.x > bp.x - bs.x / 2 && gui_params->mouse_pos.x < bp.x + bs.x / 2 &&
						gui_params->mouse_pos.y > bp.y - bs.y / 2 && gui_params->mouse_pos.y < bp.y + bs.y / 2)
					{
						gui_params->block_drag = true;
						gui_params->block_drag_id = i;
						break;
					}
				}

			if (gui_params->block_drag_id != -1)
			{
				sf::Vector2f bp = blocks[gui_params->block_drag_id].getPosition();
				blocks[gui_params->block_drag_id].setPosition(bp.x, gui_params->mouse_pos.y);
			}
		}
		else
		{
			gui_params->block_drag = false;
		}
	}
	else
		gui_params->block_drag = false;


	brash.setSize(sf::Vector2f(gui_params->brash_size, gui_params->brash_size));
	brash.setOrigin(gui_params->brash_size / 2, gui_params->brash_size / 2);
	brash.setNM(gui_params->brash_size, gui_params->brash_size);
	brash.setPosition(gui_params->mouse_pos);


	if (gui_params->mouse_mode == 3)
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			for (int i = 0; i < count_blocks; i++)
			{
				sf::Vector2f bp = blocks[i].getPosition();
				sf::Vector2f bs = blocks[i].getRenderSize();

				if (bp.x > gui_params->mouse_pos.x - gui_params->brash_size / 2 && bp.x < gui_params->mouse_pos.x + gui_params->brash_size / 2 &&
					bp.y > gui_params->mouse_pos.y - gui_params->brash_size / 2 && bp.y < gui_params->mouse_pos.y + gui_params->brash_size / 2)
				{
					blocks[i].setMass(gui_params->block_mass);
				}
			}
		}
	}

	if (gui_params->display_mode <= 1)
	{
		vertex_lines.clear();
		for (int i = 0; i < count_blocks; i++)
		{
			sf::Vertex v;
			v.position = blocks[i].getPosition();
			v.color = blocks[i].getColor();
			vertex_lines.append(v);
		}
	}

	if (gui_params->display_mode == 2)
	{
		lines.clear();
		points.clear();
		for (int i = 1; i < count_blocks; i++)
		{
			sf::LineShape line(blocks[i - 1].getPosition(), blocks[i].getPosition());
			line.setThickness(5);
			line.setFillColor(blocks[i - 1].getColor());
			if (i == count_blocks - 1)
				line.setFillColor(blocks[i].getColor());
			lines.push_back(line);

			sf::CircleShape c;
			c.setFillColor(blocks[i - 1].getColor());
			if (i == count_blocks - 1)
				c.setFillColor(blocks[i].getColor());
			c.setPosition(blocks[i].getPosition());
			c.setRadius(2.4);
			c.setOrigin(2.4, 2.4);
			points.push_back(c);
		}
	}

	if (gui_params->is_pause) return;

	for (int i = 0; i < count_blocks; i++)
		blocks[i].update();

	for (int i = 0; i < count_blocks; i++)
	{
		sf::Vector2f pos = blocks[i].getPosition();

		float summ_offset = 0;
		for (int j = 0; j < connections[i].neighbors.size(); j++)
		{
			int k = connections[i].neighbors[j];
			float offset_y = blocks[k].getPosition().y;
			summ_offset += offset_y;
		}

		summ_offset /= connections[i].neighbors.size();
		blocks[i].setNewVelosity(0, summ_offset - pos.y);
	}
}

void Simulation::draw(sf::RenderWindow& window)
{
	if (gui_params->display_mode <= 1)
		window.draw(vertex_lines);

	if (gui_params->display_mode == 2)
	{
		for (int i = 0; i < points.size(); i++)
			window.draw(points[i]);

		for (int i = 0; i < lines.size(); i++)
			window.draw(lines[i]);
	}

	if(gui_params->display_mode == 0)
		for (int i = 0; i < blocks.size(); i++)
			blocks[i].draw(window);

	if (gui_params->mouse_mode == 3)
		window.draw(brash);
}
