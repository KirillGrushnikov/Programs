#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"

#include "SFML/Graphics.hpp"

#include "Types.h"
#include "Simulation.h"

void ImGuiMenu(sf::RenderWindow& window, sf::Clock& delta_clock, DynamicParams& params, Simulation& sim);