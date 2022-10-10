#pragma once
#include "Serializer.h"
#include "CodeTimer.hpp"

using namespace ObjectModel;

void testPack(int32_t value = 10, bool print = true)
{
	Primitive* p = Primitive::create("int32", Type::I32, value);

	Object Test("Foo");
	Test.addEntity(p);

	Core::Util::retriveNsave(&Test);
	if (print)
		std::cout << "pack: " << value << std::endl;
}

int32_t testUnpack(bool print = true)
{
	std::vector<uint8_t> objectFromFile = Core::Util::load("Foo.neff");

	[[maybe_unused]] int16_t it = 0;
	Object toPrintObject = Object::unpack(objectFromFile, it);

	std::vector<uint8_t> data = toPrintObject.findPrimitiveByName("int32").getData();
	[[maybe_unused]] int16_t it2 = 0;
	int w = Core::decode<int32_t>(data, it2);
	if (print)
		std::cout << "unpack: " << w << std::endl;
	return w;
}

void testSerialization(bool print_progress = true)
{
	std::cout << "Test Serialization start\n";
	int errors = 0;
	CodeTimer timer;

	double time_pack = 0, time_unpack = 0;
	for (int i = INT16_MIN; i < INT16_MAX; i++)
	{
		timer.start();
		testPack(i, false);
		timer.stop();
		
		time_pack+=timer.get().count();

		timer.start();
		int un = testUnpack(false);
		timer.stop();

		time_unpack += timer.get().count();

		if (i != un)
		{
			std::cout << "Error: " << std::endl;
			std::cout << " pack: " << i << std::endl;
			std::cout << " unpack: " << un << std::endl;
			errors++;
		}

		if(print_progress)
			printf("Testing progress %.2f%%\r", ((float)(i + INT16_MAX) / (INT16_MAX * 2)) * 100.0);
	}
	std::cout << "Test Serialization end" << std::endl;
	std::cout << "Test Info:" << std::endl;
	std::cout << " Errors: " << errors << std::endl;
	std::cout << " Average time pack: " << time_pack / (INT16_MAX * 2) << " sec" << std::endl;
	std::cout << " Average time unpack: " << time_unpack / (INT16_MAX * 2) << " sec" << std::endl;
	remove("Foo.neff");
}
