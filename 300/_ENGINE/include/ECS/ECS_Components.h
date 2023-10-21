
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
#include "EnumStrings.h"
#include "Input/Input.h"
#include "Guid.h"
#include <Constants.h>
#include "ResourceManagerTy.h"

#include "Texture.hpp"
//#include "Graphics/GraphicsSystem.h"
//#include "Mesh.hpp"

//#include "rttr/registration.h"

DECLARE_ENUMSTRING(enum_tag, PLAYER, ENEMY, BULLET, STATIC, BUILDING)
struct uid;

//DECLARE_ENUMSTRING(enum_tag, PLAYER, ENEMY, BULLET, STATIC, BUILDING)
namespace GFX {
	struct Mesh;

}

/******************************************************************************/
/*!
	[Component] - General
 */
 /******************************************************************************/
struct General
{
	std::string name;
	/*TAG tag;*/
	//enum_tag::enum_tag tag{};
	enum_tag::enum_tag tagid{};
	//std::string tag[5] = { "PLAYER","ENEMY","BULLET","STATIC","BUILDING" };
	//int tagid{ 0 };
	SUBTAG subtag;
	bool isActive{};
	bool isPaused{};

	General() 
	: name(""), subtag(SUBTAG::ACTIVE), isActive(true) 
	{};

	std::string GetTag() { return enum_tag::ToString(tagid); }

	void SetTag(std::string newTag) { 
		std::transform(newTag.begin(), newTag.end(), newTag.begin(), ::tolower);
		newTag[0] = std::toupper(newTag[0]);
		tagid = enum_tag::GetEnum(newTag.c_str()); 
	}

	void Inspect();

	//RTTR_ENABLE()
};

/******************************************************************************/
/*!
	[Component] - Transform
 */
 /******************************************************************************/
struct Transform
{
	glm::vec3 mScale;
	glm::vec3 mRotate;
	glm::vec3 mTranslate;

	Transform() : mScale(1.f), mRotate(0.f), mTranslate(0.f) {}
	glm::quat GetQuaternion() { return glm::quat(mRotate); }
	void Inspect();

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
	[Component] - MeshRenderer
 */
 /******************************************************************************/
// this struct stores the filepaths for the meshdata, material, and shader. the actual data is stored in the resource manager
struct MeshRenderer
{
	// For now, we store the string to the filepaths. TO CHANGE to uids for efficient referencing
	//std::pair<std::string, std::string> mShaderPath{ "../assets/shader_files/animations_vert.glsl", "../assets/shader_files/pointLight_frag.glsl" };
	std::pair<std::string, std::string> mShaderPath{ "../assets/shader_files/pointLight_vert.glsl", "../assets/shader_files/pointLight_frag.glsl" };
	
	//uid									mShaderid;
	ref									mShaderRef;

	std::string							mMaterialInstancePath[4] {" "," " ," " ," " };
	vec4								mInstanceColor{ 1.f, 1.f, 1.f, 1.f };

	std::string							mMeshPath;
	

	ref									mMeshRef{};
	//void*								mMeshRef{};

	//ref									mTextureRef[5];
	//void*								mTextureRef[4];

	//unsigned							mGUID;
	//bool								mTextureCont[4];
	//void*								mMeshRef{nullptr};
	ref									mTextureRef[5]		{ {nullptr,0},{nullptr,0},{nullptr,0},{nullptr,0},{nullptr,0} };

	unsigned							mGUID;
	bool								mTextureCont[5];

	void								Inspect();
	void								SetColor(const vec4& color);
	void								SetMesh(const std::string& meshName);
	void								SetTexture(MaterialType type, const std::string& Texturename);

	
	int									GetTexture(MaterialType type) 
										{ 
											if (mTextureRef[static_cast<int>(type)].data == nullptr)
												return -1;

											return static_cast<GFX::Texture*>(mTextureRef[static_cast<int>(type)].data)->ID();
										}
	//RTTR_ENABLE()
};


struct UIrenderer
{
	std::string							mTexPath; // temporary should be UID
	void*								mTextureRef;

