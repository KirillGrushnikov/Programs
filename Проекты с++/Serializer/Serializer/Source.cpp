#include <iostream>
#include <cassert>
#include "Serializer.h"
#include "SerializeTest.hpp"

using namespace ObjectModel;

struct Point
{
private:
	friend class Poligon;
	std::string name;
	int x, y;

public:
	Point()
	{
		name = "";
		x = 0;
		y = 0;
	}

	void setName(const char* name)
	{
		this->name = (char*)name;
	}

	void setPoint(int x, int y)
	{
		this->x = x;
		this->y = y;
	}

	void info()
	{
		std::cout << "   Name: " << name << std::endl;
		std::cout << "   X: " << x << " Y: " << y << std::endl;
	}

	void serialize(Object* o)
	{
		Array* p_name = Array::createString("PointName", Type::I8, name);
		Primitive* p_x = Primitive::create("X", Type::I32, x);
		Primitive* p_y = Primitive::create("Y", Type::I32, y);
		o->addEntity(p_name);
		o->addEntity(p_x);
		o->addEntity(p_y);
	}

	void deserialize(Object* o)
	{
		std::vector<uint8_t> data_x = o->findPrimitiveByName("X").getData();
		std::vector<uint8_t> data_y = o->findPrimitiveByName("Y").getData();
		int16_t it_x = 0;
		int16_t it_y = 0;
		
		x = Core::decode<int32_t>(data_x, it_x);
		y = Core::decode<int32_t>(data_y, it_y);
		
		Array* str = (Array*)o->findByName("PointName");
		std::vector<uint8_t> std_data = str->getData();
		name = Core::decodeSTR(std_data, std_data.size());
	}
};

class Poligon
{
private:
	std::string name;
	std::vector<Point> points;

public:
	Poligon()
	{
		name = "";
	}

	void setName(const char* name)
	{
		this->name = (char*)name;
	}

	void setPoints(std::vector<Point> points)
	{
		for (int i = 0; i < points.size(); i++)
		{
			std::string name = "Point" + std::to_string(i);
			points[i].setName(name.c_str());
		}

		this->points = points;
	}

	void info()
	{
		std::cout << "   Name: " << name << std::endl;
		std::cout << "   Points: " << points.size() << std::endl;
		for (Point p : points)
			p.info();
		std::cout << std::endl;
	}

	void serialize()
	{
		std::cout << "Packing object:\n";
		Object poligon(this->name);
		//Array* name = Array::createString("ObjectName", Type::I8, this->name);
		//poligon.addEntity(name);
		
		for (int i = 0; i < points.size(); i++)
		{
			Object* point = new Object("ObjPoint: " + std::to_string(i));
			points[i].serialize(point);
			poligon.addEntity(point);
		}

		Core::Util::retriveNsave(&poligon);
	}

	void deserialize(const char* path)
	{
		std::cout << "Unpacking object:\n";
		Object poligon;

		std::vector<uint8_t> buffer = Core::Util::load(path);
		int16_t it = 0;
		poligon = Object::unpack(buffer, it);
		/*
		Array* str = (Array*)poligon.findByName("ObjectName");
		std::vector<uint8_t> std_data = str->getData();
		int16_t it2 = 0;
		name = Core::decodeSTR(std_data, std_data.size());
		*/
		name = poligon.getName();

		points.resize(poligon.objectCount);
		
		for (int i = 0; i < poligon.objectCount; i++)
		{
			Object point = poligon.objects[i];
			points[i].deserialize(&point);
		}
	}
};


int main()
{
	assert(Core::Util::isLittleEndian());

	setlocale(LC_ALL, "Russian");

	bool test, repeat;

	do
	{
		std::cout << "Start test serialization?(1/0)\n";
		std::cin >> test;

		if (test)
			testSerialization();

		std::cout << "Start create object?(1/0)\n";
		bool create_obj;
		std::cin >> create_obj;

		if (create_obj)
		{
			Poligon poligon_s;
			std::string name;
			std::cout << "Enter name object: ";
			std::cin >> name;

			poligon_s.setName(name.c_str());

			int count_points;
			do
			{
				std::cout << "Enter count points: ";
				std::cin >> count_points;
				if(count_points < 0)
					std::cout << "Error: count points less than zero\n";
			} while (count_points < 0);

			std::vector<Point> points;
			for (int i = 0; i < count_points; i++)
			{
				int x, y;
				std::cout << "Point[" << i+1 << "/" << count_points << "]\n";
				std::cout << "Enter x: ";
				std::cin >> x;
				std::cout << "Enter y: ";
				std::cin >> y;

				Point p;
				p.setPoint(x, y);

				points.push_back(p);
			}
			if(!points.empty())
				poligon_s.setPoints(points);

			poligon_s.serialize();
			poligon_s.info();


			Poligon poligon_d;

			std::string filename = name + ".neff";
			poligon_d.deserialize(filename.c_str());

			poligon_d.info();

			bool save;
			std::cout << "Save file?(1/0)\n";
			std::cin >> save;
			if (!save)
				remove(filename.c_str());
		}
		std::cout << "Repeat?(1/0)\n";
		std::cin >> repeat;
	} while (repeat);

	return 0;
}