
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
#include "Input/Input.h"
#include "Guid.h"
#include <Constants.h>

//#include "Graphics/GraphicsSystem.h"
//#include "Mesh.hpp"

//#include "rttr/registration.h"

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
	//uid									mShaders;
	//std::pair<std::string, std::string> mShaderPath{ "../assets/shader_files/animations_vert.glsl", "../assets/shader_files/pointLight_frag.glsl" };
	std::pair<std::string, std::string> mShaderPath{ "../assets/shader_files/pointLight_vert.glsl", "../assets/shader_files/pointLight_frag.glsl" };
	uid									mShaderid;
	
	std::string							mMaterialInstancePath[4] {" "," " ," " ," " };
	vec4								mInstanceColor{ 1.f, 1.f, 1.f, 1.f };

	std::string							mMeshPath;
	
	void*								mMeshRef{};
	void*								mTextureRef[4];

	unsigned							mGUID;
	bool								mTextureCont[4];

	void								Inspect();
	void								SetColor(const vec4& color);
	void								SetMesh(const std::string& meshName);
	void								SetTexture(MaterialType type, const std::string& Texturename);

	//RTTR_ENABLE()
};


struct UIrenderer
{
	std::string							mTexPath; // temporary should be UID
	void* mTextureRef;
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

	RigidBody() : mDensity(10.f), mMaterial(MATERIAL::WOOD), mMotion(MOTION::STATIC), mVelocity(0.f){};
	RigidBody(float dense, MATERIAL mat, MOTION mot, const glm::vec3& vec)
		: mDensity(dense), mMaterial(mat), mMotion(mot), mVelocity(vec){}
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
	bool mIsTrigger;
	bool mIsTriggerCollide;
	std::string mTriggerCollidingWith;

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
	bool mIsTrigger;
	bool mIsTriggerCollide;
	std::string mTriggerCollidingWith;

	SphereCollider() : mScaleOffset(1.f), mTranslateOffset(0.f) {};

	//RTTR_ENABLE()
	void							Inspect();
};

struct CapsuleCollider
{
	glm::vec3 mTranslateOffset;
	float mRadius;
	float mHalfHeight;
	bool mIsTrigger;
	bool mIsTriggerCollide;
	std::string mTriggerCollidingWith;


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

	static void AddScript(Entity id, std::string fileName);
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