	inline unsigned ID() 
	{
		if (mTextureRef != nullptr) {
			int temp = (reinterpret_cast<GFX::Texture*>(mTextureRef))->ID();
			return temp;
		}

		return 0;
	}
	void Inspect();
};



/******************************************************************************/
/*!
	[Component] - RigidBody
 */
 /******************************************************************************/
struct RigidBody
{
	float mDensity;
	MATERIAL mMaterial;
	MOTION mMotion;
	glm::vec3 mVelocity;
	bool mIsTrigger;

	RigidBody() : mDensity(10.f), mMaterial(MATERIAL::WOOD), mMotion(MOTION::STATIC), mVelocity(0.f), mIsTrigger(false) {};
	RigidBody(float dense, MATERIAL mat, MOTION mot, const glm::vec3& vec, bool isTrigger)
		: mDensity(dense), mMaterial(mat), mMotion(mot), mVelocity(vec), mIsTrigger(isTrigger) {}
	//RTTR_ENABLE()


	int mMat{ 0 };
	int mMot{ 0 };
	void							Inspect();
};

/******************************************************************************/
/*!
	[Component] - BoxCollider
 */
 /******************************************************************************/
struct BoxCollider
{
	glm::vec3 mScaleOffset;			// final scale = mScaleOffset * Transform.mScale;
	glm::vec3 mTranslateOffset;		// final pos = Transform.mTranslate + mTranslateOffset;
	

	BoxCollider() : mScaleOffset(1.f), mTranslateOffset(0.f) {}
	
	//RTTR_ENABLE()
	void							Inspect();
};

/******************************************************************************/
/*!
	[Component] - SphereCollider
 */
 /******************************************************************************/
struct SphereCollider
{
	float mScaleOffset;				// final scale = mScaleOffset * std::max(Transform.mScale.x, Transform.mScale.y, Transform.mScale.z);
	glm::vec3 mTranslateOffset;		// final pos = Transform.mTranslate + mTranslateOffset;

	SphereCollider() : mScaleOffset(1.f), mTranslateOffset(0.f) {};

	//RTTR_ENABLE()
	void							Inspect();
};

/******************************************************************************/
/*!
	[Component] - PlaneCollider
 */
 /******************************************************************************/
struct PlaneCollider //if has plane collider always static
{
	glm::vec3 mNormal;				// direction of plane
	float mTranslateOffset;			// final pos = magnitude(Transform.mTranslate) + mTranslateOffset;

	PlaneCollider() : mNormal(0.f, 1.f, 0.f), mTranslateOffset(0.f) {};

	//RTTR_ENABLE()
	void							Inspect();
};

struct AABBCollider
{
	glm::vec3 mScaleOffset;			// final scale = mScaleOffset * Transform.mScale;
	glm::vec3 mTranslateOffset;		// final pos = Transform.mTranslate + mTranslateOffset;

	AABBCollider() : mScaleOffset(1.f), mTranslateOffset(0.f) {}
};

struct CapsuleCollider
{
	glm::vec3 mTranslateOffset;
	float mRadius;
	float mHalfHeight;
	CapsuleCollider() : mTranslateOffset(0.f, 0.f, 0.f), mRadius(50.f), mHalfHeight(100.f) {}
};

/******************************************************************************/
/*!
	[Component] - Scripts
 */
 /******************************************************************************/
class Scripts {
public:
	Scripts() = default;
	~Scripts() = default;

	//static void LoadRunScript(Entity entity);

	std::string mScriptFile{};
	std::vector <Script> scriptsContainer;

	//RTTR_ENABLE()
	void Inspect();

};

/******************************************************************************/
/*!
	[Component] - Parent
 */
 /******************************************************************************/
struct Parent
{
	std::uint32_t mPrevSibling;
	std::uint32_t mNextSibling;
	std::uint32_t mParent;

	Parent() : mPrevSibling(0), mNextSibling(0), mParent(0) {};

	//RTTR_ENABLE()
};

/******************************************************************************/
/*!
	[Component] - Children
 */
 /******************************************************************************/
struct Children
{
	std::uint32_t mNumChildren;	
	std::uint32_t mFirstChild;

