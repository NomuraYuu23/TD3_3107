#include "GroundState.h"
#include "../Player.h"

void GroundState::Initialize()
{
	// ステートの設定
	player_->SetNowState(this);

	//SetIsFall(true);

	player_->velocity_ = {};

	moveSpeed_ = 6.0f;
	fallPower_ = -(1.0f/1.0f);

	// 槍を踏んだかのフラグ
	player_->isOneStepOn_ = false;


	// 着地アニメーションの再生
	if (player_->GetAnimManager() != nullptr) {
		player_->GetAnimManager()->PlayAnimation(PlayerAnimManager::Landing);
	}
	
	player_->KnockBackOnGround();
}

void GroundState::Update()
{
	// 接地していない場合
	if (!player_->isGround_) {
		player_->velocity_.y += fallPower_ + (kGravity) * kDeltaTime_;
		player_->worldtransform_.transform_.translate.y += player_->velocity_.y * kDeltaTime_ * (1.0f / IObject::sPlaySpeed);
	}

	// 速度制限
	if (player_->velocity_.y <= -40.0f) {
		player_->velocity_.y = -40.0f;
	}
}
