#include "WaveAttackState.h"
#include "../PrevSmallBoss.h"
#include "../Weapon/WaveAttack.h"

void WaveAttackState::Initialize()
{
	CreateWave();
}

void WaveAttackState::Update()
{

}

void WaveAttackState::CreateWave()
{
	std::unique_ptr<WaveAttack> waveAttackObj = std::make_unique<WaveAttack>();
	// 座標
	waveAttackObj->Initialize(boss_->model_);
	waveAttackObj->worldtransform_.transform_.translate = boss_->worldtransform_.GetWorldPosition();
	waveAttackObj->CreateVelocity({ 1,0,0 });

	// リストに追加
	static_cast<PrevSmallBoss*>(boss_)->attackLists_.push_back(std::move(waveAttackObj));
}
