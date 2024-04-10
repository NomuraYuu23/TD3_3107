#include "WallJumpState.h"
#include "../Player.h"
#include "../../GameUtility/MathUtility.h"

void WallJumpState::Initialize()
{
	float jumpPower = 15.0f;
	if (player_->velocity_.x > 0) {
		player_->velocity_.x = jumpPower * -1.0f;
	}
	else {
		player_->velocity_.x = jumpPower;
	}

	player_->velocity_.y = jumpPower;

	gravity_ = GlobalVariables::GetInstance()->GetFloatValue("Player", "Gravity");
	
	player_->SetNowState(this);
	player_->isGround_ = true;
}

void WallJumpState::Update()
{
	//if (finishTimer_.IsEnd()) {

	//}
	float mass = 1.0f;

	//player_->velocity_.x = MathUtility::Lerp(player_->velocity_.x, 0, 0.005f);
	player_->velocity_.y += mass * (kGravity * gravity_) * kDeltaTime_ * (1.0f / IObject::sPlaySpeed);

	player_->worldtransform_.transform_.translate += player_->velocity_ * kDeltaTime_ * (1.0f / IObject::sPlaySpeed);

}
