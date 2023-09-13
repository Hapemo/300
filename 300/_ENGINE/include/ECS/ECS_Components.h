#pragma once
#include "glm/glm.hpp"
#include "Script.h"
#include <vector>
#include "pch.h"
#include "Physics/PhysicsTypes.h"

enum class TAG : unsigned char
{
	PLAYER,
	UNKNOWN
	//more to come
};

enum class SUBTAG : unsigned char
{
	ACTIVE,
	BACKGROUND
	//more to come
};

// every time tags/subtags get updated, please update the maps here
static std::unordered_map<std::string, TAG> TagMap = { {"Player", TAG::PLAYER},
													   {"Unknown", TAG::UNKNOWN} };

static std::unordered_map<std::string, SUBTAG> SubTagMap = { {"Active", SUBTAG::ACTIVE},
															 {"Background", SUBTAG::BACKGROUND} };

struct General
{
	std::string name;
	TAG tag;
	SUBTAG subtag;
	bool isActive;
};

struct Transform
{
	glm::vec3 mScale;
	glm::vec3 mRotate;
	glm::vec3 mTranslate;

	Transform() : mScale(1.f), mRotate(0.f), mTranslate(0.f) {}
	glm::quat GetQuaternion() { return glm::quat(mRotate); }
	void Inspect();
};

struct RigidBody
{
	std::uint16_t mass;
	MATERIAL mMaterial;
	MOTION mMotion;
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

	std::string scriptFile{};
	std::vector <Script> scriptsContainer;
};
