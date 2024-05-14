#include "EnemyWaitState.h"
#include "../Enemy.h"

void EnemyWaitState::Initialize()
{
	enemy_->SetState(this);

	enemy_->ReleaseParent();
}

void EnemyWaitState::Update()
{

}
