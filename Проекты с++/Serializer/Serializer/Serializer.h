#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <locale>
#include <bitset>
#include <fstream>

//encode
namespace Core
{
	// 0x00 0x00 0x00 0x5
	template<typename T>
	void encode(std::vector<uint8_t>* buffer, int16_t* iterator, T value)
	{
		for (unsigned i = 0, j = 0; i < sizeof T; i++)
		{
			(*buffer)[(*iterator)++] = (value >> ((sizeof T * 8) - 8) - ((i == 0) ? j : j += 8));
		}
	}

	template<>
	void encode<float>(std::vector<uint8_t>* buffer, int16_t* iterator, float value)
	{
		int32_t result = *reinterpret_cast<int32_t*>(&value);
		encode<int32_t>(buffer, iterator, result);
	}

	template<>
	void encode<double>(std::vector<uint8_t>* buffer, int16_t* iterator, double value)
	{
		int32_t result = *reinterpret_cast<int64_t*>(&value);
		encode<int64_t>(buffer, iterator, result);
	}

	template<>
	void encode<std::string>(std::vector<uint8_t>* buffer, int16_t* iterator, std::string value)
	{
		for (unsigned i = 0; i < value.size(); i++)
		{
			encode<int8_t>(buffer, iterator, value[i]);
		}
	}

	template<typename T>
	void encode(std::vector<uint8_t>* buffer, int16_t* iterator, std::vector<T> value)
	{
		for (unsigned i = 0; i < value.size(); i++)
		{
			encode<T>(buffer, iterator, value[i]);
		}
	}
}

//decode
namespace Core
{
	template<typename T>
	T decode(std::vector<uint8_t>& buffer, int16_t& it)
	{
		T result = 0;


		for (unsigned i = 0; i < sizeof T; i++)
		{
			T temp = (T)buffer[it++] << (((sizeof(T) * 8) - 8) - (i * 8));
			result = result | temp;
		}


		return result;
	}

	template<>
	inline std::string decode<std::string>(std::vector<uint8_t>& buffer, int16_t& it)
	{
		it -= 2;
		int16_t size = decode<int16_t>(buffer, it);

		std::string result((buffer.begin() + it), (buffer.begin() + (it + size)));
		it += size;

		return result;

	}

	inline std::string decodeSTR(std::vector<uint8_t>& buffer, int16_t size)
	{
		int16_t it = 0;
		std::string result((buffer.begin() + it), (buffer.begin() + (it + size)));
		return result;
	}


	template<typename ...>
	void decode(std::vector<uint8_t>& buffer, int16_t& it, std::vector<uint8_t>& dest)
	{
		for (unsigned i = 0; i < dest.size(); i++)
		{
			dest[i] = buffer[it++];
		}
	}
}


namespace ObjectModel
{
	enum class Wrapper : int8_t
	{
		PRIMITIVE = 1,
		ARRAY,
		STRING,
		OBJECT
	};

	enum class Type : int8_t
	{
		I8 = 1,
		I16,
		I32,
		I64,

		U8,
		U16,
		U32,
		U64,

		FLOAT,
		DOUBLE,

		BOOL,
		CHAR = I8,
		SHORT = I16,
		INT = I32,
	};

	template<typename ...>
	uint8_t getTypeSize(Type type)
	{
		switch (type)
		{
		case Type::BOOL: return sizeof(bool); break;
		case Type::I8: return sizeof(uint8_t); break;
		case Type::I16: return sizeof(int16_t); break;
		case Type::I32: return sizeof(int32_t); break;
		case Type::I64: return sizeof(int64_t); break;
		case Type::FLOAT: return sizeof(float); break;
		case Type::DOUBLE: return sizeof(double); break;
		}
	}

	class Root
	{
	public:
		int8_t wrapper;
	protected:
		std::string name;
		int16_t nameLenght;
		int32_t size;
	protected:
		Root();
	public:
		int32_t getSize() const;
		void setName(std::string);
		std::string getName() const;
		virtual void pack(std::vector<uint8_t>*, int16_t*) = 0;
	};

	class Primitive : public Root
	{
	private:
		int8_t type = 0;
		std::vector<uint8_t>* data = nullptr;

