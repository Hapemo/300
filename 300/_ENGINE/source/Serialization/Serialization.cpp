/*!*************************************************************************
****
\file		   Serialization.cpp
\author(s)	   Charissa Yip, Lor Xaun Yun Michelle, Kew Yu Jun
\par DP email:
charissa.yip\@digipen.edu, xaunyunmichelle.lor\@digipen.edu
\date		   4-11-2023
\brief
This file contains various overloaded definitions of the macro function
that serializes and deserializes different data types as well as common
containers for JSON. It also includes functions to write to file and
read from file.
****************************************************************************/
#include "Serialization/Serialization.h"
#include "Debug/Logger.h"
#include "GameState/Scene.h"
#include "Script.h"
#include "AI/AIManager.h"

SERIALIZE_BASIC(bool)
{
	if (name != nullptr)
		writer.Key(name);
	writer.Bool(val);
}

SERIALIZE_BASIC(int)
{
	if (name != nullptr)
		writer.Key(name);
	writer.Int(val);
}

SERIALIZE_BASIC(unsigned char)
{
	if (name != nullptr)
		writer.Key(name);
	writer.Uint(val);
}

SERIALIZE_BASIC(std::uint32_t)
{
	if (name != nullptr)
		writer.Key(name);
	writer.Uint(val);
}

SERIALIZE_BASIC(float)
{
	if (name != nullptr)
		writer.Key(name);
	writer.Double(val);
}

SERIALIZE_BASIC(double)
{
	if (name != nullptr)
		writer.Key(name);
	writer.Double(val);
}

SERIALIZE_BASIC(std::string)
{
	// if there is a name given
	if (name != nullptr)
		writer.Key(name);
	writer.String(val.c_str());
}

SERIALIZE_BASIC(glm::ivec2)
{
	if (name != nullptr)
		writer.Key(name);
	writer.StartObject();
	Serialize(writer, "x", val.x);
	Serialize(writer, "y", val.y);
	writer.EndObject();
}

SERIALIZE_BASIC(glm::vec2)
{
	if (name != nullptr)
		writer.Key(name);
	writer.StartObject();
	Serialize(writer, "x", val.x);
	Serialize(writer, "y", val.y);
	writer.EndObject();
}

SERIALIZE_BASIC(glm::bvec3)
{
	if (name != nullptr)
		writer.Key(name);
	writer.StartObject();
	Serialize(writer, "x", val.x);
	Serialize(writer, "y", val.y);
	Serialize(writer, "z", val.z);
	writer.EndObject();
}

SERIALIZE_BASIC(glm::vec3)
{
	if (name != nullptr)
		writer.Key(name);
	writer.StartObject();
	Serialize(writer, "x", val.x);
	Serialize(writer, "y", val.y);
	Serialize(writer, "z", val.z);
	writer.EndObject();
}

SERIALIZE_BASIC(glm::vec4)
{
	if (name != nullptr)
		writer.Key(name);
	writer.StartObject();
	Serialize(writer, "x", val.x);
	Serialize(writer, "y", val.y);
	Serialize(writer, "z", val.z);
	Serialize(writer, "w", val.w);
	writer.EndObject();
}

SERIALIZE_BASIC(Script*)
{
	if (name != nullptr)
		writer.Key(name);

	writer.StartObject();
	Serialize(writer, "scriptFile", val->scriptFile);
	Serialize(writer, "variables", val->variables);
	writer.EndObject();
}

SERIALIZE_BASIC(SUBTAG)
{
	if (name != nullptr)
		writer.Key(name);
	Serialize(writer, nullptr, static_cast<int>(val));
}

SERIALIZE_BASIC(MATERIAL)
{
	if (name != nullptr)
		writer.Key(name);
	Serialize(writer, nullptr, static_cast<int>(val));
}

SERIALIZE_BASIC(MOTION)
{
	if (name != nullptr)
		writer.Key(name);
	Serialize(writer, nullptr, static_cast<int>(val));
}

SERIALIZE_BASIC(AUDIOTYPE)
{
	if (name != nullptr)
		writer.Key(name);
	Serialize(writer, nullptr, static_cast<int>(val));
}

SERIALIZE_BASIC(E_MOVEMENT_TYPE)
{
	if (name != nullptr)
		writer.Key(name);
	Serialize(writer, nullptr, static_cast<int>(val));
}

DESERIALIZE_BASIC(bool)
{
	if (name == nullptr)
		val = reader.GetBool();
	else if (reader.HasMember(name))
		val = reader[name].GetBool();
}

DESERIALIZE_BASIC(int)
{
	if (name == nullptr)
		val = reader.GetInt();
	else if (reader.HasMember(name))
		val = reader[name].GetInt();
}

DESERIALIZE_BASIC(unsigned char)
{
	if (name == nullptr)
		val = (unsigned char)reader.GetUint();
	else if (reader.HasMember(name))
		val = (unsigned char)reader[name].GetUint();
}

DESERIALIZE_BASIC(std::uint32_t)
{
	if (name == nullptr)
		val = reader.GetUint();
	else if (reader.HasMember(name))
		val = reader[name].GetUint();
}

DESERIALIZE_BASIC(float)
{
	if (name == nullptr)
		val = (float)reader.GetDouble();
	else if (reader.HasMember(name))
		val = (float)reader[name].GetDouble();
}

