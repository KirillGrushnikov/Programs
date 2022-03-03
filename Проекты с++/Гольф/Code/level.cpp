#include "level.h"

int Object::GetPropertyInt(const std::string& name)
{
    return atoi(properties[name].c_str());
}

float Object::GetPropertyFloat(const std::string& name)
{
    return strtod(properties[name].c_str(), NULL);
}

std::string Object::GetPropertyString(const std::string& name)
{
    return properties[name];
}


bool Level::LoadFromFile(const std::string& filename)
{
    TiXmlDocument levelFile(filename.c_str());


    // Загружаем XML-карту
    if (!levelFile.LoadFile())
    {
        std::cout << "Loading level \"" << filename << "\" failed." << std::endl;
        return false;
    }

    // Работаем с контейнером map
    TiXmlElement* map;
    map = levelFile.FirstChildElement("map");

    // Пример карты: <map version="1.0" orientation="orthogonal"
    // width="10" height="10" tilewidth="34" tileheight="34">
    width = std::atoi(map->Attribute("width"));
    height = std::atoi(map->Attribute("height"));
    tileWidth = std::atoi(map->Attribute("tilewidth"));
    tileHeight = std::atoi(map->Attribute("tileheight"));

    // Берем описание размеры чанков

    TiXmlElement* editorsettings, * chunksize;
    editorsettings = map->FirstChildElement("editorsettings");
    chunksize = editorsettings->FirstChildElement("chunksize");
    chunkWidth = std::atoi(chunksize->Attribute("width"));
    chunkHeight = std::atoi(chunksize->Attribute("height"));
    ratioWidth = chunkWidth / tileWidth;
    ratioHeight = chunkHeight / tileHeight;

    // Берем описание тайлсета и идентификатор первого тайла
    TiXmlElement* tilesetElement;
    tilesetElement = map->FirstChildElement("tileset");
    firstTileID = std::atoi(tilesetElement->Attribute("firstgid"));

    // source - путь до картинки в контейнере image
    TiXmlElement* image;
    image = tilesetElement->FirstChildElement("image");
    std::string imagepath = image->Attribute("source");
    if (imagepath[0] = '.')
        imagepath.erase(imagepath.begin(), imagepath.begin() + 3);

    // Пытаемся загрузить тайлсет
    sf::Image img;
    if (!img.loadFromFile(imagepath))
    {
        std::cout << "Failed to load tile sheet." << std::endl;
        return false;
    }


    img.createMaskFromColor(sf::Color(255, 255, 255));
    tilesetImage.loadFromImage(img);
    tilesetImage.setSmooth(false);

    // Получаем количество столбцов и строк тайлсета
    int columns = tilesetImage.getSize().x / tileWidth;
    int rows = tilesetImage.getSize().y / tileHeight;

    // Вектор из прямоугольников изображений (TextureRect)
    std::vector<sf::Rect<int>> subRects;

    for (int y = 0; y < rows; y++)
        for (int x = 0; x < columns; x++)
        {
            sf::Rect<int> rect;

            rect.top = y * tileHeight; //tileHeight
            rect.height = tileHeight;
            rect.left = x * tileWidth; // tileWidth
            rect.width = tileWidth;

            subRects.push_back(rect);
        }

    // Работа со слоями
    TiXmlElement* layerElement;
    layerElement = map->FirstChildElement("layer");

    std::vector<Layer> Layers;
    while (layerElement)
    {
        Layer layer;

        // Если присутствует opacity, то задаем прозрачность слоя, иначе он полностью непрозрачен
        if (layerElement->Attribute("opacity") != NULL)
        {
            float opacity = strtod(layerElement->Attribute("opacity"), NULL);
            layer.opacity = 255 * opacity;
        }
        else
        {
            layer.opacity = 255;
        }

        // Контейнер <data>
        TiXmlElement* layerDataElement;
        layerDataElement = layerElement->FirstChildElement("data");

        if (layerDataElement == NULL)
        {
            std::cout << "Bad map. No layer information found." << std::endl;
        }

        // Контейнер <tile> - описание тайлов каждого слоя
        TiXmlElement* tileElement;
        tileElement = layerDataElement->FirstChildElement("tile");

        if (tileElement == NULL)
        {
            std::cout << "Bad map. No tile information found." << std::endl;
            return false;
        }

        int x = 0;
        int y = 0;

        while (tileElement)
        {
            int tileGID = 0;
            if (tileElement->Attribute("gid") != nullptr)
                tileGID = std::atoi(tileElement->Attribute("gid"));
            int subRectToUse = tileGID - firstTileID;


            // Устанавливаем TextureRect каждого тайла
            if (subRectToUse >= 0)
            {
                sf::RectangleShape sprite;
                sprite.setTexture(&tilesetImage);
                sprite.setTextureRect(subRects[subRectToUse]);
                sprite.setPosition(x * chunkWidth, y * chunkHeight);
                sprite.setFillColor(sf::Color(255, 255, 255, layer.opacity));
                sprite.setSize(sf::Vector2f(chunkWidth, chunkHeight));

                layer.tiles.push_back(sprite);
            }

            tileElement = tileElement->NextSiblingElement("tile");

            x++;
            if (x >= width)
            {
                x = 0;
                y++;
                if (y >= height)
                    y = 0;
            }
        }

        Layers.push_back(layer);

        layerElement = layerElement->NextSiblingElement("layer");
    }
    layers = Layers;

    // Работа с объектами
    TiXmlElement* objectGroupElement;

    std::vector<Object> Objects;
    // Если есть слои объектов
    if (map->FirstChildElement("objectgroup") != NULL)
    {
        objectGroupElement = map->FirstChildElement("objectgroup");
        while (objectGroupElement)
        {
            // Контейнер <object>
            TiXmlElement* objectElement;
            objectElement = objectGroupElement->FirstChildElement("object");

            while (objectElement)
            {
                // Получаем все данные - тип, имя, позиция, etc
                std::string objectType;
                if (objectElement->Attribute("type") != NULL)
                {
                    objectType = objectElement->Attribute("type");
                }
                std::string objectName;
                if (objectElement->Attribute("name") != NULL)
                {
                    objectName = objectElement->Attribute("name");
                }
                float objectRotation = NULL;
                if (objectElement->Attribute("rotation") != NULL)
                {
                    objectRotation = std::atof(objectElement->Attribute("rotation"));
                }
                int x = std::atoi(objectElement->Attribute("x"));
                int y = std::atoi(objectElement->Attribute("y"));

                int width = 0, height = 0;
                sf::RectangleShape sprite;

                if (objectElement->Attribute("width") != NULL)
                {
                    width = atoi(objectElement->Attribute("width"));
                    height = atoi(objectElement->Attribute("height"));
                }
                else
                {
                    if (objectElement->Attribute("gid") != nullptr)
                    {
                        width = subRects[std::atoi(objectElement->Attribute("gid")) - firstTileID].width;
                        height = subRects[std::atoi(objectElement->Attribute("gid")) - firstTileID].height;
                        sprite.setTextureRect(subRects[std::atoi(objectElement->Attribute("gid")) - firstTileID]);
                    }
                }

                x *= ratioWidth;
                y *= ratioHeight;
                width *= ratioWidth;
                height *= ratioHeight;

                //Спрайт объекта(коллизия)
                sprite.setPosition(x, y);
                sprite.setSize(sf::Vector2f(width, height));
                sprite.setRotation(objectRotation);
                sprite.setFillColor(sf::Color(218, 132, 1, 70));

                // Экземпляр объекта
                Object object;
                object.name = objectName;
                object.type = objectType;
                object.rotation = objectRotation;
                object.sprite = sprite;

                sf::Rect <float> objectRect;
                objectRect.top = y;
                objectRect.left = x;
                objectRect.height = height;
                objectRect.width = width;
                object.rect = objectRect;

                // "Переменные" объекта
                TiXmlElement* properties;
                properties = objectElement->FirstChildElement("properties");
                if (properties != NULL)
                {
                    TiXmlElement* prop;
                    prop = properties->FirstChildElement("property");
                    if (prop != NULL)
                    {
                        while (prop)
                        {
                            std::string propertyName = prop->Attribute("name");
                            std::string propertyValue = prop->Attribute("value");

                            object.properties[propertyName] = propertyValue;

                            prop = prop->NextSiblingElement("property");
                        }
                    }
                }


                Objects.push_back(object);

                objectElement = objectElement->NextSiblingElement("object");
            }
            objectGroupElement = objectGroupElement->NextSiblingElement("objectgroup");
        }
        objects = Objects;
    }
    else
    {
        std::cout << "No object layers found..." << std::endl;
    }

    return true;
}