		Primitive();
	public:
		template<typename T>
		static Primitive* create(std::string name, Type type, T value)
		{
			Primitive* p = new Primitive();
			p->setName(name);
			p->wrapper = static_cast<int8_t>(Wrapper::PRIMITIVE);
			p->type = static_cast<int8_t>(type);
			p->data = new std::vector<uint8_t>(sizeof value);
			p->size += p->data->size();
			int16_t iterator = 0;
			Core::template encode(p->data, &iterator, value);

			return p;
		}

		void pack(std::vector<uint8_t>*, int16_t*);
		static Primitive unpack(std::vector<uint8_t>&, int16_t&);
		std::vector<uint8_t> getData();
	};

	class Array : public Root
	{
	public:
		int32_t count = 0;
	private:
		int8_t type = 0;
		std::vector<uint8_t>* data = nullptr;

		Array();
	public:
		template <typename T>
		static Array* createArray(std::string name, Type type, std::vector<T> value)
		{
			Array* arr = new Array();
			arr->setName(name);
			arr->wrapper = static_cast<int8_t>(Wrapper::ARRAY);
			arr->type = static_cast<int8_t>(type);
			arr->count = (int32_t)value.size();
			arr->data = new std::vector<uint8_t>(sizeof T * arr->count);
			arr->size += (int32_t)value.size() * sizeof T;
			int16_t iterator = 0;
			Core::template encode<T>(arr->data, &iterator, value);

			return arr;
		}

		template <typename T>
		static Array* createString(std::string name, Type type, T value)
		{
			Array* str = new Array();
			str->setName(name);
			str->wrapper = static_cast<int8_t>(Wrapper::STRING);
			str->type = static_cast<int8_t>(type);
			str->count = (int32_t)value.size();
			str->data = new std::vector<uint8_t>(value.size());
			str->size += (int32_t)value.size();
			int16_t iterator = 0;
			Core::template encode<T>(str->data, &iterator, value);

			return str;
		}

		void pack(std::vector<uint8_t>*, int16_t*);
		static Array unpack(std::vector<uint8_t>&, int16_t&);
		static Array unpackS(std::vector<uint8_t>&, int16_t&);
		std::vector<uint8_t> getData();
	};

	class Object : public Root
	{
	public:
		int16_t primitiveCount = 0, arrayCount = 0, stringCount = 0, objectCount = 0;
		std::vector<Primitive> primitives;
		std::vector<Array> arrays;
		std::vector<Array> strings;
		std::vector<Object> objects;

		Object(std::string);
		void addEntity(Root* r);
		Primitive findPrimitiveByName(std::string name);
		Root* findByName(std::string);
		void pack(std::vector<uint8_t>*, int16_t*);
		static Object unpack(std::vector<uint8_t>&, int16_t&);

	};
}

namespace ObjectModel
{
	//definition
	Root::Root()
		:
		name("unknown"),
		wrapper(0),
		nameLenght(0),
		size(sizeof nameLenght + sizeof wrapper + sizeof size) {}

	void Root::setName(std::string name)
	{
		this->name = name;
		nameLenght = (int16_t)name.length();
		size += nameLenght;
	}

	std::string Root::getName() const
	{
		return name;
	}

	int32_t Root::getSize() const
	{
		return size;
	}

	/// Primitive ///

	Primitive::Primitive()
	{
		size += sizeof type;
	}

	void Primitive::pack(std::vector<uint8_t>* buffer, int16_t* iterator)
	{
		Core::encode<int8_t>(buffer, iterator, wrapper);
		Core::encode<int16_t>(buffer, iterator, nameLenght);
		Core::encode<std::string>(buffer, iterator, name);
		Core::encode<int8_t>(buffer, iterator, type);
		Core::encode<uint8_t>(buffer, iterator, *data);
		Core::encode<int32_t>(buffer, iterator, size);
	}

