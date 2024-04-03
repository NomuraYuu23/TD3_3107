#include "IEnemyState.h"
#include "../Enemy.h"
#include "../../../Engine/Math/DeltaTime.h"

void IEnemyState::PreInitialize(Enemy* enemy, uint32_t pattern)
{
	// 対象の設定
	enemy_ = enemy;
	// 攻撃のパターン設定
	attackPattern_ = pattern;
}

void IEnemyState::Update()
{

	enemy_->transform_.translate += enemy_->velocity_ * kDeltaTime_ * (1.0f / IObject::sPlaySpeed);

}
