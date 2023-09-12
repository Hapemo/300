#pragma once
#include "glm/glm.hpp"
#include "Script.h"
#include <vector>
#include "pch.h"

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
															 {"Background", SUBTAG::BACKGROUND}};

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
	float data {0};

	void Inspect();
};

struct RigidBody
{
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
