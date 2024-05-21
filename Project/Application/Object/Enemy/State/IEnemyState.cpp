#include "IEnemyState.h"
#include "../Enemy.h"
#include "../../../Engine/Math/DeltaTime.h"
#include "../../GameSystem/GameSystemManager.h"

void IEnemyState::PreInitialize(Enemy* enemy, ActionMode pattern)
{
	// 対象の設定
	enemy_ = enemy;
	// 攻撃のパターン設定
	actionPattern_ = static_cast<uint32_t>(pattern);
}

void IEnemyState::PreInitialize(Enemy* enemy)
{
	// 対象の設定
	enemy_ = enemy;
}

void IEnemyState::Update()
{

	enemy_->transform_.translate += enemy_->velocity_ * kDeltaTime_ * (1.0f / GameSystemManager::sGameSpeed);
}
