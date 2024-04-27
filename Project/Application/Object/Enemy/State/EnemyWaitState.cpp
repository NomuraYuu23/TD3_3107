#include "EnemyWaitState.h"
#include "../Enemy.h"

void EnemyWaitState::Initialize()
{
	enemy_->SetState(this);

	enemy_->transform_.translate = enemy_->GetWorldPosition();
	enemy_->SetParent(nullptr);
	enemy_->MatrixUpdate();
}

void EnemyWaitState::Update()
{

}
