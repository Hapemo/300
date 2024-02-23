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
	float mStartSize{ 0.4f }, mEndSize{ 0.1f }, mSizeVariation;
	float mLifetime{ 2.f };
	float mSpeed{ 3.f };
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
	void Emit(int count, vec3 const& camRightVector, vec3 const& camUpVector);
	void Update(float dt);

	std::list<Particle> mParticles;
private:
	ParticleProperties mProperties;
};

struct ParticleEmitterSSBO
{
	alignas(16) vec4 mStartColor{};
	alignas(16) vec4 mEndColor{};
	alignas(16) vec4 mSizeLifetimeSpeed{};	// X: start size | Y: end size | Z: Lifetime | W: Speed
	alignas(16) vec4 mPosition{};			// XYZ: position | W: particle count
	alignas(16) vec4 mParticlePoolIndex{};	// X: Particle pool start index, the starting index of particle pool for this emitter
	alignas(8)	GLuint64 mTexture{};
};

struct ParticleSSBO
{
	alignas(16) vec4 mColor{};
	alignas(16) vec4 mVelocity{};
	alignas(16) vec4 mSizeLifeSpeed{};	// X: Current size | Y: Life Time left | Z: Speed
	alignas(16) vec4 mPosition {};		// XYZ: position | W: active flag (< 0 inactive, else active)
	alignas(8)	GLuint64 mTexture {};
};

#endif