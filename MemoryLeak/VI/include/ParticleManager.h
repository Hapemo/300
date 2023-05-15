/*!
file:	ParticleSystem.h
author:	Jazz Teoh Yu Jue

email:	j.teoh@digipen.edu

brief:	Particle System class. This is a singleton where it keeps track of all the particle
		operation in the game.

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#pragma once

#include "Singleton.h"
#include "ECS_items.h"
#include "ECS_components.h"
//#include "GameObject.h"
#define MAX_PARTICLES 100'000

#define YELLOW Color(255, 255, 0, 255)
#define BLUE Color (0, 0, 255, 255)
#define GREEN Color (0, 255, 0, 255)
#define RED Color (255, 0, 0, 255)
#define WHITE Color (255, 255, 255, 255)
#define PURPLE Color (255, 0, 255, 255)
#define DARK_GREY Color (90, 90, 90, 255)
#define DUMPSTER_GREEN Color (0, 79, 0, 255)

struct ParticleSystem;
struct Sprite;
struct Transform;

using Math::Vec2;


// Contain unique information for each particles
class Particle {
public:
	Particle();

	// Initialise dorment particle
	void Init(Vec2, Transform, float, EntityID);

	// Update the particle's info, such as lifespan and movement
	void Update();

	void Destroy();

	bool GetIsActive() const { return mLifespan != 0; }
	Color GetColor() const { return mColor; }
	Transform GetTransform() const { return mTransform; }
	EntityID GetEntityID() const { return mEntity; }
	float GetLifespan() const { return mLifespan; }

private:
	Transform mTransform;		// Position, scale and rotation of the particle
	Vec2 mVelocity;					// Direction of where particle is traveling
	Color mColor;						// Color of particle
	float mLifespan;				// Lifespan of particle, despawns if it reaches 0
	float mMaxLifespan;			// Initial lifespan of particle. Mainly used for calculation of interpolation for fading
	EntityID mEntity;				// The entity that owns the particle system which owns this particle
};


// Particle Manager handles all the updates of the particle systems in the ECS. 
// It also stores the memory of all the particles
// Particle System handles the spawning and initialisation of all the particles in the system
// Once the particle is spawned, it is independent.
class ParticleManager : public System {
public:
	ParticleManager();

	// Look through all the particle systems and generate particles.
	// If particle system is active, it will generate 1 time. If it has time limit, It will generate 
	// every frame until the time limit is reached
	void Update();

	// Generate one frame of particles. (This will override any duration to 0, only generate for 1 frame
	void GenerateOnce(Entity _e) const { _e.GetComponent<ParticleSystem>().mIsActive = true;  _e.GetComponent<ParticleSystem>().mDuration = 0; }

	// Generate loop of time frame
	void GenerateLoop(Entity _e, float _duration) const { 
		LOG_CUSTOM("PARTICLESYSTEM", "Entity: " + std::to_string(_e.id) + " | Duration: " + std::to_string(_duration));
		_e.GetComponent<ParticleSystem>().mIsActive = true; _e.GetComponent<ParticleSystem>().mDuration = _duration; 
	}

	void Reset();

	auto ParticleBegin() { return mParticles.begin(); }
	auto ParticleLast() { return mParticles.begin() + mParticleCount; }
	auto ParticleEnd() { return mParticles.end(); }

	uint64_t ParticleCount() const { return mParticleCount; }

	static inline void DecreaseParticleCount() { --mParticleCount; }
	static inline void ParticleChanged() { mParticleChange = true; }

private:
	void UpdateSystems(); // Update the generation of particles

	void UpdateParticles(); // Update movement of particles and render them

	void GenerateParticle(ParticleSystem const&, EntityID _e);

private:
	//SparseSet<Particle> mParticles;
	std::array<Particle, MAX_PARTICLES> mParticles;
	static uint64_t mParticleCount; // Amount of particles, also the position to the last particle
	static bool mParticleChange;		// True if particle was created or destroyed in the current frame
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

