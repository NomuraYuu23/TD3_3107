#include "WaveAttackState.h"
#include "../PrevSmallBoss.h"
#include "../Weapon/WaveAttack.h"

void WaveAttackState::Initialize()
{
	//CreateWave();
	generateCount_ = 0;
	generateTimer_.Start(20.0f);
}

void WaveAttackState::Update()
{
	// 終了までボスは動かない

	// 一定間隔ごとに左右に生成
	if (generateTimer_.IsActive()) {
		generateTimer_.Update();
	}
	if (generateTimer_.IsEnd()) {
		if (generateCount_ > 8) {

		}
		else {
			// カウント
			generateCount_++;

			float offset = 4.5f;
			// 右
			CreateWave({ 1,0,0 }, { offset,0,0 });
			// 左
			CreateWave({ -1,0,0 }, { -offset,0,0 });

			generateTimer_.Start(20.0f);
		}
	}

}

void WaveAttackState::CreateWave(const Vector3& direct, const Vector3& offset)
{
	std::unique_ptr<WaveAttack> waveAttackObj = std::make_unique<WaveAttack>();
	// 座標
	waveAttackObj->Initialize(boss_->model_);
	waveAttackObj->worldtransform_.transform_.translate = boss_->worldtransform_.GetWorldPosition();
	waveAttackObj->worldtransform_.transform_.translate += offset;
	waveAttackObj->CreateVelocity(direct);

	// リストに追加
	static_cast<PrevSmallBoss*>(boss_)->attackLists_.push_back(std::move(waveAttackObj));
}
