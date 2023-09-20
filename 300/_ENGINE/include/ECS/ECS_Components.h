#pragma once
#include "glm/glm.hpp"
#include "Script.h"
#include <vector>
#include "Physics/PhysicsTypes.h"
#include "Tags.h"
#include "rttr/registration.h"
#include "ECS.h"
#include "Audio/AudioType.h"

struct General
{
	std::string name;
	TAG tag;
	SUBTAG subtag;
	bool isActive;
	bool isPaused;

	General() 
	: name(""), tag(TAG::UNKNOWN), subtag(SUBTAG::ACTIVE), isActive(true) 
	{};

	RTTR_ENABLE()
};

struct Transform
{
	glm::vec3 mScale;
	glm::vec3 mRotate;
	glm::vec3 mTranslate;

	Transform() : mScale(100.f), mRotate(0.f), mTranslate(0.f) {}
	glm::quat GetQuaternion() { return glm::quat(mRotate); }
	void Inspect();

	RTTR_ENABLE()
};

// this struct stores the filepaths for the meshdata, material, and shader. the actual data is stored in the resource manager
struct MeshRenderer
{
	// For now, we store the string to the filepaths. TO CHANGE to uids for efficient referencing
	std::pair<std::string, std::string> mShaderPath;
	std::string							mMaterialInstancePath;
	std::string							mMeshPath;
	
	unsigned							mGUID;

	RTTR_ENABLE()
};

struct RigidBody
{
	std::uint16_t mMass;
	MATERIAL mMaterial;
	MOTION mMotion;

	RigidBody() : mMass(10.f), mMaterial(MATERIAL::WOOD), mMotion(MOTION::STATIC) {};
	RTTR_ENABLE()
};

struct BoxCollider
{
	glm::vec3 mScaleOffset;			// final scale = mScaleOffset * Transform.mScale;
	glm::vec3 mTranslateOffset;		// final pos = Transform.mTranslate + mTranslateOffset;

	BoxCollider() : mScaleOffset(1.f), mTranslateOffset(0.f) {}
	
	RTTR_ENABLE()
};

struct SphereCollider
{
	float mScaleOffset;				// final scale = mScaleOffset * std::max(Transform.mScale.x, Transform.mScale.y, Transform.mScale.z);
	glm::vec3 mTranslateOffset;		// final pos = Transform.mTranslate + mTranslateOffset;

	SphereCollider() : mScaleOffset(1.f), mTranslateOffset(0.f) {};

	RTTR_ENABLE()
};

struct PlaneCollider
{
	glm::vec3 mNormal;				// direction of plane
	float mTranslateOffset;			// final pos = magnitude(Transform.mTranslate) + mTranslateOffset;

	PlaneCollider() : mNormal(0.f, 1.f, 0.f), mTranslateOffset(0.f) {};

	RTTR_ENABLE()
};

class Scripts {
public:
	Scripts() = default;
	~Scripts() = default;
	void Inspect();

	static void AddScript(Entity id, std::string fileName);
	//static void LoadRunScript(Entity entity);

	std::string mScriptFile{};
	std::vector <Script> scriptsContainer;

	RTTR_ENABLE()
};

struct Parent
{
	std::uint32_t mPrevSibling;
	std::uint32_t mNextSibling;
	std::uint32_t mParent;

	Parent() : mPrevSibling(0), mNextSibling(0), mParent(0) {};

	RTTR_ENABLE()
};

struct Children
{
	std::uint32_t mNumChildren;	
	std::uint32_t mFirstChild;

	Children() : mNumChildren(0), mFirstChild(0) {};

	RTTR_ENABLE()
};

struct Audio
{
	std::string mFileName;
	AUDIOTYPE mAudioType;
	bool mIsPlaying;// check if audio is already playing
	bool mIsPlay;	// play audio if true

	RTTR_ENABLE()
};