DESERIALIZE_BASIC(double)
{
	if (name == nullptr)
		val = reader.GetDouble();
	else if (reader.HasMember(name))
		val = reader[name].GetDouble();
}

DESERIALIZE_BASIC(std::string)
{
	if (name == nullptr)
		val = reader.GetString();
	else if (reader.HasMember(name))
		val = reader[name].GetString();
}

DESERIALIZE_BASIC(glm::ivec2)
{
	if (name == nullptr)
	{
		Deserialize(reader, "x", val.x);
		Deserialize(reader, "y", val.y);
	}
	else
	{
		if (reader.HasMember(name))
		{
			Deserialize(reader[name], "x", val.x);
			Deserialize(reader[name], "y", val.y);
		}
	}
}

DESERIALIZE_BASIC(glm::vec2)
{
	if (name == nullptr)
	{
		Deserialize(reader, "x", val.x);
		Deserialize(reader, "y", val.y);
	}
	else
	{
		if (reader.HasMember(name))
		{
			Deserialize(reader[name], "x", val.x);
			Deserialize(reader[name], "y", val.y);
		}
	}
}

DESERIALIZE_BASIC(glm::bvec3)
{
	if (name == nullptr)
	{
		Deserialize(reader, "x", val.x);
		Deserialize(reader, "y", val.y);
		Deserialize(reader, "z", val.z);
	}
	else
	{
		if (reader.HasMember(name))
		{
			Deserialize(reader[name], "x", val.x);
			Deserialize(reader[name], "y", val.y);
			Deserialize(reader[name], "z", val.z);
		}
	}
}

DESERIALIZE_BASIC(glm::vec3)
{
	if (name == nullptr)
	{
		Deserialize(reader, "x", val.x);
		Deserialize(reader, "y", val.y);
		Deserialize(reader, "z", val.z);
	}
	else
	{
		if (reader.HasMember(name))
		{
			Deserialize(reader[name], "x", val.x);
			Deserialize(reader[name], "y", val.y);
			Deserialize(reader[name], "z", val.z);
		}
	}
}

DESERIALIZE_BASIC(glm::vec4)
{
	if (name == nullptr)
	{
		Deserialize(reader, "x", val.x);
		Deserialize(reader, "y", val.y);
		Deserialize(reader, "z", val.z);
		Deserialize(reader, "w", val.w);
	}
	else
	{
		if (reader.HasMember(name))
		{
			Deserialize(reader[name], "x", val.x);
			Deserialize(reader[name], "y", val.y);
			Deserialize(reader[name], "z", val.z);
			Deserialize(reader[name], "w", val.w);
		}
	}
}

DESERIALIZE_BASIC(Script*)
{
	name = nullptr;
	val = new Script;
	Deserialize(reader, "scriptFile", val->scriptFile);
	Deserialize(reader, "variables", val->variables);
}

DESERIALIZE_BASIC(SUBTAG)
{
	if (reader.HasMember(name))
	{
		int num;
		Deserialize(reader, name, num);
		val = static_cast<SUBTAG>(num);
	}
}

DESERIALIZE_BASIC(MATERIAL)
{
	if (reader.HasMember(name))
	{
		int num;
		Deserialize(reader, name, num);
		val = static_cast<MATERIAL>(num);
	}
}

DESERIALIZE_BASIC(MOTION)
{
	if (reader.HasMember(name))
	{
		int num;
		Deserialize(reader, name, num);
		val = static_cast<MOTION>(num);
	}
}

DESERIALIZE_BASIC(AUDIOTYPE)
{
	if (reader.HasMember(name))
	{
		int num;
		Deserialize(reader, name, num);
		val = static_cast<AUDIOTYPE>(num);
	}
}

DESERIALIZE_BASIC(E_MOVEMENT_TYPE)
{
	if (reader.HasMember(name))
	{
		int num;
		Deserialize(reader, name, num);
		val = static_cast<E_MOVEMENT_TYPE>(num);
	}
}

DESERIALIZE_BASIC(entt::entity)
{
	if (reader.HasMember(name))
	{
		int num;
		Deserialize(reader, name, num);
		val = static_cast<entt::entity>(num);
	}
}

//DESERIALIZE_BASIC(enum_tag::enum_tag)
//{
//	if (reader.HasMember(name))
//	{
//		int num;
//		Deserialize(reader, name, num);
//		val = static_cast<enum_tag::enum_tag>(num);
//	}
//}

void WriteToFile(const std::string &filename, const rapidjson::StringBuffer &buffer)
{
	std::ofstream of{filename};
	of << buffer.GetString();
	if (of.good())
		return;
	PERROR(("Writing to output file failed! File name: " + filename).c_str());
}

void ReadFromFile(const std::string &filename, rapidjson::Document &doc)
{
	std::ifstream ifs{filename};
	if (!ifs.is_open())
		PERROR(("Reading from input file failed! File name: " + filename).c_str());

	std::stringstream ss;
	ss << ifs.rdbuf();
	ifs.close();

	if (!InitDocument(ss.str(), doc) || !doc.IsArray())
		return;
}

bool InitDocument(const std::string &s, rapidjson::Document &doc)
{
	if (s.empty())
		return false;

	std::string valid(s);

	return !doc.Parse(valid.c_str()).HasParseError() ? true : false;
}
