#include "EnemyWaitState.h"
#include "../Enemy.h"

void EnemyWaitState::Initialize()
{
	enemy_->SetState(this);
}

void EnemyWaitState::Update()
{

}
