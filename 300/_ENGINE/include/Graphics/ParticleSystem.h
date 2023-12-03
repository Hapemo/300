#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include <pch.h>
#include <ECS/ECS_Systems.h>
#include <ECS/ECS_Components.h>
#include <ECS/ECS.h>


// Properties of particles
struct ParticleProperties
{
	vec4 mStartColor{ 1.f, 1.f, 1.f, 1.f }, mEndColor{ 1.f, 0.f, 0.f, 0.3f };
	vec3 mPosition{ 0.f, -40.f, 0.f };
	vec3 mVelocity, mVelocityVariation;
	float mStartSize{ 5 }, mEndSize{ 0.1 }, mSizeVariation;
	float mLifetime{ 2.f };
	float mSpeed{ 10 };
};

// Stats of each particle instance
struct Particle
{
	ParticleProperties mProperties;
	vec4 mCurrColor;
	vec3 mCurrPosition;
	float mCurrRotation;
	float mCurrSize;
	float mCurrLife;
};

// Emits the particles with specified properties
class ParticleEmitter
{
public:
	void Init(ParticleProperties const& props) { mProperties = props; }
	void Emit(int count);
	void Update(float dt);

	std::list<Particle> mParticles;
private:
	ParticleProperties mProperties;
};

#endif