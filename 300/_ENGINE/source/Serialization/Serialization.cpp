#include "Serialization/Serialization.h"
#include "Debug/Logger.h"
#include "GameState/Scene.h"
#include "Script.h"

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

SERIALIZE_BASIC(Script)
{
	if (name != nullptr)
		writer.Key(name);
	Serialize(writer, name, val.scriptFile);
}

void WriteToFile(const std::string& filename, const rapidjson::StringBuffer& buffer)
{
	std::ofstream of{ filename };
	of << buffer.GetString();
	if (of.good()) return;
	PERROR(("Writing to output file failed! File name: " + filename).c_str());
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
		val = reader.GetDouble();
	else if (reader.HasMember(name))
		val = reader[name].GetDouble();
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
	if (reader.HasMember(name))
	{
		Deserialize(reader, "x", val.x);
		Deserialize(reader, "y", val.y);
	}
}

DESERIALIZE_BASIC(glm::vec3)
{
	if (reader.HasMember(name))
	{
		Deserialize(reader[name], "x", val.x);
		Deserialize(reader[name], "y", val.y);
		Deserialize(reader[name], "z", val.z);
	}
}

DESERIALIZE_BASIC(Script)
{
	if (reader.HasMember(name))
		Deserialize(reader, name, val.scriptFile);
}
