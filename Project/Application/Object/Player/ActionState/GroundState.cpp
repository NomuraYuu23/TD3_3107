#include "GroundState.h"
#include "../Player.h"

void GroundState::Initialize()
{
	moveSpeed_ = 6.0f;
	player_->SetNowState(this);

	SetIsFall(true);

	player_->velocity_ = {};
	fallPower_ = -(1.0f/1.0f);
}

void GroundState::Update()
{
	// 仮の重力
	// 落下中か
	//if (isFall_) {
	//	player_->worldtransform_.transform_.translate.y += (-9.0f) * kDeltaTime_;
	//}
	//if (player_->worldtransform_.transform_.translate.y <= 0) {
	//	player_->worldtransform_.transform_.translate.y = 0;
	//	player_->velocity_.y = 0;
	//}
	//else {

	//}

	if (!player_->isGround_) {
		player_->velocity_.y += fallPower_ + (-kGravity) * kDeltaTime_;
		player_->worldtransform_.transform_.translate.y += player_->velocity_.y * kDeltaTime_ * (1.0f / IObject::sPlaySpeed);
	}
}
