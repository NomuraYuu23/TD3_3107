#include "EnemyGroundState.h"
#include "../Enemy.h"
#include "../../../Engine/Math/DeltaTime.h"

void EnemyGroundState::Initialize()
{
	enemy_->SetState(this);
	// 行動の初期化
	(this->*actionFuncs_[attackPattern_].initFunc_)();
}

void EnemyGroundState::Update()
{
	// 行動の更新
	(this->*actionFuncs_[attackPattern_].updateFunc_)();

}

void EnemyGroundState::MeleeInitialize()
{
	enemy_->transform_.translate.y += 5.0f * kDeltaTime_ * (1.0f / IObject::sPlaySpeed);
}

void EnemyGroundState::MeleeUpdate()
{
	enemy_->transform_.translate.x += 5.0f * kDeltaTime_ * (1.0f / IObject::sPlaySpeed);
}

void EnemyGroundState::RangedInitialize()
{
	enemy_->transform_.translate.y -= 5.0f * kDeltaTime_ * (1.0f / IObject::sPlaySpeed);
}

void EnemyGroundState::RangedUpdate()
{
	enemy_->transform_.translate.x -= 5.0f * kDeltaTime_ * (1.0f / IObject::sPlaySpeed);
}

void EnemyGroundState::MoveUpdate()
{
	if (enemy_->GetIsGround()) {

	}
	else {

	}


}
