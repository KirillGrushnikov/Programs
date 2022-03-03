#pragma once
#include <string>
#include <vector>
#include <map>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "tinyxml/tinyxml.h"


struct Object
{
    std::string name;
    std::string type;
    float rotation = 0;
    sf::Rect<float> rect;
    std::map<std::string, std::string> properties;

	sf::RectangleShape sprite;

    int GetPropertyInt(const std::string& name);
    float GetPropertyFloat(const std::string& name);
    std::string GetPropertyString(const std::string& name);
};

struct Layer
{
    int opacity;
    std::vector<sf::RectangleShape> tiles;
};

class Level
{
public:
    bool LoadFromFile(const std::string& filename);

    Object GetObject2(const std::string& name);

    std::vector<Object> GetObjects(const std::string& name);
	std::vector<Object> GetAllObjects();

    void Draw(sf::RenderWindow &window);
    void DrawCollision(sf::RenderWindow& window);

	sf::Vector2i GetTileSize();
    sf::Vector2i GetChunkSize();

    void SetGravity(float gravity)
    {
        Gravity = gravity;
    }
    float GetGravity()
    {
        return Gravity;
    }

private:
    int width, height, tileWidth, tileHeight, chunkWidth, chunkHeight;
    float ratioWidth, ratioHeight;
    int firstTileID;
    float Gravity;
    sf::Rect<float> drawingBounds;
    sf::Texture tilesetImage;
    std::vector<Object> objects;
    std::vector<Layer> layers;
};

///////////////////////////////////////
