#pragma once
#include "Serializer.h"
#include "CodeTimer.hpp"

using namespace ObjectModel;

struct TestData
{
	int16_t value;
	std::string str;
	std::vector<uint8_t> buffer;

	bool operator!= (TestData& b)
	{
		if (this->value != b.value || this->str != b.str)
			return true;
		return false;
	}
};

TestData testPackInFile(int16_t value = 10, bool print = true)
{
	Primitive* p = Primitive::create("int16", Type::I16, value);

	srand(time(0));
	int lenght_str = rand() % (15 - 4) + 3;
	std::string str;
	for (int i = 0; i < lenght_str; i++)
		str += char(rand() % (90 - 66) + 65);

	Array* s = Array::createString("string", Type::I8, str);

	Object Test("Foo");
	Test.addEntity(p);
	Test.addEntity(s);

	Core::Util::retriveNsave(&Test);
	if (print)
	{
		std::cout << "pack: " << value << std::endl;
		std::cout << "pack: " << str << std::endl;
	}
	TestData res;
	res.value = value;
	res.str = str;

	return res;
}

TestData testUnpackFromFile(bool print = true)
{
	std::vector<uint8_t> objectFromFile = Core::Util::load("Foo.neff");

	[[maybe_unused]] int16_t it = 0;
	Object toPrintObject = Object::unpack(objectFromFile, it);

	std::vector<uint8_t> data = toPrintObject.findPrimitiveByName("int16").getData();
	[[maybe_unused]] int16_t it2 = 0;
	int w = Core::decode<int16_t>(data, it2);

	it2 = 0;
	Array* str = (Array*)toPrintObject.findByName("string");
	data = str->getData();
	std::string s = Core::decodeSTR(data, data.size());
	if (print)
	{
		std::cout << "unpack: " << w << std::endl;
		std::cout << "unpack: " << s << std::endl;
	}
	TestData res;
	res.value = w;
	res.str = s;

	return res;
}

void testSerializationInFile(bool print_progress = true, bool print_pack = false, bool print_unpack = false)
{
	std::cout << "Test Serialization start\n";
	int errors = 0;
	CodeTimer timer;
	CodeTimer timer_all;

	double time_pack = 0, time_unpack = 0, time_all = 0;
	TestData data_pack, data_unpack;
	timer_all.start();
	for (int16_t i = INT16_MIN; i < INT16_MAX; i++)
	{
		timer.start();
		data_pack = testPackInFile(i, print_pack);
		timer.stop();

		time_pack += timer.get().count();

		timer.start();
		data_unpack = testUnpackFromFile(print_unpack);
		timer.stop();

		time_unpack += timer.get().count();

		if (data_pack != data_unpack)
		{
			std::cout << "Error: " << std::endl;
			std::cout << "   pack: " << data_pack.value << "  " << data_pack.str << std::endl;
			std::cout << "   unpack: " << data_unpack.value << "  " << data_unpack.str << std::endl;
			errors++;
		}

		if (print_progress)
		{
			if (print_pack || print_unpack)
				std::cout << std::endl;
			printf("Testing progress %.2f%%\r", ((float)(i + INT16_MAX) / (INT16_MAX * 2)) * 100.0);
		}
	}
	timer_all.stop();
	time_all = timer_all.get().count();
	std::cout << "\nTest Serialization end\n\n";
	std::cout << "Test Info:" << std::endl;
	std::cout << "   Last test:" << std::endl;
	std::cout << "      Pack:" << std::endl;
	std::cout << "         value: " << data_pack.value << " str: " << data_pack.str << std::endl;
	std::cout << "      Unpack:" << std::endl;
	std::cout << "         value: " << data_unpack.value << " str: " << data_unpack.str << std::endl;
	std::cout << "   Errors: " << errors << std::endl;
	std::cout << "   Test time: " << time_all << " sec" << std::endl;
	std::cout << "   Average time pack: " << time_pack / time_all << " sec" << std::endl;
	std::cout << "   Average time unpack: " << time_unpack / time_all << " sec" << std::endl;
	remove("Foo.neff");
}

