#include "EnemyDeadEmitter.h"
#include "../../../Engine/Math/DeltaTime.h"
#include "../../MakeParticle.h"

void EnemyDeadEmitter::Initialize(EmitterDesc* emitterDesc)
{
	// 基底クラスの初期化実行
	IEmitter::Initialize(emitterDesc);
}

void EnemyDeadEmitter::Update()
{
	// 基底クラスの更新実行
	IEmitter::Update();
}

std::list<IParticle*> EnemyDeadEmitter::Emit()
{
	std::list<IParticle*> particles;
	MakeParticle* makeParticle = MakeParticle::GetInstance();

	ParticleDesc desc = { transform_.translate, transform_.scale };

	for (uint32_t count = 0; count < instanceCount_; ++count) {
		particles.push_back(makeParticle->Run(&desc, paeticleName_));
	}

	return particles;
}
