#pragma once
#include "pch.h"
#include "document.h"
#include "prettywriter.h"
#include "stringbuffer.h"
#include "ECS/ECS_Components.h"
#include "ECS/ECS.h"

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
	std::string jName;
	bool jIsActive;
	std::string jTag;
	std::string jSubtag;
};

struct TransformJSON
{
	glm::vec3 jScale;
	glm::vec3 jRotate;
	glm::vec3 jTranslate;
};

class EntityJSON : public BaseJSON
{
public:
	EntityJSON();
	virtual ~EntityJSON();

	virtual bool Deserialize(const rapidjson::Value& obj);
	virtual bool Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) const;

	// helper function to convert enum to str, vice versa
	std::string FindTagString(TAG tag) const
	{
		for (const auto& it : TagMap)
		{
			if (it.second == tag)
				return it.first;
		}
	}

	TAG FindTagEnum(std::string str) const
	{
		for (const auto& it : TagMap)
		{
			if (it.first == str)
				return it.second;
		}
	}

	std::string FindSubTagString(SUBTAG subtag) const
	{
		for (const auto& it : SubTagMap)
		{
			if (it.second == subtag)
				return it.first;
		}
	}

	SUBTAG FindSubTagEnum(std::string str) const
	{
		for (const auto& it : SubTagMap)
		{
			if (it.first == str)
				return it.second;
		}
	}
	// end of helper function
		
		// no need for now
		/*const entt::entity GetID() const
		{
			return mID;
		}

		void SetID(const entt::entity ID)
		{
			mID = ID;
		}*/

		// getter: for deserializing the info into the editor
		const General GetGeneralJSON() const
		{
			General gj;
			gj.name = mGJ.jName;
			gj.isActive = mGJ.jIsActive;
			gj.tag = FindTagEnum(mGJ.jTag);
			gj.subtag = FindSubTagEnum(mGJ.jSubtag);

			return gj;
		}

		// setter: for serializing the info into the json file
		void SetGeneralJSON(const General gj)
		{
			mGJ.jName = gj.name;
			mGJ.jIsActive = gj.isActive;
			mGJ.jTag = FindTagString(gj.tag);
			mGJ.jSubtag = FindSubTagString(gj.subtag);
		}

		const Transform GetTransformJSON() const
		{
			Transform tj;
			tj.mScale = mTJ.jScale;
			tj.mRotate = mTJ.jRotate;
			tj.mTranslate = mTJ.jTranslate;

			return tj;
		}

		void SetTransformJSON(const Transform tj)
		{
			mTJ.jScale = tj.mScale;
			mTJ.jRotate = tj.mRotate;
			mTJ.jTranslate = tj.mTranslate;
		}
	
private:
	//entt::entity mID{}; // no need for now
	GeneralJSON mGJ;
	TransformJSON mTJ;
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