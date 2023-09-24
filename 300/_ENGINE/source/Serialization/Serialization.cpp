#include "Serialization/Serialization.h"
//#include "Serialization/SerializationTemp.h"
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
		mID.id = (entt::entity)obj["EntityID"].GetInt();
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
		mTJ.mScale = { (float)obj["Transform"]["Scale"][0].GetDouble(),
					   (float)obj["Transform"]["Scale"][1].GetDouble(),
					   (float)obj["Transform"]["Scale"][2].GetDouble() };

		mTJ.mRotate = { (float)obj["Transform"]["Rotate"][0].GetDouble(),
						(float)obj["Transform"]["Rotate"][1].GetDouble(),
						(float)obj["Transform"]["Rotate"][2].GetDouble() };

		mTJ.mTranslate = { (float)obj["Transform"]["Translate"][0].GetDouble(),
						   (float)obj["Transform"]["Translate"][1].GetDouble(),
						   (float)obj["Transform"]["Translate"][2].GetDouble() };
	}

	if (obj.HasMember("RigidBody"))
	{
		mRBJ.mDensity = (std::uint16_t)obj["RigidBody"]["Density"].GetDouble();
		mRBJ.mMaterial = FindMaterialEnum(obj["RigidBody"]["Material"].GetString());
		mRBJ.mMotion = FindMotionEnum(obj["RigidBody"]["Motion"].GetString());

		mrb_t = true;
	}
	else mrb_t = false;

	if (obj.HasMember("MeshRenderer"))
	{
		mMRJ.mShaderPath = std::pair<std::string, std::string>(obj["MeshRenderer"]["ShaderPath"][0].GetString(), obj["MeshRenderer"]["ShaderPath"][1].GetString());
		for(int i = 0; i < obj["MeshRenderer"]["MaterialInstancePath"].Size(); ++i)
			mMRJ.mMaterialInstancePath.push_back(obj["MeshRenderer"]["MaterialInstancePath"][i].GetString());
		mMRJ.mMeshPath = obj["MeshRenderer"]["MeshPath"].GetString();
		mMRJ.mGUID = (unsigned)obj["MeshRenderer"]["GUID"].GetInt();

		mmr_t = true;
	}
	else mmr_t = false;


	if (obj.HasMember("BoxCollider"))
	{
		mBCJ.mScaleOffset = {(float)obj["BoxCollider"]["ScaleOffset"][0].GetDouble(),
							 (float)obj["BoxCollider"]["ScaleOffset"][1].GetDouble(),
							 (float)obj["BoxCollider"]["ScaleOffset"][2].GetDouble() };

		mBCJ.mTranslateOffset = { (float)obj["BoxCollider"]["TranslateOffset"][0].GetDouble(),
								  (float)obj["BoxCollider"]["TranslateOffset"][1].GetDouble(),
								  (float)obj["BoxCollider"]["TranslateOffset"][2].GetDouble() };

		mbc_t = true;
	}
	else mbc_t = false;

	if (obj.HasMember("SphereCollider"))
	{
		mSCJ.mScaleOffset = (float)obj["SphereCollider"]["ScaleOffset"].GetDouble();

		mSCJ.mTranslateOffset = { (float)obj["SphereCollider"]["TranslateOffset"][0].GetDouble(),
								  (float)obj["SphereCollider"]["TranslateOffset"][1].GetDouble(),
								  (float)obj["SphereCollider"]["TranslateOffset"][2].GetDouble() };

		msc_t = true;
	}
	else msc_t = false;

	if (obj.HasMember("PlaneCollider"))
	{
		mPCJ.mNormal = { (float)obj["PlaneCollider"]["Normal"][0].GetDouble(),
						 (float)obj["PlaneCollider"]["Normal"][1].GetDouble(),
						 (float)obj["PlaneCollider"]["Normal"][2].GetDouble() };

		mPCJ.mTranslateOffset = (float)obj["PlaneCollider"]["TranslateOffset"].GetDouble();

		mpc_t = true;
	}
	else mpc_t = false;

	if (obj.HasMember("Scripts"))
	{
		Script tmp;

		for (int i = 0; i < obj["Scripts"].Size(); ++i)
		{
			tmp.scriptFile = obj["Scripts"][i].GetString();
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
	writer->Int((int)mID.id);
	//to_json_recursive(mID, *writer);

	writer->String("General");
	writer->StartObject();

	writer->String("Name");
	writer->String(mGJ.name.c_str());

	writer->String("Active");
	writer->Bool(mGJ.isActive);

	writer->String("Paused");
	writer->Bool(mGJ.isPaused);

	writer->String("Tag");
	writer->String(FindTagString(mGJ.tag).c_str());

	writer->String("Subtag");
	writer->String(FindSubTagString(mGJ.subtag).c_str());
	
	writer->EndObject();
	//to_json_recursive(mGJ, *writer);

	writer->String("Transform");
	writer->StartObject();

	writer->String("Scale");
	writer->StartArray();
	writer->Double(mTJ.mScale.x);
	writer->Double(mTJ.mScale.y);
	writer->Double(mTJ.mScale.z);
	writer->EndArray();

	writer->String("Rotate");
	writer->StartArray();
	writer->Double(mTJ.mRotate.x);
	writer->Double(mTJ.mRotate.y);
	writer->Double(mTJ.mRotate.z);
	writer->EndArray();

	writer->String("Translate");
	writer->StartArray();
	writer->Double(mTJ.mTranslate.x);
	writer->Double(mTJ.mTranslate.y);
	writer->Double(mTJ.mTranslate.z);
	writer->EndArray();

	writer->EndObject();
	//to_json_recursive(mTJ, *writer);

	if (mrb_t)
	{
		writer->String("RigidBody");
		writer->StartObject();

		writer->String("Density");
		writer->Uint(mRBJ.mDensity);

		writer->String("Material");
		writer->String(FindMaterialString(mRBJ.mMaterial).c_str());

		writer->String("Motion");
		writer->String(FindMotionString(mRBJ.mMotion).c_str());

		writer->EndObject();
		//to_json_recursive(mRBJ, *writer);
	}

	if (mmr_t)
	{
		writer->String("MeshRenderer");
		writer->StartObject();

		writer->String("ShaderPath");
		writer->StartArray();
		writer->String(mMRJ.mShaderPath.first.c_str());
		writer->String(mMRJ.mShaderPath.second.c_str());
		writer->EndArray();

		writer->String("MaterialInstancePath");
		writer->StartArray();
		for (int i = 0; i < mMRJ.mMaterialInstancePath.size(); ++i)
			writer->String(mMRJ.mMaterialInstancePath[i].c_str());
		writer->EndArray();

		writer->String("MeshPath");
		writer->String(mMRJ.mMeshPath.c_str());

		writer->String("GUID");
		writer->Int(mMRJ.mGUID);

		writer->EndObject();
		//to_json_recursive(mMRJ, *writer);
	}

	if (mbc_t)
	{
		writer->String("BoxCollider");
		writer->StartObject();

		writer->String("ScaleOffset");
		writer->StartArray();
		writer->Double(mBCJ.mScaleOffset.x);
		writer->Double(mBCJ.mScaleOffset.y);
		writer->Double(mBCJ.mScaleOffset.z);
		writer->EndArray();

		writer->String("TranslateOffset");
		writer->StartArray();
		writer->Double(mBCJ.mTranslateOffset.x);
		writer->Double(mBCJ.mTranslateOffset.y);
		writer->Double(mBCJ.mTranslateOffset.z);
		writer->EndArray();

		writer->EndObject();
		//to_json_recursive(mBCJ, *writer);
	}

	if (msc_t)
	{
		writer->String("SphereCollider");
		writer->StartObject();

		writer->String("ScaleOffset");
		writer->Double(mSCJ.mScaleOffset);

		writer->String("TranslateOffset");
		writer->StartArray();
		writer->Double(mSCJ.mTranslateOffset.x);
		writer->Double(mSCJ.mTranslateOffset.y);
		writer->Double(mSCJ.mTranslateOffset.z);
		writer->EndArray();

		writer->EndObject();
		//to_json_recursive(mSCJ, *writer);
	}

	if (mpc_t)
	{
		writer->String("PlaneCollider");
		writer->StartObject();

		writer->String("Normal");
		writer->StartArray();
		writer->Double(mPCJ.mNormal.x);
		writer->Double(mPCJ.mNormal.y);
		writer->Double(mPCJ.mNormal.z);
		writer->EndArray();

		writer->String("TranslateOffset");
		writer->Double(mPCJ.mTranslateOffset);

		writer->EndObject();
		//to_json_recursive(mPCJ, *writer);
	}

	if (ms_t)
	{
		writer->String("Scripts");
		writer->StartArray();

		for (int i = 0; i < mSJ.scriptsContainer.size(); ++i)
			writer->String(mSJ.scriptsContainer[i].scriptFile.c_str());

		writer->EndArray();
		//to_json_recursive(mSJ, *writer);
	}

	if (mp_t)
	{
		writer->String("Parent");
		writer->StartObject();

		writer->String("Parent");
		writer->Int(mPJ.mParent);

		writer->String("PreviousSibling");
		writer->Int(mPJ.mPrevSibling);

		writer->String("NextSibling");
		writer->Int(mPJ.mNextSibling);

		writer->EndObject();
		//to_json_recursive(mPJ, *writer);
	}

	if (mc_t)
	{
		writer->String("Children");
		writer->StartObject();

		writer->String("NumChildren");
		writer->Int(mCJ.mNumChildren);

		writer->String("FirstChild");
		writer->Int(mCJ.mFirstChild);

		writer->EndObject();
		//to_json_recursive(mCJ, *writer);
	}

	if (ma_t)
	{
		writer->String("Audio");
		writer->StartObject();

		writer->String("Filename");
		writer->String(mAJ.mFileName.c_str());

		writer->String("AudioType");
		writer->String(FindAudioString(mAJ.mAudioType).c_str());

		writer->String("Playing");
		writer->Bool(mAJ.mIsPlaying);

		writer->String("Play");
		writer->Bool(mAJ.mIsPlay);

		writer->EndObject();
		//to_json_recursive(mAJ, *writer);
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
		mScene.mName = obj["Scene"]["SceneName"].GetString();
		mScene.mIsPause = obj["Scene"]["Pause"].GetBool();
		mScene.mForceRender = obj["Scene"]["ForceRender"].GetBool();
	}

	return true;
}

bool SceneJSON::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) const
{
	writer->StartObject();

	writer->String("Scene");
	writer->StartObject();

	writer->String("SceneName");
	writer->String(mScene.mName.c_str());

	writer->String("Pause");
	writer->Bool(mScene.mIsPause);

	writer->String("ForceRender");
	writer->Bool(mScene.mForceRender);

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

std::string FindTagString(TAG tag)
{
	for (const auto& it : TagMap)
	{
		if (it.second == tag)
			return it.first;
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

std::string FindSubTagString(SUBTAG subtag)
{
	for (const auto& it : SubTagMap)
	{
		if (it.second == subtag)
			return it.first;
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

std::string FindMaterialString(MATERIAL tag)
{
	for (const auto& it : MaterialMap)
	{
		if (it.second == tag)
			return it.first;
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

std::string FindMotionString(MOTION tag)
{
	for (const auto& it : MotionMap)
	{
		if (it.second == tag)
			return it.first;
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

std::string FindAudioString(AUDIOTYPE tag)
{
	for (const auto& it : AudioMap)
	{
		if (it.second == tag)
			return it.first;
	}
}

//template <typename T>
//std::string FindString(T& val, const std::unordered_map<std::string, T>& map)
//{
//	for (const auto& it : map)
//	{
//		if (it.second == val)
//			return it.first;
//	}
//}
//
//template <typename T>
//T FindEnum(const std::string& str, const std::unordered_map<std::string, T>& map)
//{
//	for (const auto& it : map)
//	{
//		if (it.first == str)
//			return it.second;
//	}
//}