Object Level::GetObject2(const std::string& name)
{
    // Только первый объект с заданным именем
    for (int i = 0; i < objects.size(); i++)
        if (objects[i].name == name)
            return objects[i];
}

std::vector<Object> Level::GetObjects(const std::string& name)
{
    // Все объекты с заданным именем
    std::vector<Object> vec;
    for (int i = 0; i < objects.size(); i++)
        if (objects[i].name == name)
            vec.push_back(objects[i]);

    return vec;
}

std::vector<Object> Level::GetAllObjects()
{
    return objects;
};

sf::Vector2i Level::GetTileSize()
{
    return sf::Vector2i(tileWidth, tileHeight);
}

sf::Vector2i Level::GetChunkSize()
{
    return sf::Vector2i(chunkWidth, chunkHeight);
}

void Level::Draw(sf::RenderWindow& window)
{
    // Рисуем все тайлы (объекты НЕ рисуем!)
    for (int layer = 0; layer < layers.size(); layer++)
    {
        for (int tile = 0; tile < layers[layer].tiles.size(); tile++)
        {
            window.draw(layers[layer].tiles[tile]);
        }
    }
}

void Level::DrawCollision(sf::RenderWindow& window)
{
    // Рисуем всю коллизию
    for (int obj = 0; obj < objects.size(); obj++)
        window.draw(objects[obj].sprite);
}