
/*!*************************************************************************
****
\file		   ECS_Components.h
\author(s)	   
\par DP email:
\date		   16-8-2023
\brief
Components used by the ECS.
****************************************************************************/

#pragma once
#include "glm/glm.hpp"
#include "Script.h"
#include <vector>
#include "Physics/PhysicsTypes.h"
#include "Tags.h"
#include "ECS.h"
#include "Audio/AudioType.h"
#include <Animator.hpp>
#include <Camera.hpp>
#include "EnumStrings.h"
#include "Input/KeyBind.h"
#include "Input/Input.h"
//#include "Graphics/GraphicsSystem.h"
#include "Mesh.hpp"
#include "Serialization/Serialization.h"
//#include "rttr/registration.h"

//DECLARE_ENUMSTRING(enum_tag, PLAYER, ENEMY, BULLET, STATIC, BUILDING)

/******************************************************************************/
/*!
	[Component] - General
 */
 /******************************************************************************/
struct General : public Serializable
{
	std::string name;
	/*TAG tag;*/
	//enum_tag::enum_tag tag;
	//enum_tag::enum_tag tagid;
	std::string tag[5] = { "PLAYER","ENEMY","BULLET","STATIC","BUILDING" };
	int tagid{ 0 };
	SUBTAG subtag;
	bool isActive{};
	bool isPaused{};

	General() 
	: name(""), subtag(SUBTAG::ACTIVE), isActive(true) 
	{};

	void Inspect();
	void SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const
	{
		writer.StartObject();
		Serialize(writer, "name", name);
		Serialize(writer, "tagid", tagid);
		Serialize(writer, "subtag", static_cast<int>(subtag));
		Serialize(writer, "isActive", isActive);
		Serialize(writer, "isPaused", isPaused);
		writer.EndObject();
	}
	//RTTR_ENABLE()
};

/******************************************************************************/
/*!
	[Component] - Transform
 */
 /******************************************************************************/
struct Transform : public Serializable
{
	glm::vec3 mScale;
	glm::vec3 mRotate;
	glm::vec3 mTranslate;

	Transform() : mScale(1.f), mRotate(0.f), mTranslate(0.f) {}
	glm::quat GetQuaternion() { return glm::quat(mRotate); }
	void Inspect();

	void SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const
	{
		writer.StartObject();
		Serialize(writer, "scale", mScale);
		Serialize(writer, "rotate", mRotate);
		Serialize(writer, "translate", mTranslate);
		writer.EndObject();
	}
	//RTTR_ENABLE()
};

/******************************************************************************/
/*!
	[Component] - Animator
 */
 /******************************************************************************/
struct Animator
{
	GFX::Animator	mAnimator;

	void Inspect();
};

/******************************************************************************/
/*!
	[Component] - RigidBody
 */
 /******************************************************************************/
struct RigidBody : public Serializable
{
	float mDensity;
	MATERIAL mMaterial;
	MOTION mMotion;
	glm::vec3 mVelocity;

	RigidBody() : mDensity(10.f), mMaterial(MATERIAL::WOOD), mMotion(MOTION::STATIC), mVelocity(0.f) {};
	//RTTR_ENABLE()


	int mMat{ 0 };
	int mMot{ 0 };
	void							Inspect();

	void SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const
	{
		writer.StartObject();
		Serialize(writer, "density", mDensity);
		Serialize(writer, "material", static_cast<int>(mMaterial));
		Serialize(writer, "motion", static_cast<int>(mMotion));
		Serialize(writer, "velocity", mVelocity);
		writer.EndObject();
	}
};

/******************************************************************************/
/*!
	[Component] - MeshRenderer
 */
 /******************************************************************************/
// this struct stores the filepaths for the meshdata, material, and shader. the actual data is stored in the resource manager
struct MeshRenderer : public Serializable
{
	// For now, we store the string to the filepaths. TO CHANGE to uids for efficient referencing
	std::pair<std::string, std::string> mShaderPath{ "../assets/shader_files/pointLight_vert.glsl", "../assets/shader_files/pointLight_frag.glsl" };
	std::string							mMaterialInstancePath[4] {" "," " ," " ," " };
	std::string							mMeshPath;
	
	void*								mMeshRef{};
	void*								mTextureRef[4];
	bool								mTextureCont[4];

	unsigned							mGUID;

	void								Inspect();



	GFX::Mesh							testmesh;
	//RTTR_ENABLE()


