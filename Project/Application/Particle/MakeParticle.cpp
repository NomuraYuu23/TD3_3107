#include "MakeParticle.h"
#include "user/EnemyDead/EnemyDeadParticle.h"
#include "user/RunSmoke/RunSmoke.h"
#include "user/SpearCatch/SpearCatchParticle.h"
#include "user/FallingLeaf/FallingLeafParticle.h"
#include "user/SpearJump/SpearJumpParticle.h"
#include "user/GoalParticle/GoalParticle.h"
#include "user/SmokeParticle/SmokeParticle.h"
#include "user/ImapledParticle/ImpaledParticle.h"
#include "user/DeadForcusing/DeadForcusingParticle.h"
#include "user/PlayerAttack/PlayerAttackParticle.h"
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
	case kSpearJumpParticle:
		particle = new SpearJumpParticle();
		particle->Initialize(particleDesc);
		break;
	case kSpearCatchParticle:
		particle = new SpearCatchParticle();
		particle->Initialize(particleDesc);
		break;
	case kFallingLeafParticle:
		particle = new FallingLeafParticle();
		particle->Initialize(particleDesc);
		break;
	case kGoalParticle:
		particle = new GoalParticle();
		particle->Initialize(particleDesc);
		break;
	case kSmokePaticle:
		particle = new SmokeParticle();
		particle->Initialize(particleDesc);
		break;
	case kImpaledParticle:
		particle = new ImpaledParticle();
		particle->Initialize(particleDesc);
		break;
	case kDeadForcusingParticle:
		particle = new DeadForcusingParticle();
		particle->Initialize(particleDesc);
		break;
	case kPlayerAttackParticle:
		particle = new PlayerAttackParticle();
		particle->Initialize(particleDesc);
		break;

	case kCountOfParticleName:
		break;
	default:
		assert(0);
		break;
	}

	return particle;

}
