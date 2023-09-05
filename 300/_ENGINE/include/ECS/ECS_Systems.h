#pragma once
// systems include
#include "Physics/PhysicsSystem.h"

class SystemManager
{
	//PhysicsSystem mPhysicsSystem;

public:
	void Init();
	void Update(float dt);
	void Exit();
};