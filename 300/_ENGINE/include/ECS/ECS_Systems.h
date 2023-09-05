#pragma once
// systems include
#include "Physics/PhysicsSystem.h"
#include "ScriptingSystem.h"

class SystemManager
{
	//PhysicsSystem mPhysicsSystem;
	//ScriptingSystem mScriptingSystem;
public:
	void Init();
	void Update(float dt);
	void Exit();
};