	//void								Inspect();
	void SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const
	{
		writer.StartObject();
		Serialize(writer, "vert", mShaderPath.first);
		Serialize(writer, "frag", mShaderPath.second);
		Serialize(writer, "materialinstance", mMaterialInstancePath, 4);
		Serialize(writer, "mesh", mMeshPath);
		Serialize(writer, "texturecont", mTextureCont, 4);
		Serialize(writer, "guid", mGUID);
		writer.EndObject();
	}
};


/******************************************************************************/
/*!
	[Component] - BoxCollider
 */
 /******************************************************************************/
struct BoxCollider : public Serializable
{
	glm::vec3 mScaleOffset;			// final scale = mScaleOffset * Transform.mScale;
	glm::vec3 mTranslateOffset;		// final pos = Transform.mTranslate + mTranslateOffset;
	

	BoxCollider() : mScaleOffset(1.f), mTranslateOffset(0.f) {}
	
	//RTTR_ENABLE()
	void							Inspect();
	void SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const
	{
		writer.StartObject();
		Serialize(writer, "scaleoffset", mScaleOffset);
		Serialize(writer, "translateoffset", mTranslateOffset);
		writer.EndObject();
	}
};

/******************************************************************************/
/*!
	[Component] - SphereCollider
 */
 /******************************************************************************/
struct SphereCollider : public Serializable
{
	float mScaleOffset;				// final scale = mScaleOffset * std::max(Transform.mScale.x, Transform.mScale.y, Transform.mScale.z);
	glm::vec3 mTranslateOffset;		// final pos = Transform.mTranslate + mTranslateOffset;

	SphereCollider() : mScaleOffset(1.f), mTranslateOffset(0.f) {};

	//RTTR_ENABLE()
	void							Inspect();
	void SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const
	{
		writer.StartObject();
		Serialize(writer, "scaleoffset", mScaleOffset);
		Serialize(writer, "translateoffset", mTranslateOffset);
		writer.EndObject();
	}
};

/******************************************************************************/
/*!
	[Component] - PlaneCollider
 */
 /******************************************************************************/
struct PlaneCollider : public Serializable //if has plane collider always static
{
	glm::vec3 mNormal;				// direction of plane
	float mTranslateOffset;			// final pos = magnitude(Transform.mTranslate) + mTranslateOffset;

	PlaneCollider() : mNormal(0.f, 1.f, 0.f), mTranslateOffset(0.f) {};

	//RTTR_ENABLE()
	void							Inspect();
	void SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const
	{
		writer.StartObject();
		Serialize(writer, "normal", mNormal);
		Serialize(writer, "translateoffset", mTranslateOffset);
		writer.EndObject();
	}
};

/******************************************************************************/
/*!
	[Component] - Scripts
 */
 /******************************************************************************/
class Scripts : public Serializable {
public:
	Scripts() = default;
	~Scripts() = default;

	static void AddScript(Entity id, std::string fileName);
	//static void LoadRunScript(Entity entity);

	std::string mScriptFile{};
	std::vector <Script> scriptsContainer;

	//RTTR_ENABLE()
	void Inspect();
	void SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const
	{
		writer.StartObject();
		Serialize(writer, "scriptfile", mScriptFile);
		Serialize(writer, "scriptscontainer", scriptsContainer);
		writer.EndObject();
	}
};

/******************************************************************************/
/*!
	[Component] - Parent
 */
 /******************************************************************************/
struct Parent : public Serializable
{
	std::uint32_t mPrevSibling;
	std::uint32_t mNextSibling;
	std::uint32_t mParent;

	Parent() : mPrevSibling(0), mNextSibling(0), mParent(0) {};
	void SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const
	{
		writer.StartObject();
		Serialize(writer, "prevsibling", mPrevSibling);
		Serialize(writer, "nextsibling", mNextSibling);
		Serialize(writer, "parent", mParent);
		writer.EndObject();
	}
	//RTTR_ENABLE()
};

/******************************************************************************/
/*!
	[Component] - Children
 */
 /******************************************************************************/
struct Children : public Serializable
{
	std::uint32_t mNumChildren;	
	std::uint32_t mFirstChild;

	Children() : mNumChildren(0), mFirstChild(0) {};
	void SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const
	{
		writer.StartObject();
		Serialize(writer, "numchildren", mNumChildren);
		Serialize(writer, "firstchild", mFirstChild);
		writer.EndObject();
	}
	//RTTR_ENABLE()
};


