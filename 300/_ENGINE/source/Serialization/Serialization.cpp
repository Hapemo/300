#include "Serialization/Serialization.h"
#include "Serialization/SerializationTemp.h"
#include "GameState/Scene.h"

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

EntityJSON::EntityJSON() : mID(0) {}

EntityJSON::~EntityJSON() {}

bool EntityJSON::Deserialize(const rapidjson::Value& obj)
{
	if (obj.HasMember("EntityID"))
	{
		mID.id = (entt::entity)obj["EntityID"]["EntityID"].GetInt();
	}

	if (obj.HasMember("General"))
	{
		mGJ.name = obj["General"]["Name"].GetString();
		mGJ.isActive = obj["General"]["Active"].GetBool();
		mGJ.isPaused = obj["General"]["Paused"].GetBool();
		mGJ.tag = FindTagEnum(obj["General"]["Tag"].GetString());
		mGJ.subtag = FindSubTagEnum(obj["General"]["Subtag"].GetString());
	}

	if (obj.HasMember("Transform"))
	{
		mTJ.mScale.x = (float)obj["Transform"]["Scale"]["x"].GetDouble();
		mTJ.mScale.y = (float)obj["Transform"]["Scale"]["y"].GetDouble();
		mTJ.mScale.z = (float)obj["Transform"]["Scale"]["z"].GetDouble();

		mTJ.mRotate.x = (float)obj["Transform"]["Rotate"]["x"].GetDouble();
		mTJ.mRotate.y = (float)obj["Transform"]["Rotate"]["y"].GetDouble();
		mTJ.mRotate.z = (float)obj["Transform"]["Rotate"]["z"].GetDouble();

		mTJ.mTranslate.x = (float)obj["Transform"]["Translate"]["x"].GetDouble();
		mTJ.mTranslate.y = (float)obj["Transform"]["Translate"]["y"].GetDouble();
		mTJ.mTranslate.z = (float)obj["Transform"]["Translate"]["z"].GetDouble();
	}

	if (obj.HasMember("RigidBody"))
	{
		mRBJ.mMass = (std::uint16_t)obj["RigidBody"]["Mass"].GetDouble();
		mRBJ.mMaterial = FindMaterialEnum(obj["RigidBody"]["Material"].GetString());
		mRBJ.mMotion = FindMotionEnum(obj["RigidBody"]["Motion"].GetString());

		mrb_t = true;
	}
	else mrb_t = false;

	if (obj.HasMember("MeshRenderer"))
	{
		mMRJ.mShaderPath = std::pair<std::string, std::string>(obj["MeshRenderer"]["ShaderPath"]["first"].GetString(), obj["MeshRenderer"]["ShaderPath"]["second"].GetString());
		for(int i = 0; i < obj["MeshRenderer"]["MaterialInstancePath"].Size(); ++i)
			mMRJ.mMaterialInstancePath.push_back(obj["MeshRenderer"]["MaterialInstancePath"][i].GetString());
		mMRJ.mMeshPath = obj["MeshRenderer"]["MeshPath"].GetString();
		mMRJ.mGUID = (unsigned)obj["MeshRenderer"]["GUID"].GetInt();

		mmr_t = true;
	}
	else mmr_t = false;


	if (obj.HasMember("BoxCollider"))
	{
		mBCJ.mScaleOffset.x = (float)obj["BoxCollider"]["ScaleOffset"]["x"].GetDouble();
		mBCJ.mScaleOffset.y = (float)obj["BoxCollider"]["ScaleOffset"]["y"].GetDouble();
		mBCJ.mScaleOffset.z = (float)obj["BoxCollider"]["ScaleOffset"]["z"].GetDouble();

		mBCJ.mTranslateOffset.x = (float)obj["BoxCollider"]["TranslateOffset"]["x"].GetDouble();
		mBCJ.mTranslateOffset.y = (float)obj["BoxCollider"]["TranslateOffset"]["y"].GetDouble();
		mBCJ.mTranslateOffset.z = (float)obj["BoxCollider"]["TranslateOffset"]["z"].GetDouble();

		mbc_t = true;
	}
	else mbc_t = false;

	if (obj.HasMember("SphereCollider"))
	{
		mSCJ.mScaleOffset = (float)obj["SphereCollider"]["ScaleOffset"].GetDouble();

		mSCJ.mTranslateOffset.x = (float)obj["SphereCollider"]["TranslateOffset"]["x"].GetDouble();
		mSCJ.mTranslateOffset.y = (float)obj["SphereCollider"]["TranslateOffset"]["y"].GetDouble();
		mSCJ.mTranslateOffset.z = (float)obj["SphereCollider"]["TranslateOffset"]["z"].GetDouble();

		msc_t = true;
	}
	else msc_t = false;

	if (obj.HasMember("PlaneCollider"))
	{
		mPCJ.mNormal.x = (float)obj["PlaneCollider"]["Normal"]["x"].GetDouble();
		mPCJ.mNormal.y = (float)obj["PlaneCollider"]["Normal"]["y"].GetDouble();
		mPCJ.mNormal.z = (float)obj["PlaneCollider"]["Normal"]["z"].GetDouble();

		mPCJ.mTranslateOffset = (float)obj["PlaneCollider"]["TranslateOffset"].GetDouble();

		mpc_t = true;
	}
	else mpc_t = false;

	if (obj.HasMember("Scripts"))
	{
		Script tmp;

		for (int i = 0; i < obj["Scripts"]["ScriptsFiles"].Size(); ++i)
		{
			tmp.scriptFile = obj["Scripts"]["ScriptsFiles"][i]["Filename"].GetString();
			mSJ.scriptsContainer.push_back(tmp);
		}

		ms_t = true;
	}
	else ms_t = false;

	if (obj.HasMember("Parent"))
	{
		mPJ.mParent = (std::uint32_t)obj["Parent"]["Parent"].GetInt();
		mPJ.mPrevSibling = (std::uint32_t)obj["Parent"]["PreviousSibling"].GetInt();
		mPJ.mNextSibling = (std::uint32_t)obj["Parent"]["NextSibling"].GetInt();

		mp_t = true;
	}
	else mp_t = false;

	if (obj.HasMember("Children"))
	{
		mCJ.mNumChildren = (std::uint32_t)obj["Children"]["NumChildren"].GetInt();
		mCJ.mFirstChild = (std::uint32_t)obj["Children"]["FirstChild"].GetInt();

		mc_t = true;
	}
	else mc_t = false;

	if (obj.HasMember("Audio"))
	{
		mAJ.mFileName = obj["Audio"]["Filename"].GetString();
		mAJ.mAudioType = FindAudioEnum(obj["Audio"]["AudioType"].GetString());
		mAJ.mIsPlaying = obj["Audio"]["Playing"].GetBool();
		mAJ.mIsPlay = obj["Audio"]["Play"].GetBool();

		ma_t = true;
	}
	else ma_t = false;

	return true;
}

