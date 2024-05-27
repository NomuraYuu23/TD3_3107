#include "ChaseEnemyState.h"
#include "../../ObjectList.h"

void ChaseEnemyState::Initialize()
{
	enemy_->SetState(this);
	enemy_->MatrixUpdate();

}