	Primitive Primitive::unpack(std::vector<uint8_t>& buffer, int16_t& it)
	{
		Primitive p;
		p.wrapper = Core::decode<int8_t>(buffer, it);
		p.nameLenght = Core::decode<int16_t>(buffer, it);
		p.name = Core::decode<std::string>(buffer, it);
		p.type = Core::decode<int8_t>(buffer, it);
		p.data = new std::vector<uint8_t>(getTypeSize((Type)p.type));
		Core::decode(buffer, it, *p.data);
		p.size = Core::decode<int32_t>(buffer, it);

		return p;
	}

	std::vector<uint8_t> Primitive::getData()
	{
		return *data;
	}

	/// Primitive end ///

	/// Array ///

	Array::Array()
	{
		size += sizeof type + sizeof count;
	}

	void Array::pack(std::vector<uint8_t>* buffer, int16_t* iterator)
	{
		Core::encode<int8_t>(buffer, iterator, wrapper);
		Core::encode<int16_t>(buffer, iterator, nameLenght);
		Core::encode<std::string>(buffer, iterator, name);
		Core::encode<int8_t>(buffer, iterator, type);
		Core::encode<int32_t>(buffer, iterator, count);
		Core::encode<uint8_t>(buffer, iterator, *data);
		Core::encode<int32_t>(buffer, iterator, size);
	}

	Array Array::unpack(std::vector<uint8_t>& buffer, int16_t& it)
	{
		Array arr;
		arr.wrapper = Core::decode<int8_t>(buffer, it);
		arr.nameLenght = Core::decode<int16_t>(buffer, it);
		arr.name = Core::decode<std::string>(buffer, it);
		arr.type = Core::decode<int8_t>(buffer, it);
		arr.count = Core::decode<int32_t>(buffer, it);
		arr.data = new std::vector<uint8_t>(getTypeSize((Type)arr.type) * arr.count);
		Core::decode(buffer, it, *arr.data);
		arr.size = Core::decode<int32_t>(buffer, it);


		return arr;
	}

	Array Array::unpackS(std::vector<uint8_t>& buffer, int16_t& it)
	{
		Array str;
		str.wrapper = Core::decode<int8_t>(buffer, it);
		str.nameLenght = Core::decode<int16_t>(buffer, it);
		str.name = Core::decode<std::string>(buffer, it);
		str.type = Core::decode<uint8_t>(buffer, it);
		str.count = Core::decode<int32_t>(buffer, it);
		str.data = new std::vector<uint8_t>(str.count);
		Core::decode(buffer, it, *str.data);
		str.size = Core::decode<int32_t>(buffer, it);


		return str;
	}

	std::vector<uint8_t> Array::getData()
	{
		return *data;
	}

	/// Array end ///

	/// Object ///

	Object::Object(std::string name = "default")
	{
		setName(name);
		wrapper = static_cast<int8_t>(Wrapper::OBJECT);
		size += (sizeof int16_t) * 4;
	}

	void Object::addEntity(Root* r)
	{
		switch (r->wrapper)
		{
		case 1: primitives.push_back(*dynamic_cast<Primitive*>(r)); primitiveCount += 1; break;
		case 2: arrays.push_back(*dynamic_cast<Array*>(r)); arrayCount += 1; break;
		case 3: strings.push_back(*dynamic_cast<Array*>(r)); stringCount += 1; break;
		case 4: objects.push_back(*dynamic_cast<Object*>(r)); objectCount += 1; break;
		}

		size += r->getSize();
	}

	Primitive Object::findPrimitiveByName(std::string name)
	{
		for (Primitive p : primitives)
		{
			if (p.getName() == name)
			{
				return p;
			}
		}
	}

	Root* Object::findByName(std::string name)
	{
		for (Primitive p : primitives)
		{
			if (p.getName() == name)
			{
				return dynamic_cast<Root*>(&p);
			}
		}

		for (Array arr : arrays)
		{
			if (arr.getName() == name)
			{
				return  dynamic_cast<Root*>(&arr);
			}
		}

		for (Array str : strings)
		{
			if (str.getName() == name)
			{
				return  dynamic_cast<Root*>(&str);
			}
		}

		for (Object o : objects)
		{
			if (o.getName() == name)
			{
				return  dynamic_cast<Root*>(&o);
			}
		}

		std::cout << "no as such" << std::endl;

		__asm {
			int 3
		}
		return new Object("SYSTEM:empty");
	}

