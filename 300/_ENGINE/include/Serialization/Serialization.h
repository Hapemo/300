#pragma once
#include "Serializable.h"
#include "pch.h"
#include <concepts>
#include "glm/glm.hpp"

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
void SerializeToFile(const std::string& filename, const rapidjson::StringBuffer& buffer);
#pragma endregion file_operations

// Derived types
template <typename T>
void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const char* name, const T& val)
{
	if (!std::derived_from<T, Serializable>) return;
	
	const Serializable& base = static_cast<const Serializable&>(val);

	if (name != nullptr)
		writer.Key(name);

	writer.StartObject();
	base.Serialize(writer);
	writer.EndObject();
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