#pragma once
#include "pch.h"
#include "document.h"
#include "prettywriter.h"
#include "stringbuffer.h"
#include "../ECS_Components.h"
#include "../ECS.h"

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
struct GeneralJSON
{
	std::string mName;
	bool mIsActive;
	TAG mTag;
	SUBTAG mSubtag;
};

class EntityJSON : public BaseJSON
{
public:
	EntityJSON();
	virtual ~EntityJSON();

	virtual bool Deserialize(const rapidjson::Value& obj);
	virtual bool Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) const;


		const entt::entity GetID() const
		{
			return mID;
		}

		void SetID(const entt::entity ID)
		{
			mID = ID;
		}

		// for deserializing the info into the editor
		const General GetGeneralJSON() const
		{
			General gj;
			gj.name = mGJ.mName;
			gj.isActive = mGJ.mIsActive;
			gj.tag = mGJ.mTag;
			gj.subtag = mGJ.mSubtag;

			return gj;
		}

		// for serializing the info into the json file
		void SetGeneralJSON(const General gj)
		{
			mGJ.mName = gj.name;
			mGJ.mIsActive = gj.isActive;
			mGJ.mTag = gj.tag;
			mGJ.mSubtag = gj.subtag;
		}
	
private:
	entt::entity mID{};
	GeneralJSON mGJ;
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