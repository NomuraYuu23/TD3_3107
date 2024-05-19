#include "GroupEnemyState.h"
#include "../Enemy.h"
#include "../../../Engine/Math/DeltaTime.h"

void GroupEnemyState::Initialize()
{
	enemy_->SetState(this);
	// 行動の初期化
	(this->*actionFuncs_[attackPattern_].initFunc_)();
	enemy_->velocity_.y = 0.0f;
}

void GroupEnemyState::Update()
{
	// 速度処理
	if (!enemy_->IsGround()) {
		//enemy_->velocity_.y += (kGravity) * kDeltaTime_ * (1.0f / IObject::sPlaySpeed);
	}
	else {

	}
	// 落下処理
	enemy_->transform_.translate += enemy_->velocity_ * kDeltaTime_ * (1.0f / IObject::sPlaySpeed);

	// 行動の更新
	(this->*actionFuncs_[attackPattern_].updateFunc_)();

}

void GroupEnemyState::MeleeInitialize()
{

}

void GroupEnemyState::MeleeUpdate()
{

}

void GroupEnemyState::RangedInitialize()
{

}

void GroupEnemyState::RangedUpdate()
{

}

void GroupEnemyState::MoveUpdate()
{

}
