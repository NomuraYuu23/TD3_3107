#include "SpearAerialState.h"
#include "../Player.h"

void SpearAerialState::Initialize()
{

	// 落下処理
	player_->velocity_.y = player_->spearJumpPower_;

	jumpPower_ = 1.50f;

	// 横方向用の計算
	velocity_ = {};
	float moveXPower = 3.0f;
	if (player_->velocity_.x > 0) {
		velocity_.x = moveXPower;
	}
	else if (player_->velocity_.x < 0) {
		velocity_.x = -moveXPower;
	}

	player_->SetNowState(this);
	player_->isGround_ = true;
	testCount = 0;

}

void SpearAerialState::Update()
{

	// Y軸更新処理
	//player_->velocity_.y += jumpPower_;
	float mass = 1.0f;
	// Y軸処理
	//if (player_->velocity_.y > 0){
	//	if (testCount % 2 == 0) {
	//		player_->velocity_.y += mass * (-player_->gravity_) * kDeltaTime_;
	//		player_->worldtransform_.transform_.translate.y += player_->velocity_.y * kDeltaTime_ * (1.0f / IObject::sPlaySpeed);
	//	}
	//}
	//else {
	player_->velocity_.y += mass * (-player_->gravity_) * kDeltaTime_;
	//}
	testCount++;
	// X軸処理
	velocity_.x += velocity_.x * kDeltaTime_;

	player_->worldtransform_.transform_.translate.x += player_->velocity_.x * kDeltaTime_ * (1.0f / IObject::sPlaySpeed);
	player_->worldtransform_.transform_.translate.y += player_->velocity_.y * kDeltaTime_ * (1.0f / IObject::sPlaySpeed);


}
