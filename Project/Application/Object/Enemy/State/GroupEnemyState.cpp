#include "GroupEnemyState.h"
#include "../Enemy.h"
#include "../../../Engine/Math/DeltaTime.h"

void GroupEnemyState::Initialize()
{
	enemy_->SetState(this);
	// 行動の初期化
	(this->*actionFuncs_[actionPattern_].initFunc_)();

}

void GroupEnemyState::Update()
{

	if (enemy_->GetMoveStop()) {
		enemy_->ReleaseParent();
	}
	else {
		enemy_->ResetParent();
	}

	// 行動の更新
	(this->*actionFuncs_[actionPattern_].updateFunc_)();

}

void GroupEnemyState::PatrolInitialize()
{

}

void GroupEnemyState::PatrolUpdate()
{

}

void GroupEnemyState::ChaseInitialize()
{
	
}

void GroupEnemyState::ChaseUpdate()
{

}

void GroupEnemyState::MoveUpdate()
{

}
