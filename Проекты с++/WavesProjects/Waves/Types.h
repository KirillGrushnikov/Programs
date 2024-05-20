#pragma once

#include <vector>
#include <string>
#include "SFML/Graphics.hpp"

typedef unsigned int uint;
typedef unsigned char byte;
using std::string;

struct DynamicParams
{
	sf::Vector2f mouse_pos;
	int mouse_mode = 0; // 0 - None 1 - Select 2 - Move
	int display_mode = 0; // 0 - Default 1 - No blocks 2 - Smoothed
	int seed = 0;

	int total_blocks = 100;
	bool block_select = false;
	int block_select_id = 0;

	bool block_drag = false;
	int block_drag_id = 0;

	int block_mass = 50;
	int brash_size = 100;

	bool is_pause = true;

	bool focus_on_field = true;

	bool show_console = false;

	DynamicParams() {};
};