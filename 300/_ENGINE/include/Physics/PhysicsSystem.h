#pragma once
#include "PhysX.h"
#include "pch.h"
#include "Material.h"
#include "Singleton.h"

struct PhysicsSystem : public Singleton<PhysicsSystem>
{
	PhysX mPX;
	std::unordered_map<MATERIAL, physx::PxMaterial*> mMaterials;

	PhysicsSystem() = default;
	void Init();
	void Update(float dt);
	void Exit();

private:
	physx::PxMaterial* CreateMaterial(float us, float ud, float res);
};