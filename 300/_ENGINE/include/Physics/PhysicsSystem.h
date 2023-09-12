#pragma once
#include "PhysX.h"
#include "pch.h"
#include "Material.h"

class PhysicsSystem
{
public:
	PhysicsSystem() = default;
	void Init();
	void Update(float dt);
	void Exit();

private:
	PhysX mPX;
	std::unordered_map<MATERIAL, physx::PxMaterial*> mMaterials;
	physx::PxMaterial* CreateMaterial(float us, float ud, float res);
};