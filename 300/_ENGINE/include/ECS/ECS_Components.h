
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

//#define TAGCOUNT 32

#include "glm/glm.hpp"
#include <algorithm>
#include "Script.h"
#include <vector>
#include "Physics/PhysicsTypes.h"
#include "Tags.h"
#include "ECS.h"
#include "Audio/AudioType.h"
#include "../../../lib/FMOD/core/inc/fmod.hpp"
#include <Animator.hpp>
#include <Camera.hpp>
#include <string>
//#include "EnumStrings.h"
//#include "EnumTags.h"
#include "Input/Input.h"
#include "Guid.h"
#include <Constants.h>
#include "ResourceManagerTy.h"
#include "Serialization/Serialization.h"
#include "Texture.hpp"
//#include "Graphics/GraphicsSystem.h"
//#include "Mesh.hpp"

//#include "rttr/registration.h"

//DECLARE_ENUMSTRING(enum_tag, PLAYER, ENEMY, BULLET, STATIC, BUILDING)
struct uid;

namespace GFX {
	struct Mesh;

}

/******************************************************************************/
/*!
	[Component] - General
 */
 /******************************************************************************/
struct General : public Serializable
{
	std::string name;
	/*TAG tag;*/
	//enum_tag::enum_tag tag{};
	unsigned char tagid = 0; //@han
	//std::string tag[5] = { "PLAYER","ENEMY","BULLET","STATIC","BUILDING" };
	//int tagid{ 0 };
	SUBTAG subtag;
	bool isActive{};
	bool isDelete{};
	bool isPaused{};

	General() 
	: name(""), subtag(SUBTAG::ACTIVE), isActive(true), isDelete(false)
	{};

	std::string GetTag() { return ECS::GetTag(tagid); }

	void SetTag(const std::string& tagName) { tagid = ECS::GetTag(tagName); }


	void Inspect();
	void SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const;
	void DeserializeSelf(rapidjson::Value& reader);

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
	void SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const;
	void DeserializeSelf(rapidjson::Value& reader);
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
	void PauseAnimation() { mAnimator.mIsPaused = true; }
	void UnpauseAnimation() { mAnimator.mIsPaused = false; }
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
	//std::pair<std::string, std::string> mShaderPath{ "../assets/shader_files/animations_vert.glsl", "../assets/shader_files/pointLight_frag.glsl" };
	
	std::pair<std::string, std::string> mShaderPath{ "../assets/shader_files/pointLight_vert.glsl", "../assets/shader_files/pointLight_frag.glsl" };
	
	ref									mShaderRef;

	std::string							mMaterialInstancePath[5] {" "," " ," " ," ", " "};
	vec4								mInstanceColor{ 1.f, 1.f, 1.f, 1.f };

	std::string							mMeshPath;

	ref									mMeshRef{};
	ref									mTextureRef[5]		{ {nullptr,0},{nullptr,0},{nullptr,0},{nullptr,0},{nullptr,0} };

	// store the descriptor data for the material instance per mesh renderer instance
	_GEOM::Texture_DescriptorData		mTextureDescriptorData[5];
	

	unsigned							mGUID;
	//bool								mTextureCont[5];

	void								Inspect();
	void								SetColor(const vec4& color);
	void								SetMesh(const std::string& meshName, Entity inst);
	void								SetTexture(MaterialType type, const std::string& Texturename);

	
	int									GetTexture(MaterialType type) 
										{ 
											if (mTextureRef[static_cast<int>(type)].data == nullptr)
												return -1;

											return static_cast<GFX::Texture*>(mTextureRef[static_cast<int>(type)].data)->ID();
										}

	void SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const;
	void DeserializeSelf(rapidjson::Value& reader);
	//RTTR_ENABLE()
};


struct UIrenderer : public Serializable
{
	std::string							mTexPath; // temporary should be UID
	ref									mTextureRef;
	float								mDegree;

