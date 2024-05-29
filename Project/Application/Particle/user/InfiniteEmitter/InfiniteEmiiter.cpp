#include "InfiniteEmiiter.h"
#include "../../../Engine/Math/DeltaTime.h"
#include "../../MakeParticle.h"
#include "../../../../Engine/base/OutputLog.h"

void InfiniteEmiiter::Initialize(EmitterDesc* emitterDesc)
{
	// 基底クラスを初期化
	IEmitter::Initialize(emitterDesc);


	// トランスフォームのポインタを保存
	emitTransform_ = emitterDesc->transform;

}

void InfiniteEmiiter::Update()
{
	// 一生生成を行う
	frequencyTime_ += kDeltaTime_;
	if (frequencyTime_ >= frequency_) {
		frequencyTime_ = frequencyTime_ - frequency_;
		toEmit_ = true;
	}

	// トランスフォームがnullptrになったらエミッタを破棄
	if (emitTransform_ == nullptr) {
		isDead_ = true;
	}
}

std::list<IParticle*> InfiniteEmiiter::Emit()
{
	std::list<IParticle*> particles;
	MakeParticle* makeParticle = MakeParticle::GetInstance();

	ParticleDesc desc;

	// トランスフォームのポインタが渡されていれば
	if (emitTransform_ != nullptr) {
		desc = { emitTransform_->translate ,emitTransform_->scale, velocity_ };
	}
	else {
		desc = { transform_.translate ,transform_.scale, velocity_ };
	}
	

	for (uint32_t count = 0; count < instanceCount_; ++count) {
		particles.push_back(makeParticle->Run(&desc, paeticleName_));
	}

	return particles;
}
