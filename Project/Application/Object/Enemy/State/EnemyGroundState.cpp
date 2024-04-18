#include "EnemyGroundState.h"
#include "../Enemy.h"
#include "../../../Engine/Math/DeltaTime.h"

void EnemyGroundState::Initialize()
{
	enemy_->SetState(this);
	// 行動の初期化
	(this->*actionFuncs_[attackPattern_].initFunc_)();
	enemy_->velocity_.y = 0.0f;
}

void EnemyGroundState::Update()
{
	// 速度処理
	if (!enemy_->IsGround()) {
		enemy_->velocity_.y += (kGravity) * kDeltaTime_ * (1.0f / IObject::sPlaySpeed);
	}
	else {

	}
	// 落下処理
	enemy_->transform_.translate += enemy_->velocity_ * kDeltaTime_ * (1.0f / IObject::sPlaySpeed);

	// 行動の更新
	(this->*actionFuncs_[attackPattern_].updateFunc_)();

}

void EnemyGroundState::MeleeInitialize()
{
	enemy_->velocity_.x = 0.0f;
}

void EnemyGroundState::MeleeUpdate()
{
	//enemy_->velocity_.x += kDeltaTime_* (1.0f / IObject::sPlaySpeed);
	//enemy_->transform_.translate += enemy_->velocity_ * kDeltaTime_ * (1.0f / IObject::sPlaySpeed);
}

void EnemyGroundState::RangedInitialize()
{
	enemy_->velocity_.y = 3.0f;
}

void EnemyGroundState::RangedUpdate()
{
	//enemy_->velocity_.x += kDeltaTime_ * (1.0f / IObject::sPlaySpeed);
	//enemy_->transform_.translate += enemy_->velocity_ * kDeltaTime_ * (1.0f / IObject::sPlaySpeed);
}

void EnemyGroundState::MoveUpdate()
{
	//if (enemy_->GetIsGround()) {

	//}
	//else {

	//}


}