	inline unsigned ID() 
	{
		//if (mTextureRef != nullptr) {
		//	int temp = (reinterpret_cast<GFX::Texture*>(mTextureRef))->ID();
		//	return temp;
		//}

		return 0;
	}
	void Inspect();
	void SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const;
	void DeserializeSelf(rapidjson::Value& reader);
};



/******************************************************************************/
/*!
	[Component] - RigidBody
 */
 /******************************************************************************/
struct RigidBody : public Serializable //@han
{
	float mDensity;
	MATERIAL mMaterial;
	MOTION mMotion;
	glm::vec3 mVelocity;
	glm::bvec3 mRotationConstraints;
	bool mGravity;

	RigidBody() : mDensity(10.f), mMaterial(MATERIAL::WOOD), mMotion(MOTION::STATIC), mVelocity(0.f), mGravity(true) {};
	RigidBody(float dense, MATERIAL mat, MOTION mot, const glm::vec3& vec)
		: mDensity(dense), mMaterial(mat), mMotion(mot), mVelocity(vec){}
	//RTTR_ENABLE()

	void							Inspect();
	void SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const;
	void DeserializeSelf(rapidjson::Value& reader);
};

/******************************************************************************/
/*!
	[Component] - BoxCollider
 */
 /******************************************************************************/
struct BoxCollider : public Serializable //@han
{
	glm::vec3 mScaleOffset;			// final scale = mScaleOffset * Transform.mScale;
	glm::vec3 mTranslateOffset;		// final pos = Transform.mTranslate + mTranslateOffset;
	bool mIsTrigger;

	BoxCollider() : mScaleOffset(1.f), mTranslateOffset(0.f), mIsTrigger(false){}
	
	//RTTR_ENABLE()
	void							Inspect();
	void SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const;
	void DeserializeSelf(rapidjson::Value& reader);
};

/******************************************************************************/
/*!
	[Component] - SphereCollider
 */
 /******************************************************************************/
struct SphereCollider : public Serializable //@han
{
	float mScaleOffset;				// final scale = mScaleOffset * std::max(Transform.mScale.x, Transform.mScale.y, Transform.mScale.z);
	glm::vec3 mTranslateOffset;		// final pos = Transform.mTranslate + mTranslateOffset;
	bool mIsTrigger;

	SphereCollider() : mScaleOffset(1.f), mTranslateOffset(0.f), mIsTrigger(false) {};

	//RTTR_ENABLE()
	void							Inspect();
	void SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const;
	void DeserializeSelf(rapidjson::Value& reader);
};

struct CapsuleCollider : public Serializable //@han
{
	glm::vec3 mTranslateOffset;
	float mRadius;
	float mHalfHeight;
	bool mIsTrigger;


	CapsuleCollider() : mTranslateOffset(0.f, 0.f, 0.f), mRadius(50.f), mHalfHeight(100.f) {}
	void SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const;
	void DeserializeSelf(rapidjson::Value& reader);

	void Inspect();
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

	//static void LoadRunScript(Entity entity);

	//std::string mScriptFile{};
	std::vector <Script> scriptsContainer;

	template<typename ...args>
	void RunFunctionForAllScripts(const char* funcName, args... arguments)
	{
		for (auto& elem : scriptsContainer)
		{
			elem.Run(funcName, arguments...);
		}
	}

	//RTTR_ENABLE()
	void Inspect(entt::entity entityID);
	void SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const;
	void DeserializeSelf(rapidjson::Value& reader);

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

	void SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const;
	void DeserializeSelf(rapidjson::Value& reader);

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

	void SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const;
	void DeserializeSelf(rapidjson::Value& reader);
	//RTTR_ENABLE()
};


/******************************************************************************/
/*!
	[Component] - Audio
 */
 /******************************************************************************/

