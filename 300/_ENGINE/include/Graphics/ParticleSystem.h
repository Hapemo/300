#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include <pch.h>
#include <ECS/ECS_Systems.h>
#include <ECS/ECS_Components.h>
#include <ECS/ECS.h>

struct Particle
{

};

struct ParticleProperties
{
	vec4 mStartColor, mEndColor;
	vec3 mPosition;
	vec3 mVelocity, mVelocityVariation;
	float mStartSize, mEndSize, mSizeVariation;
	float mLifetime{ 1.f };
};

class ParticleEmitter
{
public:

private:
	ParticleProperties mProperties;
	int mCount{};

};

#endif