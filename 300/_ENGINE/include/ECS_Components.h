#pragma once
#include "glm/glm.hpp"
#include "../../lib/glm-0.9.9.8/glm/glm.hpp" 

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
	float y;
};

struct TestComponent3
{
	float z;
};