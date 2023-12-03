#include <Graphics/ParticleSystem.h>

void ParticleEmitter::Emit(int count)
{
	for (int i{}; i < count; ++i)
	{
		Particle p;
		ParticleProperties props;

		props.mVelocity = glm::ballRand(1.f);

		p.mProperties	= props;
		p.mCurrColor	= mProperties.mStartColor;
		p.mCurrPosition = mProperties.mPosition;
		p.mCurrRotation = GFX::Utils::Random(0.f, 360.f);
		p.mCurrSize		= mProperties.mStartSize;
		p.mCurrLife		= 0.f;

		mParticles.emplace_back(std::move(p));
	}
}

void ParticleEmitter::Update(float dt)
{
	std::vector<std::list<Particle>::iterator> toBeDeleted{};
	for (auto it = mParticles.begin(); it != mParticles.end(); it++)
	{
		// Deletes particle that exceeded lifetime
		if (it->mCurrLife > it->mProperties.mLifetime)
		{
			toBeDeleted.emplace_back(it);
			continue;
		}

		float factor = it->mCurrLife / it->mProperties.mLifetime;

		it->mCurrColor		= glm::mix(it->mProperties.mStartColor, it->mProperties.mEndColor, factor);
		it->mCurrPosition	= it->mCurrPosition + (it->mProperties.mVelocity * it->mProperties.mSpeed * dt);
		it->mCurrSize		= glm::mix(it->mProperties.mStartSize, it->mProperties.mEndSize, factor);
		it->mCurrLife		+= dt;
	}

	for (int i{}; i < toBeDeleted.size(); ++i)
	{
		mParticles.erase(toBeDeleted[i]);
	}
}
