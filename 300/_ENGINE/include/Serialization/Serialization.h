#pragma once
#include "Serializable.h"
#include "pch.h"
#include <concepts>
#include "glm/glm.hpp"

// forward declaration
struct Scene;
class Script;

/*///////////////////////////////
* SERIALIZATION
*////////////////////////////////
#pragma region serialization
#define SERIALIZE_BASIC(T) void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const char* name, const T& val)
#pragma region basic_types
SERIALIZE_BASIC(bool);
SERIALIZE_BASIC(int);
SERIALIZE_BASIC(std::uint32_t);
SERIALIZE_BASIC(float);
SERIALIZE_BASIC(double);
SERIALIZE_BASIC(std::string);
SERIALIZE_BASIC(glm::ivec2);
SERIALIZE_BASIC(glm::vec3);
SERIALIZE_BASIC(Script);
#pragma endregion basic_types
// Derived types has to inherit from Serializable
#pragma region derived_types
template <typename T>
void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const char* name, const T& val);
#pragma endregion derived_types

#pragma region containers
template <typename T>
void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const char* name, const T* arr, size_t size);
template <typename T>
void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const char* name, const std::vector<T>& vec);
template <typename T1, typename T2>
void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const char* name, const std::map<T1, T2>& map);
#pragma endregion containers

#pragma region file_operations
void WriteToFile(const std::string& filename, const rapidjson::StringBuffer& buffer);
#pragma endregion file_operations

#pragma region implementation
// Derived types
template <typename T>
void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const char* name, const T& val)
{
	if (!std::derived_from<T, Serializable>) return;
	if (name != nullptr) writer.Key(name);

	val.SerializeSelf(writer);
}

// Containers
template <typename T>
void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const char* name, const T* arr, size_t size)
{
	if (name != nullptr)
		writer.Key(name);

	writer.StartArray();

	for (size_t i = 0; i < size; ++i)
		Serialize(writer, nullptr, arr[i]);

	writer.EndArray();
}

template <typename T>
void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const char* name, const std::vector<T>& vec)
{
	if (name != nullptr)
		writer.Key(name);

	writer.StartArray();

	for (const T& t : vec)
		Serialize(writer, nullptr, t);

	writer.EndArray();
}

template <typename T1, typename T2>
void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const char* name, const std::map<T1, T2>& map)
{
	if (name != nullptr)
		writer.Key(name);

	writer.StartArray();

	for (auto it = map.begin(); it != map.end(); ++it)
	{
		writer.StartObject();

		Serialize(writer, "key", it->first);
		Serialize(writer, "value", it->second);

		writer.EndObject();
	}

	writer.EndArray();
}
#pragma endregion implementation
#pragma endregion serialization

/*///////////////////////////////
* DESERIALIZATION
*////////////////////////////////
#pragma region deserialization
#define DESERIALIZE_BASIC(T) void Deserialize(rapidjson::Value& reader, const char* name, T& val)

#pragma region basic_types
DESERIALIZE_BASIC(bool);
DESERIALIZE_BASIC(int);
DESERIALIZE_BASIC(std::uint32_t);
DESERIALIZE_BASIC(float);
DESERIALIZE_BASIC(double);
DESERIALIZE_BASIC(std::string);
DESERIALIZE_BASIC(glm::ivec2);
DESERIALIZE_BASIC(glm::vec3);
DESERIALIZE_BASIC(Script);
#pragma endregion basic_types
// Derived types has to inherit from Serializable
#pragma region derived_types
template <typename T>
void Deserialize(rapidjson::Value& reader, const char* name, const T& val);
#pragma endregion derived_types

#pragma region containers
template <typename T>
void Deserialize(rapidjson::Value& reader, const char* name, const T* arr, size_t size);
template <typename T>
void Deserialize(rapidjson::Value& reader, const char* name, const std::vector<T>& vec);
template <typename T1, typename T2>
void Deserialize(rapidjson::Value& reader, const char* name, const std::map<T1, T2>& map);
#pragma endregion containers

#pragma region file_operations
#pragma endregion file_operations

#pragma region implementation
// Derived types
template <typename T>
void Deserialize(rapidjson::Value& reader, const char* name, const T& val)
{
	if (!reader.HasMember(name)) return;
	if (!std::derived_from<T, Serializable>) return;

	val.DeserializeSelf(reader[name]);
}

// Containers
template <typename T>
void Deserialize(rapidjson::Value& reader, const char* name, const T* arr, size_t size)
{
	if (!reader.HasMember(name)) return;

	for (size_t i = 0; i < size; ++i)
		Deserialize(reader[name][i], nullptr, arr[i]);
}

template <typename T>
void Deserialize(rapidjson::Value& reader, const char* name, const std::vector<T>& vec)
{
	if (!reader.HasMember(name)) return;

	// make sure the vector is empty
	vec.clear();
	for (auto i = 0; i < reader[name].Size(); ++i)
	{
		T item;
		Deserialize(reader[name][i], nullptr, item);
		vec.push_back(item);
	}
}

template <typename T1, typename T2>
void Deserialize(rapidjson::Value& reader, const char* name, const std::map<T1, T2>& map)
{
	if (!reader.HasMember(name)) return;

	for (auto i = 0; i < reader[name].Size(); ++i)
	{
		if (reader[name][i].HasMember("key") && reader[name][i].HasMember("value")) {
			T1 key;
			T2 value;

			Deserialize(reader[name][i], "key", key);
			Deserialize(reader[name][i], "value", value);

			map.insert(std::make_pair(key, value));
		}
	}
}
#pragma endregion implementation
#pragma endregion deserialization