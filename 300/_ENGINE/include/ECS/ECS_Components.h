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

//#include "rttr/registration.h"

struct General
{
	std::string name;
	std::string tag[5] = { "PLAYER","ENEMY","BULLET","STATIC","BUILDING" };
	int tagid{ 0 };
	SUBTAG subtag;
	bool isActive;
	bool isPaused;

	General() 
	: name(""), subtag(SUBTAG::ACTIVE), isActive(true) 
	{};

	void								Inspect();

	//RTTR_ENABLE()
};

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

struct Animator
{
	GFX::Animator	mAnimator;

	void Inspect();
};

// this struct stores the filepaths for the meshdata, material, and shader. the actual data is stored in the resource manager
struct MeshRenderer
{
	// For now, we store the string to the filepaths. TO CHANGE to uids for efficient referencing
	std::pair<std::string, std::string> mShaderPath;
	std::string							mMaterialInstancePath[4];
	std::string							mMeshPath;
	
	void*								mMeshRef;
	void*								mTextureRef[4];
	bool								mTextureCont[4];

	unsigned							mGUID;

	void								Inspect();
	//RTTR_ENABLE()


	//void								Inspect();
};

struct RigidBody
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
};

struct BoxCollider
{
	glm::vec3 mScaleOffset;			// final scale = mScaleOffset * Transform.mScale;
	glm::vec3 mTranslateOffset;		// final pos = Transform.mTranslate + mTranslateOffset;
	

	BoxCollider() : mScaleOffset(1.f), mTranslateOffset(0.f) {}
	
	//RTTR_ENABLE()
	void							Inspect();
};

struct SphereCollider
{
	float mScaleOffset;				// final scale = mScaleOffset * std::max(Transform.mScale.x, Transform.mScale.y, Transform.mScale.z);
	glm::vec3 mTranslateOffset;		// final pos = Transform.mTranslate + mTranslateOffset;

	SphereCollider() : mScaleOffset(1.f), mTranslateOffset(0.f) {};

	//RTTR_ENABLE()
	void							Inspect();
};

struct PlaneCollider //if has plane collider always static
{
	glm::vec3 mNormal;				// direction of plane
	float mTranslateOffset;			// final pos = magnitude(Transform.mTranslate) + mTranslateOffset;

	PlaneCollider() : mNormal(0.f, 1.f, 0.f), mTranslateOffset(0.f) {};

	//RTTR_ENABLE()
	void							Inspect();
};

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

struct Parent
{
	std::uint32_t mPrevSibling;
	std::uint32_t mNextSibling;
	std::uint32_t mParent;

	Parent() : mPrevSibling(0), mNextSibling(0), mParent(0) {};

	//RTTR_ENABLE()
};

struct Children
{
	std::uint32_t mNumChildren;	
	std::uint32_t mFirstChild;

	Children() : mNumChildren(0), mFirstChild(0) {};

	//RTTR_ENABLE()
};

struct Audio
{	
	std::string mFilePath;				// File Path to the Audio File
	std::string mFileName;				// Name of Audio file
	//std::string mFullPath;				// 
	AUDIOTYPE mAudioType;				// SFX or BGM?
	bool mIsPlaying;					// check if audio is already playing
	bool mIsPlay;						// play audio if true

	// Don't need to serialize ...
	std::vector<int> mPlaySFXChannelID;    // Currently playing in SFX Channel...
	std::vector<int> mPlayBGMChannelID;	   // Currently playing in BGM Channel ...

	Audio() : mFilePath("../assets/Audio"), mFileName("Sample Name"), mAudioType(AUDIO_NULL), mIsPlaying(false), mIsPlay(false) {}

	Audio(std::string file_path_to_audio, std::string file_audio_name, AUDIOTYPE audio_type, bool isPlay) : mAudioType(audio_type) , mIsPlaying(false) , mIsPlay(isPlay)
	{	
		mFilePath = file_path_to_audio;
		mFileName = file_audio_name;
	}
	//RTTR_ENABLE()
	void							Inspect();
};


struct Camera 
{
	GFX::Camera						mCamera;

	void							Inspect();
};

struct Prefab
{
	std::string mPrefab;
};

struct PointLight
{
	vec3	mLightColor{ 1.f, 1.f, 1.f };
	float	mAttenuation;
	float	mIntensity;
	void Inspect();
};