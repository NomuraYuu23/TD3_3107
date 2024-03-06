#include "AerialState.h"
#include "../Player.h"

void AerialState::Initialize()
{
	//player_->worldtransform_.transform_.translate.y += 5.0f;
	// 落下処理
	player_->velocity_.y = 0;

	jumpPower_ = 2.0f;

	// 横方向用の計算
	velocity_ = {};
	if (player_->velocity_.x > 0) {
		velocity_.x = 6.0f;
	}
	else if (player_->velocity_.x < 0) {
		velocity_.x = -6.0f;
	}

}

void AerialState::Update()
{
	//if (player_->input_->TriggerKey(DIK_SPACE)) {
	//	player_->ChangeState(std::make_unique<GroundState>());
	//}
	// 落下確認処理
	if (player_->worldtransform_.transform_.translate.y < 0.0f) {
		player_->worldtransform_.transform_.translate.y = 0;
		player_->ChangeState(std::make_unique<GroundState>());
		return;
	}

	// 落下処理
	jumpPower_ += 0.02f;
	jumpPower_ += (-player_->gravity_) * kDeltaTime_;
	player_->velocity_.y += jumpPower_;
	player_->worldtransform_.transform_.translate.y += player_->velocity_.y * kDeltaTime_;
	velocity_.x += velocity_.x * kDeltaTime_;
	player_->worldtransform_.transform_.translate.x += velocity_.x * kDeltaTime_;

}
