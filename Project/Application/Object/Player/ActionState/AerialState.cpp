#include "AerialState.h"
#include "../Player.h"

void AerialState::Initialize()
{
	//player_->worldtransform_.transform_.translate.y += 5.0f;
	// 落下処理
	player_->velocity_.y = 20.0f;

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

}

void AerialState::Update()
{
	// 落下確認処理
	if (player_->worldtransform_.transform_.translate.y < 0.0f) {
		player_->worldtransform_.transform_.translate.y = 0;
		player_->ChangeState(std::make_unique<GroundState>());
		return;
	}

	// 落下処理
	// ジャンプ（重力計算
	jumpPower_ += (-player_->gravity_) * kDeltaTime_;


	// Y軸更新処理
	player_->velocity_.y += jumpPower_;

	if (player_->velocity_.y <= -30.0f) {
		player_->velocity_.y = -30.0f;
	}

	player_->worldtransform_.transform_.translate.y += player_->velocity_.y * kDeltaTime_;
	
	// X軸更新処理
	velocity_.x += velocity_.x * kDeltaTime_;
	player_->worldtransform_.transform_.translate.x += velocity_.x * kDeltaTime_;

}
