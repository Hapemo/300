#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include <pch.h>
#include <ECS/ECS_Systems.h>
#include <ECS/ECS_Components.h>
#include <ECS/ECS.h>


// Properties of particles
//struct ParticleEmitter
//{
//	vec4 mStartColor{ 1.f, 1.f, 1.f, 1.f }, mEndColor{ 1.f, 0.f, 0.f, 0.3f };
//	vec3 mPosition{ 0.f, -40.f, 0.f };
//	vec3 mVelocity, mVelocityVariation;
//	float mStartSize{ 0.4f }, mEndSize{ 0.1f }, mSizeVariation;
//	float mLifetime{ 2.f };
//	float mSpeed{ 3.f };
//	int	mCount{ 100 };
//};

//// Stats of each particle instance
//struct Particle
//{
//	ParticleProperties mProperties;
//	vec4 mCurrColor;
//	vec3 mCurrPosition;
//	float mCurrRotation;
//	float mCurrSize;
//	float mCurrLife;
//};
//
//// Emits the particles with specified properties
//class ParticleEmitter
//{
//public:
//	void Init(ParticleProperties const& props) { mProperties = props; }
//	void Emit(int count, vec3 const& camRightVector, vec3 const& camUpVector);
//	void Update(float dt);
//
//	std::list<Particle> mParticles;
//private:
//	ParticleProperties mProperties;
//};

struct ParticleEmitterSSBO
{
	alignas(16) vec4 mStartColor{};
	alignas(16) vec4 mEndColor{};
	alignas(16) vec4 mSizeLifetimeCount{};	// X: start size | Y: end size | Z: Lifetime | W: Count
	alignas(16) vec4 mPositionSpeed{};		// XYZ: position | W: Speed
	alignas(8)	GLuint64 mTexture{};

	void Init(ParticleEmitter const& e);
};

struct ParticleSSBO
{
	alignas(16) vec4 mStartColor	{};
	alignas(16) vec4 mEndColor		{};
	alignas(16) vec4 mCurrColor		{};
	alignas(16) vec4 mVelocity		{};
	alignas(16) vec4 mSizeLife		{};	// X: Start size | Y: End size | Z: Life Time left | W: Max Life
	alignas(16) vec4 mPositionSpeed	{};	// XYZ: position | W: Speed
	alignas(8)	GLuint64 mTexture	{};
	alignas(64) mat4 mLtwMatrix		{};	// Local-to-world transformation matrix
};

#endif