TestData testPack(int16_t value = 10, bool print = true)
{
	Primitive* p = Primitive::create("int16", Type::I16, value);

	srand(time(0));
	int lenght_str = rand() % (15 - 4) + 3;
	std::string str;
	for (int i = 0; i < lenght_str; i++)
		str += char(rand() % (90 - 66) + 65);

	Array* s = Array::createString("string", Type::I8, str);

	Object Test("Foo");
	Test.addEntity(p);
	Test.addEntity(s);

	if (print)
	{
		std::cout << "pack: " << value << std::endl;
		std::cout << "pack: " << str << std::endl;
	}


	std::vector<uint8_t> buffer(Test.getSize());
	int16_t iterator = 0;
	Test.pack(&buffer, &iterator);

	TestData res;
	res.value = value;
	res.str = str;
	res.buffer = buffer;

	return res;
}

TestData testUnpack(std::vector<uint8_t> buffer, bool print = true)
{
	[[maybe_unused]] int16_t it = 0;
	Object toPrintObject = Object::unpack(buffer, it);

	std::vector<uint8_t> data = toPrintObject.findPrimitiveByName("int16").getData();
	[[maybe_unused]] int16_t it2 = 0;
	int w = Core::decode<int16_t>(data, it2);

	it2 = 0;
	Array* str = (Array*)toPrintObject.findByName("string");
	data = str->getData();
	std::string s = Core::decodeSTR(data, data.size());
	if (print)
	{
		std::cout << "unpack: " << w << std::endl;
		std::cout << "unpack: " << s << std::endl;
	}
	TestData res;
	res.value = w;
	res.str = s;

	return res;
}

void testSerialization(bool print_progress = true, bool print_pack = false, bool print_unpack = false)
{
	std::cout << "Test Serialization start\n";
	int errors = 0;
	CodeTimer timer;
	CodeTimer timer_all;

	double time_pack = 0, time_unpack = 0, time_all = 0;
	TestData data_pack, data_unpack;
	timer_all.start();
	for (int16_t i = INT16_MIN; i < INT16_MAX; i++)
	{
		timer.start();
		data_pack = testPack(i, print_pack);
		timer.stop();

		time_pack += timer.get().count();

		timer.start();
		data_unpack = testUnpack(data_pack.buffer, print_unpack);
		timer.stop();

		time_unpack += timer.get().count();

		if (data_pack != data_unpack)
		{
			std::cout << "Error: " << std::endl;
			std::cout << "   pack: " << data_pack.value << "  " << data_pack.str << std::endl;
			std::cout << "   unpack: " << data_unpack.value << "  " << data_unpack.str << std::endl;
			errors++;
		}

		if (print_progress)
		{
			printf("Testing progress %.2f%%\r", ((float)(i + INT16_MAX) / (INT16_MAX * 2)) * 100.0);
			if (print_pack || print_unpack)
				std::cout << std::endl;
		}
	}
	timer_all.stop();
	time_all = timer_all.get().count();
	std::cout << "\nTest Serialization end\n\n";
	std::cout << "Test Info:" << std::endl;
	std::cout << "   Last test:" << std::endl;
	std::cout << "      Pack:" << std::endl;
	std::cout << "         value: " << data_pack.value << " str: " << data_pack.str << std::endl;
	std::cout << "      Unpack:" << std::endl;
	std::cout << "         value: " << data_unpack.value << " str: " << data_unpack.str << std::endl;
	std::cout << "   Errors: " << errors << std::endl;
	std::cout << "   Test time: " << time_all << " sec" << std::endl;
	std::cout << "   Average time pack: " << time_pack / time_all << " sec" << std::endl;
	std::cout << "   Average time unpack: " << time_unpack / time_all << " sec" << std::endl;
	remove("Foo.neff");
}