struct Audio : public Serializable
{
	// Serialize
	// -----------------------------------------
	std::string    mFilePath;				            // File Path to the Audio File (required for loading)
	std::string    mFileName;				            // Name of Audio file (required for loading)
	std::string    mFullPath;				            // Full Path (File Path + Audio File)

	bool           mPlayonAwake = false;		        // [Flag] - flag to play as the scene launches. 
	bool           mIsLooping = false;			        // [Flag] - flag to decide whether if audio is looping.

	// Audio Type [Channel Management]
	AUDIOTYPE      mAudioType;			                // SFX or BGM (Mute Channels)

	// Volume 
	float		   mVolume = 1.0f;

	// 3D Audio
	bool		   m3DAudio = false;
	float		   mMinDistance = 0.5f;		         // Testing Values
	float		   mMaxDistance = 3000.0f;		         // Testing Values

	// Position
	glm::vec3	   mPosition = { 0.0f, 0.0f, 0.0f }; // Q. <Transform> or glm::vec3
	glm::vec3      mVelocity = { 0.0f, 0.0f, 0.0f }; // For [Doppler] effect. 

	// Do not serialize 
	// ------------------------------------------
	// Update Loop - Boolean Checks
	bool		   mIsPlaying = false;					 // [Flag] - Check if audio is already playing (Channel Interaction)
	bool           mIsPlay = false;						 // [Flag] - to decide whether to play audio (if true)

	// Update Loop - Fade In / Fade out data
	bool		   mFadeIn = false;						 // [Flag] - This audio will be faded out. 
	bool		   mFadeOut = false;					 // [Flag] - This audio will be faded in.
	float		   mFadeInMaxVol = 1.0f;				 // Flexibility with audio volume fade in (control over volume)
	float		   mFadeOutToVol = 0.0f;				 // Flexibility to adjust the audio volume as it fades out (don't have to be 0.0f)
	float		   mFadeSpeedModifier = 0.2f;			 // How fast the fading goes (modifier * dt)

	// For Editor
	bool		   mIsEmpty = true;						 // [For Editor] - if empty delete all data in this <Audio> component
	bool		   mIsLoaded = false;					 // [For Loading]

	// Pause State [Editor/Pause Menu]
	bool		   mSetPause = false;					 // [Flag] - set pause for channel.
	bool		   mPaused = false;						 // [For Resuming Logic]
	bool		   mWasPaused = false;					 // [For Resuming Logic]
	bool		   mSetUnpause = false;					 // [Flag] - for unpausing channels.
	float		   mTypeChanged = false;				 // [For Editor] - trigger type change

	// Q. Can a <Audio> entity have their very own channel.
	uid            mChannelID;							 // Channel ID (Channel Management)
	FMOD::Channel* mChannel;         					 // Use this to facilitate manipulation of audio.
	FMOD::Sound* mSound = nullptr;					 // Each <Audio> can only hold a reference to the "Audio File" it's attached to.

	// Fade Volume Stuff
	float fade_timer = 0.0f;							 // How long the fade has elapsed
	float fade_duration = 5.0f;							 // How long to fade...

	Audio() : mFilePath(""), mFileName(""), mAudioType(AUDIO_SFX), mIsEmpty(true)
	{
		mChannelID = uid();
	}

	Audio(std::string file_path_to_audio, std::string file_audio_name, AUDIOTYPE audio_type, bool playOnAwake) : mAudioType(audio_type), mIsPlaying(false), mPlayonAwake(playOnAwake),
		mIsEmpty(false)
	{
		mFilePath = file_path_to_audio;
		mFileName = file_audio_name;
		mFullPath = file_path_to_audio + "/" + mFileName;

		mChannelID = uid();
	}

	// For [Editor]
	void ClearAudioComponent()
	{
		mFilePath = "";
		mFileName = "";
		mFullPath = "";
		mAudioType = AUDIO_SFX;
		mIsPlay = false;
		mPlayonAwake = false;
		mIsEmpty = true;
		mIsLoaded = false;
		m3DAudio = false; // Added [10/26]
	}

