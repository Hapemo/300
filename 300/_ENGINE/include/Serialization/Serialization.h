#pragma once
#include "pch.h"
//#include "rttr/type.h"
#include "document.h"
#include "prettywriter.h"
#include "stringbuffer.h"
#include "ECS/ECS_Components.h"
#include "ECS/ECS.h"
//#include "GameState/GameStateManager.h"
class Scene;

//helper functions to convert strings to enums
//template <typename T>
//std::string FindString(const T& val, const std::unordered_map<std::string, T>& map);
//
//template <typename T>
//T FindEnum(const std::string& str, const std::unordered_map<std::string, T>& map);

TAG FindTagEnum(std::string str);
std::string FindTagString(TAG tag);

SUBTAG FindSubTagEnum(std::string str);
std::string FindSubTagString(SUBTAG subtag);

MATERIAL FindMaterialEnum(std::string str);
std::string FindMaterialString(MATERIAL tag);

MOTION FindMotionEnum(std::string str);
std::string FindMotionString(MOTION tag);

AUDIOTYPE FindAudioEnum(std::string str);
std::string FindAudioString(AUDIOTYPE tag);

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

class EntityJSON : public BaseJSON
{
public:
	EntityJSON();
	virtual ~EntityJSON();

	virtual bool Deserialize(const rapidjson::Value& obj);
	virtual bool Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) const;

	const entt::entity GetIDJSON() const
	{
		return mID.id;
	}

	void SetIDJSON(const entt::entity id)
	{
		mID.id = id;
		mid_t = true;
	}

	// getter: for deserializing the info into the editor
	const General GetGeneralJSON() const
	{
		return mGJ;
	}

	// setter: for serializing the info into the json file
	void SetGeneralJSON(const General gj)
	{
		mGJ = gj;
		mg_t = true;
	}

	const Transform GetTransformJSON() const
	{
		return mTJ;
	}

	void SetTransformJSON(const Transform tj)
	{
		mTJ = tj;
		mt_t = true;
	}

	const RigidBody GetRBJSON() const
	{
		return mRBJ;
	}

	void SetRBJSON(const RigidBody rbj)
	{
		mRBJ = rbj;
		mrb_t = true;
	}

	void SetMRJSON(const MeshRenderer mrj)
	{
		mMRJ = mrj;
		mmr_t = true;
	}

	const MeshRenderer GetMRJSON() const
	{
		return mMRJ;
	}

	const BoxCollider GetBCJSON() const
	{
		return mBCJ;
	}

	void SetBCJSON(const BoxCollider bcj)
	{
		mBCJ = bcj;
		mbc_t = true;
	}

	const SphereCollider GetSCJSON() const
	{
		return mSCJ;
	}

	void SetSCJSON(const SphereCollider scj)
	{
		mSCJ = scj;
		msc_t = true;
	}

	const PlaneCollider GetPCJSON() const
	{
		return mPCJ;
	}

	void SetPCJSON(const PlaneCollider pcj)
	{
		mPCJ = pcj;
		mpc_t = true;
	}

	const Scripts GetSJSON() const
	{
		return mSJ;
	}

	void SetSJSON(const Scripts sj)
	{
		mSJ = sj;
		ms_t = true;
	}

	const Parent GetPJSON() const
	{
		return mPJ;
	}

	void SetPJSON(const Parent pj)
	{
		mPJ = pj;
		mp_t = true;
	}

	const Children GetCJSON() const
	{
		return mCJ;
	}

	void SetCJSON(const Children cj)
	{
		mCJ = cj;
		mc_t = true;
	}

	void SetAJSON(const Audio aj)
	{
		mAJ = aj;
		ma_t = true;
	}

	const Audio GetAJSON() const
	{
		return mAJ;
	}

	// trackers to disable serialization if component does not exist
	bool mid_t = false, mg_t = false, mt_t = false, mmr_t = false,
		mrb_t = false, mbc_t = false, msc_t = false,
		mpc_t = false, ms_t = false, mp_t = false,
		mc_t = false, ma_t = false;

private:
	Entity mID;
	General mGJ;
	Transform mTJ;
	RigidBody mRBJ;
	MeshRenderer mMRJ;
	BoxCollider mBCJ;
	SphereCollider mSCJ;
	PlaneCollider mPCJ;
	Scripts mSJ;
	Parent mPJ;
	Children mCJ;
	Audio mAJ;
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

struct ScJSON
{
	std::string name;
	bool isPause;
	bool forceRender;
};

class SceneJSON : public BaseJSON
{
public:
	SceneJSON();

	virtual ~SceneJSON();

	virtual bool Deserialize(const rapidjson::Value& obj);
	virtual bool Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) const;

	void SetSceneJSON(const Scene scj);
	const Scene GetSceneJSON() const;

private:
	ScJSON sj;
};

class SceneListJSON : public BaseJSON
{
public:
	virtual ~SceneListJSON() {};

	virtual bool Deserialize(const std::string& s);
	virtual bool Deserialize(const rapidjson::Value&) { return false; }
	virtual bool Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) const;

	std::list<SceneJSON>& SceneList() { return scenes; }

private:
	std::list<SceneJSON> scenes;
};