#pragma once
#include "Serializable.h"
#include "pch.h"
#include <concepts>
#include "glm/glm.hpp"
#include "ECS/Tags.h"
#include "Physics/PhysicsTypes.h"
#include "Audio/AudioType.h"
#include <entt.hpp>
#include <variant>
#include <typeinfo>
//#include "ECS/ECS_Components.h"
//#include "ECS/EnumStrings.h"
#include "ECS/EnumTags.h"

// forward declaration
class Scene;
class Script;
enum class E_MOVEMENT_TYPE : char;

/*///////////////////////////////
* SERIALIZATION
*////////////////////////////////
#pragma region serialization
#define SERIALIZE_BASIC(T) void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const char* name, T const& val)
#pragma region basic_types
SERIALIZE_BASIC(bool);
SERIALIZE_BASIC(int);
SERIALIZE_BASIC(unsigned char);
SERIALIZE_BASIC(std::uint32_t);
SERIALIZE_BASIC(float);
SERIALIZE_BASIC(double);
SERIALIZE_BASIC(std::string);
SERIALIZE_BASIC(glm::ivec2);
SERIALIZE_BASIC(glm::bvec3);
SERIALIZE_BASIC(glm::vec3);
SERIALIZE_BASIC(glm::vec4);
SERIALIZE_BASIC(Script*);
SERIALIZE_BASIC(SUBTAG);
SERIALIZE_BASIC(MATERIAL);
SERIALIZE_BASIC(MOTION);
SERIALIZE_BASIC(AUDIOTYPE);
SERIALIZE_BASIC(E_MOVEMENT_TYPE);
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
template <typename ...T>
void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const char* name, const std::variant<T...>& var);
#pragma endregion containers

#pragma region file_operations
void WriteToFile(const std::string& filename, const rapidjson::StringBuffer& buffer);
#pragma endregion file_operations

#pragma region implementation
// Derived types
template <typename T>
void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const char* name, T const& val)
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
		Serialize(writer, "val", it->second);

		writer.EndObject();
	}

	writer.EndArray();
}

template <typename ...T>
void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const char* name, const std::variant<T...>& var)
{
	if (name != 0)
		writer.Key(name);

	writer.StartObject();
	std::visit([&](auto&& val)
		{
			Serialize(writer, "type", (int)(typeid(val).hash_code()));
			Serialize(writer, "value", val);
		}, var);
	writer.EndObject();
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
DESERIALIZE_BASIC(unsigned char);
DESERIALIZE_BASIC(std::uint32_t);
DESERIALIZE_BASIC(float);
DESERIALIZE_BASIC(double);
DESERIALIZE_BASIC(std::string);
DESERIALIZE_BASIC(glm::ivec2);
DESERIALIZE_BASIC(glm::bvec3);
DESERIALIZE_BASIC(glm::vec3);
DESERIALIZE_BASIC(glm::vec4);
DESERIALIZE_BASIC(Script*);
DESERIALIZE_BASIC(SUBTAG);
DESERIALIZE_BASIC(MATERIAL);
DESERIALIZE_BASIC(MOTION);
DESERIALIZE_BASIC(AUDIOTYPE);
DESERIALIZE_BASIC(E_MOVEMENT_TYPE);
DESERIALIZE_BASIC(entt::entity);
//DESERIALIZE_BASIC(enum_tag::enum_tag);
#pragma endregion basic_types
// Derived types has to inherit from Serializable
#pragma region derived_types
template <typename T>
void Deserialize(rapidjson::Value& reader, const char* name, T& val);
#pragma endregion derived_types

#pragma region containers
template <typename T>
void Deserialize(rapidjson::Value& reader, const char* name, T* arr, size_t size);
template <typename T>
void Deserialize(rapidjson::Value& reader, const char* name, std::vector<T>& vec);
template <typename T1, typename T2>
void Deserialize(rapidjson::Value& reader, const char* name, std::map<T1, T2>& map);
template <typename ...T>
void Deserialize(rapidjson::Value& reader, const char* name, std::variant<T...>& var);
template <typename variant, typename T, typename ...Ts>
void GetVariantValue(rapidjson::Value& reader, int type, variant& _variant);
#pragma endregion containers

#pragma region file_operations
void ReadFromFile(const std::string& filename, rapidjson::Document& doc);
bool InitDocument(const std::string& s, rapidjson::Document& doc);
#pragma endregion file_operations

#pragma region implementation
// Derived types
template <typename T>
void Deserialize(rapidjson::Value& reader, const char* name, T& val)
{
	if (!reader.HasMember(name)) return;
	if (std::derived_from<T, Serializable>) return;

	val.DeserializeSelf(reader[name]);
}

// Containers
template <typename T>
void Deserialize(rapidjson::Value& reader, const char* name, T* arr, size_t size)
{
	if (!reader.HasMember(name)) return;

	for (rapidjson::SizeType i = 0; i < (rapidjson::SizeType)size; ++i)
		Deserialize(reader[name][i], nullptr, arr[i]);
}

template <typename T>
void Deserialize(rapidjson::Value& reader, const char* name, std::vector<T>& vec)
{
	if (name != nullptr)
	{
		if (!reader.HasMember(name)) return;

		//check if the array is empty
		if (reader[name].Empty()) return;

		// make sure the vector is empty
		//vec.clear();
		for (size_t i = 0; i < (size_t)reader[name].Size(); ++i)
		{
			T item;
			Deserialize(reader[name][i], nullptr, item);
			vec.push_back(item);
		}
	}
	else if(name == nullptr)
	{
		for (size_t i = 0; i < reader.Size(); ++i)
		{
			T item;
			Deserialize(reader[i], nullptr, item);
			vec.push_back(item);
		}
	}
}

template <typename T1, typename T2>
void Deserialize(rapidjson::Value& reader, const char* name, std::map<T1, T2>& map)
{
	if (!reader.HasMember(name)) return;

	for (rapidjson::SizeType i = 0; i < reader[name].Size(); ++i)
	{
		if (reader[name][i].HasMember("key") && reader[name][i].HasMember("val")) {
			T1 key;
			T2 value;

			Deserialize(reader[name][i], "key", key);
			Deserialize(reader[name][i], "val", value);

			map.insert(std::make_pair(key, value));
		}
	}
}

template <typename ...T>
void Deserialize(rapidjson::Value& reader, const char* name, std::variant<T...>& var)
{
	if (name != nullptr)
	{
		if (reader.HasMember(name))
		{
			int type;
			Deserialize(reader[name], "type", type);
			GetVariantValue<std::variant<T...>, T...>(reader[name]["value"], type, var);
		}
	}
	else if(name == nullptr)
	{
		int type;
		Deserialize(reader, "type", type);
		GetVariantValue<std::variant<T...>, T...>(reader["value"], type, var);
	}
}

template <typename variant, typename T, typename ...Ts>
void GetVariantValue(rapidjson::Value& reader, int type, variant& _variant)
{
	if (type == (int)(typeid(T).hash_code()))
	{
		T value;
		Deserialize(reader, nullptr, value);
		_variant = value;
	}
	else if constexpr (sizeof...(Ts) > 0)
		GetVariantValue<variant, Ts...>(reader, type, _variant);
}
#pragma endregion implementation
#pragma endregion deserialization