#include "EnemyWaitState.h"
#include "../Enemy.h"

void EnemyWaitState::Initialize()
{
	enemy_->SetState(this);

	enemy_->ReleaseParent();
	enemy_->goalAngle_ = enemy_->parentEmitter_->GetNowAngle();
}

void EnemyWaitState::Update()
{
	//enemy_->transform_.translate.y += 0.1f;
}
