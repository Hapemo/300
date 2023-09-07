#include "Serialization/Serialization.h"

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
	return true;
}

bool EntityJSON::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) const
{
	writer->StartObject();

	// no need for now
	/*writer->String("EntityID");
	writer->Uint((std::uint32_t)mID);*/

	writer->String("Name");
	writer->String(mGJ.jName.c_str());

	writer->String("isActive");
	writer->Bool(mGJ.jIsActive);

	writer->String("Tag");
	writer->String(mGJ.jTag.c_str());

	writer->String("Subtag");
	writer->String(mGJ.jSubtag.c_str());

	writer->String("Scale");
	writer->StartArray();
	writer->Double(mTJ.jScale.x);
	writer->Double(mTJ.jScale.y);
	writer->Double(mTJ.jScale.z);
	writer->EndArray();

	writer->String("Rotate");
	writer->StartArray();
	writer->Double(mTJ.jRotate.x);
	writer->Double(mTJ.jRotate.y);
	writer->Double(mTJ.jRotate.z);
	writer->EndArray();

	writer->String("Translate");
	writer->StartArray();
	writer->Double(mTJ.jTranslate.x);
	writer->Double(mTJ.jTranslate.y);
	writer->Double(mTJ.jTranslate.z);
	writer->EndArray();

	writer->EndObject();
	
	return true;
}

bool EntityListJSON::Deserialize(const std::string& s)
{
	rapidjson::Document doc;
	if (!InitDocument(s, doc))
	{
		return false;
	}

	if (!doc.IsArray())
	{
		return false;
	}

	for (rapidjson::Value::ConstValueIterator ci = doc.Begin(); ci != doc.End(); ++ci)
	{
		EntityJSON ent;
		ent.Deserialize(*ci);
		ents.push_back(ent);
	}

	return true;
}
bool EntityListJSON::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) const
{
	writer->StartArray();

	for (std::list<EntityJSON>::const_iterator i = ents.begin(); i != ents.end(); i++)
	{
		(*i).Serialize(writer);
	}

	writer->EndArray();

	return true;
}
