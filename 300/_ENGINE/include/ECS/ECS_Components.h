#pragma once
#include "glm/glm.hpp"
#include "Script.h"
#include <vector>
#include "Physics/PhysicsTypes.h"
#include "Tags.h"


//enum class TAG : unsigned char
//{
//	PLAYER,
//	UNKNOWN
//	// BATTLEGROUND
//	//more to come
//};
//
//enum class SUBTAG : unsigned char
//{
//	ACTIVE,
//	BACKGROUND
//	//more to come
//};

// every time tags/subtags get updated, please update the maps here
//static std::unordered_map<std::string, TAG> TagMap = { {"Player", TAG::PLAYER},
//													   {"Unknown", TAG::UNKNOWN} };
//
//static std::unordered_map<std::string, SUBTAG> SubTagMap = { {"Active", SUBTAG::ACTIVE},
//															 {"Background", SUBTAG::BACKGROUND} };

struct General
{
	/*Entity parent;*/
	std::string name;
	TAG tag;
	SUBTAG subtag;
	bool isActive;

	/*General() : parent(0) {};*/
};

struct Transform
{
	glm::vec3 mScale;
	glm::vec3 mRotate;
	glm::vec3 mTranslate;

	Transform() : mScale(100.f), mRotate(0.f), mTranslate(0.f) {}
	glm::quat GetQuaternion() { return glm::quat(mRotate); }
	void Inspect();
};

// this struct stores the filepaths for the meshdata, material, and shader. the actual data is stored in the resource manager
struct MeshRenderer
{
	// For now, we store the string to the filepaths. TO CHANGE to uids for efficient referencing
	std::pair<std::string, std::string> mShaderPath;
	std::string							mMaterialInstancePath;
	std::string							mMeshPath;
	
	unsigned							mGUID;
};

struct RigidBody
{
	std::uint16_t mMass;
	MATERIAL mMaterial;
	MOTION mMotion;

	RigidBody() : mMass(10.f), mMaterial(MATERIAL::WOOD), mMotion(MOTION::STATIC) {};
};

struct BoxCollider
{
	glm::vec3 mScaleOffset;			// final scale = mScaleOffset * Transform.mScale;
	glm::vec3 mTranslateOffset;		// final pos = Transform.mTranslate + mTranslateOffset;

	BoxCollider() : mScaleOffset(1.f), mTranslateOffset(0.f) {}
};

struct SphereCollider
{
	float mScaleOffset;				// final scale = mScaleOffset * std::max(Transform.mScale.x, Transform.mScale.y, Transform.mScale.z);
	glm::vec3 mTranslateOffset;		// final pos = Transform.mTranslate + mTranslateOffset;

	SphereCollider() : mScaleOffset(1.f), mTranslateOffset(0.f) {};
};

struct PlaneCollider
{
	glm::vec3 mNormal;				// direction of plane
	float mTranslateOffset;			// final pos = magnitude(Transform.mTranslate) + mTranslateOffset;

	PlaneCollider() : mNormal(0.f, 1.f, 0.f), mTranslateOffset(0.f) {};
};

struct SoundInfo
{
	unsigned int unique_ID;
	std::string  file_path;
	std::string	 sound_name;
	bool		 is3D;
	bool		 isLooping; 
};

/***************************************************************************/
/*!
\brief
Script component that entities who need to have script will possess.
*/
/***************************************************************************/
class Scripts {
public:
	Scripts() = default;
	~Scripts() = default;
	void Inspect();

	static void AddScript(Entity id, std::string fileName);
	//static void LoadRunScript(Entity entity);

	std::string mScriptFile{};
	std::vector <Script> scriptsContainer;
};