	void Object::pack(std::vector<uint8_t>* buffer, int16_t* iterator)
	{
		Core::encode<int8_t>(buffer, iterator, wrapper);
		Core::encode<int16_t>(buffer, iterator, nameLenght);
		Core::encode<std::string>(buffer, iterator, name);


		Core::encode<int16_t>(buffer, iterator, primitiveCount);
		for (auto p : primitives)
		{
			p.pack(buffer, iterator);
		}

		Core::encode<int16_t>(buffer, iterator, arrayCount);
		for (auto arr : arrays)
		{
			arr.pack(buffer, iterator);
		}

		Core::encode<int16_t>(buffer, iterator, stringCount);
		for (auto str : strings)
		{
			str.pack(buffer, iterator);
		}

		Core::encode<int16_t>(buffer, iterator, objectCount);
		for (auto o : objects)
		{
			o.pack(buffer, iterator);
		}

		Core::encode<int32_t>(buffer, iterator, size);
	}

	Object Object::unpack(std::vector<uint8_t>& buffer, int16_t& it)
	{
		Object obj;
		obj.wrapper = Core::decode<int8_t>(buffer, it);
		obj.nameLenght = Core::decode<int16_t>(buffer, it);
		obj.name = Core::decode<std::string>(buffer, it);

		obj.primitiveCount = Core::decode<int16_t>(buffer, it);
		for (int i = 0; i < obj.primitiveCount; i++)
		{
			obj.primitives.push_back(Primitive::unpack(buffer, it));
		}

		obj.arrayCount = Core::decode<int16_t>(buffer, it);
		for (int i = 0; i < obj.arrayCount; i++)
		{
			obj.arrays.push_back(Array::unpack(buffer, it));
		}

		obj.stringCount = Core::decode<int16_t>(buffer, it);
		for (int i = 0; i < obj.stringCount; i++)
		{
			obj.strings.push_back(Array::unpackS(buffer, it));
		}

		obj.objectCount = Core::decode<int16_t>(buffer, it);
		for (int i = 0; i < obj.objectCount; i++)
		{
			obj.objects.push_back(unpack(buffer, it));
		}

		obj.size = Core::decode<int32_t>(buffer, it);


		return obj;
	}

	/// Object end ///
}

//util
namespace Core
{
	namespace Util
	{
		bool isLittleEndian()
		{
			// 0x00 0x00 0x00 0b0000 0101
			int8_t a = 5;
			std::string result = std::bitset<8>(a).to_string();
			return (result.back() == '1') ? true : false;
		}

		void save(const char* filename, std::vector<uint8_t>& buffer)
		{
			std::ofstream out;
			out.open(filename, std::ofstream::binary);
			for (unsigned i = 0; i < buffer.size(); i++)
			{
				out << buffer[i];
			}
			out.close();
		}

		std::vector<uint8_t> load(const char* path)
		{
			std::ifstream in(path, std::ios::binary);
			std::vector<uint8_t> result((std::istreambuf_iterator<char>(in)), (std::istreambuf_iterator<char>()));
			in.close();
			return result;
		}

		void retriveNsave(ObjectModel::Root* r)
		{
			int16_t iterator = 0;
			std::vector<uint8_t> buffer(r->getSize());
			std::string name = r->getName().substr(0, r->getName().length()).append(".neff");
			r->pack(&buffer, &iterator);
			save(name.c_str(), buffer);
		}
	}
}

/*
int32_t foo = 5;
Primitive* p = Primitive::create("int32", Type::I32, foo);
Core::Util::retriveNsave(p);

std::vector<int64_t> data{ 1, 2, 3, 4 };
Array* arr = Array::createArray("array", Type::I64, data);
Core::Util::retriveNsave(arr);

std::string data_str = "Hello world";
Array* str = Array::createString("string", Type::I8, data_str);
Core::Util::retriveNsave(str);

Object obj("object");
obj.addEntity(p);
obj.addEntity(arr);
obj.addEntity(str);

Object obj2("object2");
obj2.addEntity(p);
Core::Util::retriveNsave(&obj2);

obj.addEntity(&obj2);
Core::Util::retriveNsave(&obj);
*/