RTTR_REGISTRATION
{
	rttr::registration::class_<Entity>("Entity")
	.property("EntityID", &Entity::id)
	;

	rttr::registration::class_<General>("General")
	.property("Name", &General::name)
	.property("Tag", &General::tag)
	.property("Subtag", &General::subtag)
	.property("Active", &General::isActive)
	.property("Paused", &General::isPaused)
	;

	rttr::registration::enumeration<TAG>("Tag")
	(
		rttr::value("Player", TAG::PLAYER),
		rttr::value("Unknown", TAG::UNKNOWN)
	);

	rttr::registration::enumeration<SUBTAG>("Subtag")
	(
		rttr::value("Active", SUBTAG::ACTIVE),
		rttr::value("Background", SUBTAG::BACKGROUND)
	);

	rttr::registration::class_<Transform>("Transform")
	.property("Scale", &Transform::mScale)
	.property("Rotate", &Transform::mRotate)
	.property("Translate", &Transform::mTranslate)
	;

	rttr::registration::class_<RigidBody>("RigidBody")
	.property("Mass", &RigidBody::mMass)
	.property("Material", &RigidBody::mMaterial)
	.property("Motion", &RigidBody::mMotion)
	;

	/*rttr::registration::class_<MeshRenderer>("MeshRenderer")
	.property("ShaderPath", &MeshRenderer::mShaderPath)
	.property("MaterialInstancePath", &MeshRenderer::mMaterialInstancePath)
	.property("MeshPath", &MeshRenderer::mMeshPath)
	.property("GUID", &MeshRenderer::mGUID)
	;*/

	rttr::registration::class_<BoxCollider>("BoxCollider")
	.property("ScaleOffset", &BoxCollider::mScaleOffset)
	.property("TranslateOffset", &BoxCollider::mTranslateOffset)
	;

	rttr::registration::class_<SphereCollider>("SphereCollider")
	.property("ScaleOffset", &SphereCollider::mScaleOffset)
	.property("TranslateOffset", &SphereCollider::mTranslateOffset)
	;

	rttr::registration::class_<PlaneCollider>("PlaneCollider")
	.property("Normal", &PlaneCollider::mNormal)
	.property("TranslateOffset", &PlaneCollider::mTranslateOffset)
	;

	rttr::registration::class_<Scripts>("Scripts")
	.property("ScriptsFiles", &Scripts::scriptsContainer)
	;

	rttr::registration::class_<Parent>("Parent")
	.property("Parent", &Parent::mParent)
	.property("PreviousSibling", &Parent::mPrevSibling)
	.property("NextSibling", &Parent::mNextSibling)
	;

	rttr::registration::class_<Children>("Children")
	.property("NumChildren", &Children::mNumChildren)
	.property("FirstChild", &Children::mFirstChild)
	;

	rttr::registration::class_<Audio>("Audio")
	.property("Filename", &Audio::mFileName)
	.property("AudioType", &Audio::mAudioType)
	.property("Playing", &Audio::mIsPlaying)
	.property("Play", &Audio::mIsPlay)
	;

	rttr::registration::class_<glm::vec3>("vec3")
	.property("x", &glm::vec3::x)
	.property("y", &glm::vec3::y)
	.property("z", &glm::vec3::z)
	;

	rttr::registration::enumeration<MATERIAL>("Material")
	(
		rttr::value("Rubber", MATERIAL::RUBBER),
		rttr::value("Wood", MATERIAL::WOOD),
		rttr::value("Metal", MATERIAL::METAL),
		rttr::value("Ice", MATERIAL::ICE),
		rttr::value("Concrete", MATERIAL::CONCRETE),
		rttr::value("Glass", MATERIAL::GLASS)
	);

	rttr::registration::enumeration<MOTION>("Motion")
	(
		rttr::value("Static", MOTION::STATIC),
		rttr::value("Dynamic", MOTION::DYNAMIC)
	);

	rttr::registration::enumeration<AUDIOTYPE>("AudioType")
	(
		rttr::value("BGM", AUDIOTYPE::AUDIO_BGM),
		rttr::value("SFX", AUDIOTYPE::AUDIO_SFX)
	);

	/*rttr::registration::class_<Script>("Script")
	.property("Filename", &Script::scriptFile)
	;*/
}

