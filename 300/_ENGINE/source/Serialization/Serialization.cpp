#include "Serialization.h"

bool BaseJSON::DeserializeFile(const std::string& filename)
{
	std::ifstream file(filename);
	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();

	return Deserialize(buffer.str());
}

bool BaseJSON::SerializeFile(const std::string& filename)
{
	std::ofstream file(filename);
	std::string str = Serialize();
	file << str;
	file.flush();
	file.close();

	return true;
}

bool BaseJSON::Deserialize(const std::string& s)
{
	rapidjson::Document doc;
	if (!InitDocument(s, doc))
	{
		return false;
	}

	Deserialize(doc);

	return true;
}

std::string BaseJSON::Serialize() const
{
	rapidjson::StringBuffer ss;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(ss);
	if (Serialize(&writer))
	{
		return ss.GetString();
	}
	return "";
}

bool BaseJSON::InitDocument(const std::string& s, rapidjson::Document& doc)
{
	if (s.empty())
	{
		return false;
	}

	std::string validJSON(s);

	return !doc.Parse(validJSON.c_str()).HasParseError() ? true : false;
}

EntityJSON::EntityJSON() {}

EntityJSON::~EntityJSON() {}

bool EntityJSON::Deserialize(const rapidjson::Value& obj)
{
	/*
		if (obj.HasMember("ComponentName")
		{
			blabla.something = obj["BlaBla"].GetSomething();
		}
	*/
}

bool EntityJSON::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) const
{
	writer->StartObject();

	/*
		eg:
		writer->String("ComponentName");
		writer->String(ec.component_name.c_str());
	*/

	writer->EndObject();
}