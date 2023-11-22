#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include <pch.h>
#include <ECS/ECS_Systems.h>
#include <ECS/ECS_Components.h>
#include <ECS/ECS.h>

// Properties of particles
struct ParticleProperties
{
	vec4 mStartColor, mEndColor;
	vec3 mPosition;
	vec3 mVelocity, mVelocityVariation;
	float mStartSize, mEndSize, mSizeVariation;
	float mLifetime{ 1.f };
};

// Stats of each particle instance
struct Particle
{
	ParticleProperties mProperties;
	vec4 mCurrColor;
	vec3 mCurrPosition;
	float mCurrRotation;
	float mCurrSize;
	float currLife;
};

// Emits the particles with specified properties
class ParticleEmitter
{
public:

private:
	ParticleProperties mProperties;
	int mCount{};
};

#endif