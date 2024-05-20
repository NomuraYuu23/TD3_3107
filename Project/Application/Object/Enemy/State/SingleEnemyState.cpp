#include "SingleEnemyState.h"
#include "../Enemy.h"

void SingleEnemyState::Initialize()
{
	enemy_->SetState(this);
	// 行動の初期化
	(this->*actionFuncs_[attackPattern_].initFunc_)();
}

void SingleEnemyState::Update()
{

	// 行動の更新
	(this->*actionFuncs_[attackPattern_].updateFunc_)();

}

void SingleEnemyState::MeleeInitialize()
{
}

void SingleEnemyState::MeleeUpdate()
{
}

void SingleEnemyState::RangedInitialize()
{
}

void SingleEnemyState::RangedUpdate()
{
}
