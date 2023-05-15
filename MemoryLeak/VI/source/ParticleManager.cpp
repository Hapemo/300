/*!
file:	ParticleSystem.cpp
author:	Jazz Teoh Yu Jue

email:	j.teoh@digipen.edu

brief:	Particle System class. This is a singleton where it keeps track of all the particle
		operation in the game.

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include "ParticleManager.h"
#include "Camera.h"
#include "pch.h"
#include "Helper.h"
#include "Utilities.h"
#include "ECS_tools.h"
//#include "GameObject.h"
#include "SpriteVariable.h"


uint64_t ParticleManager::mParticleCount{0};
bool		 ParticleManager::mParticleChange{};

//==============================
// Particle 
//==============================
Particle::Particle() : mTransform(), mVelocity(), mColor(), mLifespan(), mMaxLifespan(), mEntity() {}

void Particle::Init(Vec2 _velocity, Transform _transform, float _lifespan, EntityID _e) {
	mVelocity = _velocity;
	mTransform = _transform;
	mColor = ECS::GetComponent<ParticleSystem>(_e).mParticleInfo.mSprite.color;
	mMaxLifespan = mLifespan = _lifespan;
	mEntity = _e;
	ParticleManager::ParticleChanged();
}

void Particle::Update() {
	if (mLifespan == 0) {
		LOG_WARN("Should not be updating inactive particle");
		return;
	} else if ((mLifespan -= static_cast<float>(FPSManager::dt)) < 0) { // Update Lifespan
		Destroy();
		return;
	}

	ParticleSystem const& system{ ECS::GetComponent<ParticleSystem>(mEntity) };

	// Update position
	mTransform.translation += static_cast<float>(FPSManager::dt) * mVelocity;

	// Update scale
	if (system.mParticleInfo.mExpanding) {
		bool xMoreThan0{ mTransform.scale.x > 0 };
		bool yMoreThan0{ mTransform.scale.y > 0 };
		float yExpand = mTransform.scale.y / mTransform.scale.x;
		mTransform.scale.x += system.mParticleInfo.mExpanding * static_cast<float>(FPSManager::dt);
		mTransform.scale.y += system.mParticleInfo.mExpanding * yExpand * static_cast<float>(FPSManager::dt);

		// If shrink till 0 size in either axis, destroy
		if ((xMoreThan0 && (mTransform.scale.x < 0)) || yMoreThan0 && (mTransform.scale.y < 0)) {
			mLifespan = 0;
			Destroy();
		}
	}

	// Update rotation
	if (system.mParticleInfo.mRotation)
		mTransform.rotation += system.mParticleInfo.mRotation * static_cast<float>(FPSManager::dt);

	// Update Color/Alpha
	if (system.mParticleInfo.mFading && system.mParticleInfo.mFadeIn) {
		float ratio = abs(mLifespan/mMaxLifespan - 0.5f);
		ratio = abs(ratio - 0.5f);
		mColor.a = static_cast<GLubyte>(ratio * system.OriginalAlpha());
	} else if (system.mParticleInfo.mFading) {
		mColor.a = static_cast<GLubyte>((mLifespan / mMaxLifespan) * system.OriginalAlpha());
	} else if (system.mParticleInfo.mFadeIn) {
		mColor.a = static_cast<GLubyte>((1 - (mLifespan / mMaxLifespan)) * system.OriginalAlpha());
	}

}

void Particle::Destroy() {
	memset(this, 0, sizeof(Particle));
	ParticleManager::DecreaseParticleCount();
	ParticleManager::ParticleChanged();
}

//==============================
// Particle Manager
//==============================
ParticleManager::ParticleManager() : mParticles() { srand(static_cast<unsigned int>(time(0))); LOG_CREATE("PARTICLESYSTEM"); }

void ParticleManager::UpdateSystems() {
	for (auto& e : mEntities) {
		if (!e.ShouldRun()) continue;
		ParticleSystem& system{ e.GetComponent<ParticleSystem>() };
		// Update particle system active state
		if (!system.mIsActive) continue;
		system.mDuration -= static_cast<float>(FPSManager::dt);
		if (system.mDuration < 0) {
			system.mDuration = 0;
			system.mIsActive = false;
		}//if (!system.mIsActive) continue;

		// Track slow spawning
		if (system.mSlow) {
			float& tracker{ system.SlowTracker() };
			tracker += static_cast<float>(FPSManager::dt);
			if (tracker > system.mSlow)	tracker = 0;
			else continue;
		}
		
		GenerateParticle(system, e.id);
	}
}

void ParticleManager::UpdateParticles() {
	//std::cout << "particle count: " << mParticleCount << '\n';
	for (int i{}; i < mParticleCount; ++i)
		mParticles[i].Update();

}

void ParticleManager::Update() {
	mParticleChange = false;
	UpdateSystems();
	UpdateParticles();

	if (mParticleChange)
		std::sort( mParticles.begin(), mParticles.end(), [] ( Particle p1, Particle p2 ) { return p1.GetEntityID() > p2.GetEntityID(); } );
}

void ParticleManager::Reset() {
	LOG_CUSTOM("PARTICLESYSTEM", "Reset Particle Manager of size " + std::to_string(sizeof(ParticleManager)));
	memset(&mParticles, 0, mParticles.size() * sizeof(Particle));
	mParticleChange = false;
	mParticleCount = 0;
}

void ParticleManager::GenerateParticle(ParticleSystem const& _system, EntityID _e) {
	for (int i{}; i < _system.mDensity; ++i) {
		Particle& currP = mParticles[mParticleCount++];

		//Initialise the particle and give it a random position and direction to move from
		Transform trans{};
		trans.rotation = _system.mParticleInfo.mFacing;
		trans.scale *= _system.mParticleInfo.mScale;
		trans.translation = Entity(_e).GetComponent<Transform>().translation +
												_system.mCenter + Vec2(static_cast<float>(Util::RandInt(static_cast<int>(_system.mAreaWidth) / 2, -static_cast<int>(_system.mAreaWidth) / 2)),
																							 static_cast<float>(Util::RandInt(static_cast<int>(_system.mAreaWidth) / 2, -static_cast<int>(_system.mAreaWidth) / 2)));


		float angle = Util::RandInt(static_cast<int>(_system.mSpread) * 10, static_cast<int>(-_system.mSpread) * 10) / 10.f + _system.mDirection;

		angle *= (static_cast<float>(Math::PI)/180.f);
		Vec2 vel{};
		if (_system.mDirection) // Only move particle if it needs to move
			vel = Vec2{ cosf(angle), sinf(angle) }.Normalized() * _system.mParticleInfo.mSpeed;

		currP.Init(std::move(vel), trans, _system.mParticleInfo.mLifespan, _e);
	}

}


//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
