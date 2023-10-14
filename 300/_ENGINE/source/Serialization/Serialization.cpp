#include "Serialization/Serialization.h"
#include "Debug/Logger.h"
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

void WriteToFile(const std::string& filename, const rapidjson::StringBuffer& buffer)
{
	std::ofstream of{ filename };
	of << buffer.GetString();
	if (of.good()) return;
	PERROR(("Writing to output file failed! File name: " + filename).c_str());
}