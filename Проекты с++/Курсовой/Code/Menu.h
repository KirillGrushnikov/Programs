#pragma once
#include "SFML/Audio.hpp"
extern sf::Sound shoot;
extern sf::Music bgmusic;
extern int volume_music;
extern int volume_effect;
extern int what_diff;
extern bool choose_mars;
extern bool fullscreen;
extern sf::Cursor cursor;
extern sf::RectangleShape Loading;
extern int ColorLoading;
extern bool isLoading;
void chekMusic();
void menu(sf::RenderWindow& window);