/******************************************************************************/
/*!
	[Component] - Audio
 */
 /******************************************************************************/

struct Audio : public Serializable
{
	std::string mFilePath;				// File Path to the Audio File
	std::string mFileName;				// Name of Audio file
	std::string mFullPath;				// 
	AUDIOTYPE mAudioType;				// SFX or BGM
	bool mIsPlay;						// play audio if true

	// Don't need to serialize ...
	std::vector<int> mPlaySFXChannelID;    // Currently playing in SFX Channel...
	std::vector<int> mPlayBGMChannelID;	   // Currently playing in BGM Channel ...
	bool             mIsPlaying;		   // Check if audio is already playing

	// For Editor
	bool			 mIsEmpty = false;	   // [For Editor] - if empty delete all data in this <Audio> component
	bool			 mIsLoaded = false;	   // [For Loading]

	Audio() : mFilePath("../assets/Audio"), mFileName("Empty Audio"), mAudioType(AUDIO_NULL), mIsPlaying(false), mIsPlay(false), mIsEmpty(true) {}

	Audio(std::string file_path_to_audio, std::string file_audio_name, AUDIOTYPE audio_type, bool isPlay) : mAudioType(audio_type), mIsPlaying(false), mIsPlay(isPlay)
	{
		mFilePath = file_path_to_audio;
		mFileName = file_audio_name;
		mFullPath = file_path_to_audio + "/" + mFileName;
	}

	void ClearAudioComponent()
	{
		mFilePath = "";
		mFileName = "";
		mFullPath = "";
		mAudioType = AUDIO_NULL;
		mIsPlay = false;
		mIsEmpty = true;
	}


	int mAudio{ 0 };

	void SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const
	{
		writer.StartObject();
		Serialize(writer, "filename", mFileName);
		Serialize(writer, "audiotype", static_cast<int>(mAudioType));
		Serialize(writer, "playing", mIsPlaying);
		Serialize(writer, "play", mIsPlay);
		writer.EndObject();
	}

	//RTTR_ENABLE()
	void							Inspect();
};

/******************************************************************************/
/*!
	[Component] - Camera
 */
 /******************************************************************************/
struct Camera : public Serializable
{
	GFX::Camera						mCamera;
	void							Inspect();

	void SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const
	{
		writer.StartObject();
		Serialize(writer, "positon", mCamera.mPosition);
		Serialize(writer, "target", mCamera.mTarget);
		Serialize(writer, "size", mCamera.mSize);
		Serialize(writer, "near", mCamera.mNear);
		Serialize(writer, "far", mCamera.mFar);
		writer.EndObject();
	}
};

/******************************************************************************/
/*!
	[Component] - Prefab
 */
 /******************************************************************************/
struct Prefab : public Serializable
{
	std::string mPrefab;
	void SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const
	{
		writer.StartObject();
		Serialize(writer, "prefab", mPrefab);
		writer.EndObject();
	}
};

/******************************************************************************/
/*!
	[Component] - PointLight
 */
 /******************************************************************************/
struct PointLight : public Serializable
{
	vec3	mLightColor{ 1.f, 1.f, 1.f };
	float	mAttenuation{};
	float	mIntensity{};
	void Inspect();

	void SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const
	{
		writer.StartObject();
		Serialize(writer, "lightcolor", mLightColor);
		Serialize(writer, "attenuation", mAttenuation);
		Serialize(writer, "intensity", mIntensity);
		writer.EndObject();
	}
};


// Added [9/27]
// Pseudo-Component (Helps InputActionMapEditor)

/******************************************************************************/
/*!
	[Support] InputActionMap()
 */
 /******************************************************************************/
struct PseudoInputAction
{
	std::string							      mActionName;
	std::unordered_map<KEY_BIND, E_KEY>       mDefinedKeyMap;
	std::unordered_map<KEY_BIND, MAP_TO_AXIS> mMapToAxis;