	int mAudio{ 0 };

	//RTTR_ENABLE()
	void							Inspect();
	void SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const;
	void DeserializeSelf(rapidjson::Value& reader);
};

struct AudioListener
{
	// Position
	glm::vec3	   mPosition = { 0.0f, 0.0f, 0.0f }; // Q. <Transform> or glm::vec3
	glm::vec3      mVelocity = { 0.0f, 0.0f, 0.0f }; // For [Doppler] effect. 
	glm::vec3	   mForward  = { 0.0f, 0.0f, 0.0f };
	glm::vec3	   mUp       = { 0.0f, 0.0f, 0.0f };

	int mAudioListener{ 0 };

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
	void SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const;
	void DeserializeSelf(rapidjson::Value& reader);
};

/******************************************************************************/
/*!
	[Component] - Prefab
 */
 /******************************************************************************/
struct Prefab : public Serializable
{
	std::string mPrefab;

	void SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const;
	void DeserializeSelf(rapidjson::Value& reader);
};

/******************************************************************************/
/*!
	[Component] - PointLight
 */
 /******************************************************************************/
struct PointLight : public Serializable
{
	vec3	mLightColor{ 1.f, 1.f, 1.f };
	float	mLinearFalloff{};
	float	mQuadraticFalloff{};
	float	mIntensity{ 1.5f };

	void Inspect();

	void SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const;
	void DeserializeSelf(rapidjson::Value& reader);

	void SetColor(const vec3& color);
};

/******************************************************************************/
/*!
	[Component] - Visual Effects (VFX)
 */
 /******************************************************************************/
struct VFX : public Serializable
{
	vec3		mBloomThreshold{ 0.2126, 0.7152, 0.0722 };
	bool		isObjectBloom{ 1 };

	void Inspect();

	void SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const;
	void DeserializeSelf(rapidjson::Value& reader);

	void EnableObjectBloom() { isObjectBloom = true; }
	void DisableObjectBloom() { isObjectBloom = false; }
	void SetEntityBloomThreshold(glm::vec3 threshold) { mBloomThreshold = threshold; }
};

/******************************************************************************/
/*!
	[Component] - AIData
 */
/******************************************************************************/
enum class E_MOVEMENT_TYPE : char;
struct AISetting : public Serializable {
	E_MOVEMENT_TYPE mMovementType;	// AI's movement type
	float mSpreadOut;								// Percentage determining how much it changes the direction. Max 100
	float mStayAway;								// For flying enemy, horizontal distance to stay away from player
	float mElevation;								// For flying enemy, vertical distance to stay away from player
	std::string mTargetName;				// Name of target (Will be searching via gamestate, not scene)

	void Inspect();
	Entity GetTarget() { return mTarget; }
	Entity GetTarget() const { return mTarget; }
	void SetTarget(Entity _e) { mTarget = _e; }

	void SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const;
	void DeserializeSelf(rapidjson::Value& reader);

private: 
	Entity mTarget;									// AI's target
};

/******************************************************************************/
/*!
	[Component] - Button
 */
 /******************************************************************************/
struct Button
{
	bool mInteractable{ true };
	bool mIsHover{ false };
	bool mIsClick{ false };
	bool mActivated{ false };
	bool mRenderFlag{ true };

	inline bool IsInteractable() { return mInteractable; }
	inline bool IsHovered() { return mIsHover; }
	inline bool IsClicked() { return mIsClick; }
	inline bool IsActivated() { return mActivated; }
};

struct Crosshair : public Serializable
{
	float mThickness{ 1 };
	float mInner	{ 10 };
	float mOuter	{ 20 };

	vec4 mColor		{ 1.f, 1.f, 1.f, 1.f };

	void SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const;
	void DeserializeSelf(rapidjson::Value& reader);
};