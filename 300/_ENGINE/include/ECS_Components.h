#pragma once
#include "glm/glm.hpp"

struct Transform
{
	glm::vec3 mScale;
	glm::vec3 mRotate;
	glm::vec3 mTranslate;
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