#include "IEnemyState.h"
#include "../Enemy.h"
#include "../../../Engine/Math/DeltaTime.h"

void IEnemyState::PreInitialize(Enemy* enemy)
{
	// 対象の設定
	enemy_ = enemy;
}

void IEnemyState::Update()
{

	enemy_->transform_.translate += enemy_->velocity_ * kDeltaTime_ * (1.0f / IObject::sPlaySpeed);

}