bool EntityJSON::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) const
{
	writer->StartObject();

	writer->String("EntityID");
	to_json_recursive(mID, *writer);

	writer->String("General");
	to_json_recursive(mGJ, *writer);

	writer->String("Transform");
	to_json_recursive(mTJ, *writer);

	if (mrb_t)
	{
		writer->String("RigidBody");
		to_json_recursive(mRBJ, *writer);
	}

	if (mmr_t)
	{
		writer->String("MeshRenderer");
		to_json_recursive(mMRJ, *writer);
	}

	if (mbc_t)
	{
		writer->String("BoxCollider");
		to_json_recursive(mBCJ, *writer);
	}

	if (msc_t)
	{
		writer->String("SphereCollider");
		to_json_recursive(mSCJ, *writer);
	}

	if (mpc_t)
	{
		writer->String("PlaneCollider");
		to_json_recursive(mPCJ, *writer);
	}

	if (ms_t)
	{
		writer->String("Scripts");
		to_json_recursive(mSJ, *writer);
	}

	if (mp_t)
	{
		writer->String("Parent");
		to_json_recursive(mPJ, *writer);
	}

	if (mc_t)
	{
		writer->String("Children");
		to_json_recursive(mCJ, *writer);
	}

	if (ma_t)
	{
		writer->String("Audio");
		to_json_recursive(mAJ, *writer);
	}

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

SceneJSON::SceneJSON() {}

SceneJSON::~SceneJSON() {}

bool SceneJSON::Deserialize(const rapidjson::Value& obj)
{
	if (obj.HasMember("Scene"))
	{
		sj.name = obj["Scene"]["SceneName"].GetString();
		sj.isPause = obj["Scene"]["Pause"].GetBool();
		sj.forceRender = obj["Scene"]["ForceRender"].GetBool();
	}

	return true;
}

bool SceneJSON::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) const
{
	writer->StartObject();

	writer->String("Scene");
	writer->StartObject();

	writer->String("SceneName");
	writer->String(sj.name.c_str());

	writer->String("Pause");
	writer->Bool(sj.isPause);

	writer->String("ForceRender");
	writer->Bool(sj.forceRender);

	writer->EndObject();

	writer->EndObject();

	return true;
}

bool SceneListJSON::Deserialize(const std::string& s)
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
		SceneJSON sce;
		sce.Deserialize(*ci);
		scenes.push_back(sce);
	}

	return true;
}
bool SceneListJSON::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) const
{
	writer->StartArray();

	for (std::list<SceneJSON>::const_iterator i = scenes.begin(); i != scenes.end(); i++)
	{
		(*i).Serialize(writer);
	}

	writer->EndArray();

	return true;
}

// helper functions for translating strings to tags

TAG FindTagEnum(std::string str)
{
	for (const auto& it : TagMap)
	{
		if (it.first == str)
			return it.second;
	}
}

SUBTAG FindSubTagEnum(std::string str)
{
	for (const auto& it : SubTagMap)
	{
		if (it.first == str)
			return it.second;
	}
}

MATERIAL FindMaterialEnum(std::string str)
{
	for (const auto& it : MaterialMap)
	{
		if (it.first == str)
			return it.second;
	}
}

MOTION FindMotionEnum(std::string str)
{
	for (const auto& it : MotionMap)
	{
		if (it.first == str)
			return it.second;
	}
}

AUDIOTYPE FindAudioEnum(std::string str)
{
	for (const auto& it : AudioMap)
	{
		if (it.first == str)
			return it.second;
	}
}

void SceneJSON::SetSceneJSON(const Scene scj)
{
	sj.name = scj.mName;
	sj.isPause = scj.mIsPause;
	sj.forceRender = scj.mForceRender;
}

const Scene SceneJSON::GetSceneJSON() const
{
	Scene scj;
	scj.mName = sj.name;
	scj.mIsPause = sj.isPause;
	scj.mForceRender = sj.forceRender;

	return scj;
}