	PseudoInputAction() : mActionName("Sample Action")
	{
		mDefinedKeyMap[KEY_BIND::KEY_UP] = E_KEY::UNKNOWN;
		mDefinedKeyMap[KEY_BIND::KEY_DOWN] = E_KEY::UNKNOWN;
		mDefinedKeyMap[KEY_BIND::KEY_LEFT] = E_KEY::UNKNOWN;
		mDefinedKeyMap[KEY_BIND::KEY_RIGHT] = E_KEY::UNKNOWN;

		mMapToAxis[KEY_BIND::KEY_UP] = Y_POSITIVE;
		mMapToAxis[KEY_BIND::KEY_DOWN] = Y_NEGATIVE;
		mMapToAxis[KEY_BIND::KEY_LEFT] = X_NEGATIVE;
		mMapToAxis[KEY_BIND::KEY_RIGHT] = X_POSITIVE;
	}

	PseudoInputAction(std::string action_name) : mActionName(action_name)
	{
		mDefinedKeyMap[KEY_BIND::KEY_UP] = E_KEY::UNKNOWN;
		mDefinedKeyMap[KEY_BIND::KEY_DOWN] = E_KEY::UNKNOWN;
		mDefinedKeyMap[KEY_BIND::KEY_LEFT] = E_KEY::UNKNOWN;
		mDefinedKeyMap[KEY_BIND::KEY_RIGHT] = E_KEY::UNKNOWN;

		mMapToAxis[KEY_BIND::KEY_UP] = Y_POSITIVE;
		mMapToAxis[KEY_BIND::KEY_DOWN] = Y_NEGATIVE;
		mMapToAxis[KEY_BIND::KEY_LEFT] = X_NEGATIVE;
		mMapToAxis[KEY_BIND::KEY_RIGHT] = X_POSITIVE;
	}

	void AddKeyBinding(KEY_BIND key_bind, E_KEY key)
	{
		mDefinedKeyMap[key_bind] = key;
	}

	void RemoveKeyBinding(KEY_BIND key_bind)
	{
		mDefinedKeyMap[key_bind] = E_KEY::UNKNOWN;
	}

	void LinkKeyBinding(KEY_BIND key_bind, E_KEY key)
	{
		mDefinedKeyMap[key_bind] = key;
	}

	glm::vec2 ReadValue(E_KEY key_pressed) const
	{
		glm::vec2 vec;
		KEY_BIND key_bind;

		for (auto& defined_key_pair : mDefinedKeyMap)
		{
			if (defined_key_pair.second == key_pressed)
			{
				key_bind = defined_key_pair.first;
			}
		}

		auto map_axis_it = mMapToAxis.find(key_bind);

		if (map_axis_it != mMapToAxis.end())
		{
			switch (map_axis_it->second)
			{
				case X_POSITIVE:
					vec = { 1.0f, 0.0f };
					return vec;
				case X_NEGATIVE:
					vec = { -1.0f, 0.0f };
					return vec;
				case Y_POSITIVE:
					vec = { 0.0f , 1.0f };
					return vec;
				case Y_NEGATIVE:
					vec = { 0.0f , -1.0f };
					return vec;
			}
		}
		vec = { 0.0f, 0.0f };
		return vec;
	}

	int mKeyBindUp{ 0 };
	int mKeyBindDown{ 0 };
	int mKeyBindLeft{ 0 };
	int mKeyBindRight{ 0 };

	// [For Editor]
	std::string mSelectedBindingUP{ " " };
	std::string mSelectedBindingDOWN{ " " };
	std::string mSelectedBindingLEFT{ " " };
	std::string mSelectedBindingRIGHT{ " " };
	// [For Update]
	bool isEnabled = true;
};

/******************************************************************************/
/*!
	[Component] InputActionMapEditor
 */
 /******************************************************************************/
struct InputActionMapEditor
{
	// Action Map -> contains (Action) -> contains (E_KEY)
	// std::unordered_map<std::string, E_KEY> mActionMapNew;
	std::unordered_map <std::string, PseudoInputAction> mActionMap;

	InputActionMapEditor() {} // Just Initalizes the mActionMap.

	InputActionMapEditor(std::string action_name) {
		mActionMap[action_name] = PseudoInputAction();
	}

	void AddAction(std::string action_name)
	{
		PseudoInputAction new_action = PseudoInputAction(action_name);
		mActionMap[action_name] = new_action;
	}

	void AddActionMap(std::string map_name)
	{
		mActionMap[map_name] = PseudoInputAction();
	}

	PseudoInputAction& GetAction(std::string action_name)
	{
		return mActionMap[action_name];
	}


	void Inspect();

	// [For Editor]
	int mAction{ 0 };
	std::string mSelectedMapName{ " " };
	bool selected = false;

};


