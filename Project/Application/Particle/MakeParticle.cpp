#include "MakeParticle.h"
#include "user/EnemyDead/EnemyDeadParticle.h"
#include "user/RunSmoke/RunSmoke.h"
#include "user/SpearCatch/SpearCatchParticle.h"
#include <cassert>

MakeParticle* MakeParticle::GetInstance()
{
    return nullptr;
}

IParticle* MakeParticle::Run(ParticleDesc* particleDesc, uint32_t paeticleName)
{
	
	IParticle* particle = nullptr;

	switch (paeticleName)
	{
	case kDefaultParticle:
		particle = new IParticle();
		particle->Initialize(particleDesc);
		break;
	case kEnemyDeadParticle:
		particle = new EnemyDeadParticle();
		particle->Initialize(particleDesc);
		break;
	case kRunSmokeParticle:
		particle = new RunSmoke();
		particle->Initialize(particleDesc);
		break;
	case kSpearCatchParticle:
		particle = new SpearCatchParticle();
		particle->Initialize(particleDesc);
		break;
	case kCountOfParticleName:
	default:
		assert(0);
		break;
	}

	return particle;

}
