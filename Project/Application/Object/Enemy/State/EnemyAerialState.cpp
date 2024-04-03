#include "EnemyAerialState.h"

void EnemyAerialState::Initialize()
{
	// 行動の初期化
	(this->*actionFuncs_[attackPattern_].initFunc_)();
}

void EnemyAerialState::Update()
{

	// 行動の更新
	(this->*actionFuncs_[attackPattern_].updateFunc_)();

}

void EnemyAerialState::MeleeInitialize()
{
}

void EnemyAerialState::MeleeUpdate()
{
}

void EnemyAerialState::RangedInitialize()
{
}

void EnemyAerialState::RangedUpdate()
{
}
