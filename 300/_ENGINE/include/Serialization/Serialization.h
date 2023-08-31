#pragma once
#include "pch.h"
#include "document.h"
#include "prettywriter.h"
#include "stringbuffer.h"

class BaseJSON
{
public:
	bool DeserializeFile(const std::string& filename);
	bool SerializeFile(const std::string& filename);

	virtual bool Deserialize(const std::string& s);
	virtual std::string Serialize() const;

	virtual bool Deserialize(const rapidjson::Value& obj) = 0;
	virtual bool Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) const = 0;

protected:
	bool InitDocument(const std::string& s, rapidjson::Document& doc);
};

// to be replaced by added components later on
// e.g.: Transform, Texture, Collider, etc.
struct ExampleComponent
{
	std::string component_name;
	int ent_id{};
	float num_1{}, num_2{};
};

class EntityJSON : public BaseJSON
{
public:
	EntityJSON();
	virtual ~EntityJSON();

	virtual bool Deserialize(const rapidjson::Value& obj);
	virtual bool Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) const;

	/*
		// for deserializing the info into the editor
		const ComponentType getComponent() const
		{
			ComponentType ct;
			ct.component_name = ec.component_name;
			ct.ent_id = ec.ent_id;
			ct.num_1 = ec.num_1;
			ct.num_2 = ec.num_2;

			return ct;
		}

		// for serializing the info into the json file
		void setComponent(const ComponentType ct)
		{
			ec.component_name = ct.component_name;
			ec.ent_id = ct.ent_id;
			ec.num_1 = ct.num_1;
			ec.num_2 = ct.num2_2;
		}
	*/
private:
	ExampleComponent ec;
};

class EntityListJSON : public BaseJSON
{
public:
	virtual ~EntityListJSON() {};

	virtual bool Deserialize(const std::string& s);
	virtual bool Deserialize(const rapidjson::Value&) { return false; }
	virtual bool Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) const;

	std::list<EntityJSON>& EntitiesList() { return ents; }

private:
	std::list<EntityJSON> ents;

};