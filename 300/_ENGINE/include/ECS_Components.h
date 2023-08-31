#pragma once
#include "../glm-0.9.9.8/glm/glm.hpp"
#include "../../_SCRIPTING/Script.h"
#include <vector>

struct Transform
{
	glm::vec3 scale;
	glm::vec3 rotate;
	glm::vec3 translate;
};

struct TestComponent1
{
	float x;
};

struct TestComponent2
{
	float y ;
};

struct TestComponent3
{
	float z;
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
	static void LoadRunScript(Entity entity);

	std::string scriptFile{};
	std::vector <Script> scriptsContainer;
};