	Children() : mNumChildren(0), mFirstChild(0) {};

	//RTTR_ENABLE()
};


/******************************************************************************/
/*!
	[Component] - Audio
 */
 /******************************************************************************/

struct Audio
{
	// Serialize
	// -----------------------------------------
	std::string mFilePath;				   // File Path to the Audio File (required for loading)
	std::string mFileName;				   // Name of Audio file (required for loading)
	std::string mFullPath;				   // Full Path (File Path + Audio File)

	bool mPlayonAwake = false;		       // [Flag] - flag to play as the scene launches. 
	bool mIsLooping = false;			   // [Flag] - flag to decide whether if audio is looping.
	
	// Audio Type [Channel Management]
	AUDIOTYPE      mAudioType;			   // SFX or BGM (Mute Channels)

	// Volume 
	float		   mVolume = 1.0f;


	// Do not serialize 
	// ------------------------------------------
	// Update Loop - Boolean Checks
	bool		   mIsPlaying = false;		      // [Flag] - Check if audio is already playing (Channel Interaction)
	bool           mIsPlay = false;			       // [Flag] - to decide whether to play audio (if true)
	bool		   mFadeIn = false;				   // [Flag] - This audio will be faded out. 
	bool		   mFadeOut = false;			   // [Flag] - This audio will be faded in.

	// For Editor
	bool		   mIsEmpty = true;	       // [For Editor] - if empty delete all data in this <Audio> component
	bool		   mIsLoaded = false;	   // [For Loading]

	// Pause State [Editor/Pause Menu]
	bool		   mSetPause = false;      // [Flag] - set pause for channel.
	bool		   mPaused = false;        // [For Unpausing]
	bool		   mSetUnpause = false;    // [Flag] - for unpausing channels.
	float		   mTypeChanged = false;   // [For Editor] - trigger type change

	// Q. Can a <Audio> entity have their very own channel.
	uid            mChannelID;             // Channel ID (Channel Management)
	FMOD::Channel* mChannel;         	   // Use this to facilitate manipulation of audio.
	FMOD::Sound* mSound;				   // Each <Audio> can only hold a reference to the "Audio File" it's attached to.

	// Fade Volume Stuff
	float fade_timer = 0.0f;			   // How long the fade has elapsed
	float fade_duration = 5.0f;			   // How long to fade...

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
		mSound = nullptr;
	}

	int mAudio{ 0 };

	//RTTR_ENABLE()
	void							Inspect();
};



/******************************************************************************/
/*!
	[Component] - Camera
 */
 /******************************************************************************/
struct Camera 
{
	GFX::Camera						mCamera;

	void							Inspect();
};

/******************************************************************************/
/*!
	[Component] - Prefab
 */
 /******************************************************************************/
struct Prefab
{
	std::string mPrefab;
};

/******************************************************************************/
/*!
	[Component] - PointLight
 */
 /******************************************************************************/
struct PointLight
{
	vec3	mLightColor{ 1.f, 1.f, 1.f };
	float	mLinearFalloff{};
	float	mQuadraticFalloff{};
	float	mIntensity{};

	void Inspect();
	void SetColor(const vec3& color);
};

/******************************************************************************/
/*!
	[Component] - Visual Effects (VFX)
 */
 /******************************************************************************/
struct VFX
{
	vec3		mBloomThreshold{ 0.2126, 0.7152, 0.0722 };

	void Inspect();
};


// Added [9/27]
// Pseudo-Component (Helps InputActionMapEditor)

/******************************************************************************/
/*!
	[Component] - AIData
 */
/******************************************************************************/
enum class E_MOVEMENT_TYPE : char;
enum class E_ATTACK_TYPE : char;
struct AISetting {
	E_MOVEMENT_TYPE mMovementType;	// AI's movement type
	E_ATTACK_TYPE mAttackType;			// AI's attack type
	float mSpreadOut;								// Degree of spreading out
	std::uint32_t mTarget;									// AI's target
};


