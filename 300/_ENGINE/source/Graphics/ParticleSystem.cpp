#include <Graphics/ParticleSystem.h>
//
//void ParticleEmitter::Emit(int count, vec3 const& camRightVector, vec3 const& camUpVector)
//{
//	for (int i{}; i < count; ++i)
//	{
//		Particle p;
//		ParticleProperties props;
//
//		props.mVelocity = vec3(glm::circularRand(1.f), 0.f);
//		props.mVelocity = props.mVelocity.x * camRightVector + props.mVelocity.y * camUpVector;
//		props.mVelocityVariation = GFX::Utils::Random(0.3f, 0.8f) * camUpVector + GFX::Utils::Random(-0.3f, 0.3f) * camRightVector;
//
//		p.mProperties	= props;
//		p.mCurrColor	= mProperties.mStartColor;
//		p.mCurrPosition = mProperties.mPosition;
//		p.mCurrRotation = GFX::Utils::Random(0.f, 360.f);
//		p.mCurrSize		= mProperties.mStartSize;
//		p.mCurrLife		= 0.f;
//		p.mProperties.mSpeed *= GFX::Utils::Random(0.5f, 1.f);
//
//		mParticles.emplace_back(std::move(p));
//	}
//}
//
//void ParticleEmitter::Update(float dt)
//{
//	std::vector<std::list<Particle>::iterator> toBeDeleted{};
//	for (auto it = mParticles.begin(); it != mParticles.end(); it++)
//	{
//		// Deletes particle that exceeded lifetime
//		if (it->mCurrLife > it->mProperties.mLifetime)
//		{
//			toBeDeleted.emplace_back(it);
//			continue;
//		}
//
//		float factor = it->mCurrLife / it->mProperties.mLifetime;
//
//		it->mCurrColor		= glm::mix(it->mProperties.mStartColor, it->mProperties.mEndColor, factor);
//		it->mCurrPosition	= it->mCurrPosition + (it->mProperties.mVelocity * it->mProperties.mSpeed * dt);
//		it->mCurrSize		= glm::mix(it->mProperties.mStartSize, it->mProperties.mEndSize, factor);
//		it->mCurrLife		+= dt;
//		it->mCurrRotation	+= 90.f * dt;
//		it->mProperties.mSpeed -= dt;
//		it->mProperties.mVelocity = glm::normalize(it->mProperties.mVelocity + it->mProperties.mVelocityVariation);
//	}
//
//	for (int i{}; i < toBeDeleted.size(); ++i)
//	{
//		mParticles.erase(toBeDeleted[i]);
//	}
//}

void ParticleEmitterSSBO::Init(ParticleEmitter const& e)
{
	mStartColor = e.mStartColor;
	mEndColor = e.mEndColor;
	mSizeLifetimeSpeed = vec4(e.mStartSize, e.mEndSize, e.mLifetime, e.mSpeed);
}
