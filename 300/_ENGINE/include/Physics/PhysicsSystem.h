#pragma once
#include "PhysX.h"
#include "Materials.h"
#include "pch.h"

class PhysicsSystem
{
	PhysX mPhysX;
	std::unordered_map<MATERIAL, physx::PxMaterial*> mMaterialList;

public:
	PhysicsSystem();
	void Init();
	void Update(float dt);
	void Exit();

private:
	void CreateMaterial(MATERIAL material, float us, float uk, float e);
};