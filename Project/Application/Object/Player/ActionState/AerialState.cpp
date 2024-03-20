#include "AerialState.h"
#include "../Player.h"

void AerialState::Initialize()
{
	//player_->worldtransform_.transform_.translate.y += 5.0f;
	// 落下処理
	// jsonデータ
	const char* groupName = "Player";
	player_->velocity_.y = GlobalVariables::GetInstance()->GetFloatValue(groupName, "NormalJumpPower");
	
	groupName = "Common";
	
	gravity_ = GlobalVariables::GetInstance()->GetFloatValue(groupName, "Gravity");

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

void AerialState::Update()
{
	// 落下確認処理
	//if (player_->worldtransform_.transform_.translate.y < 0.0f) {
	//	player_->worldtransform_.transform_.translate.y = 0;
	//	player_->ChangeState(std::make_unique<GroundState>());
	//	return;
	//}

	// 落下処理
	// ジャンプ（重力計算
	//jumpPower_ += (-player_->gravity_) * kDeltaTime_;

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

	player_->velocity_.y += mass * (-gravity_) * kDeltaTime_;

	testCount++;
	// X軸処理
	velocity_.x += velocity_.x * kDeltaTime_;

	player_->worldtransform_.transform_.translate.x += player_->velocity_.x * kDeltaTime_ * (1.0f / IObject::sPlaySpeed);
	player_->worldtransform_.transform_.translate.y += player_->velocity_.y * kDeltaTime_ * (1.0f / IObject::sPlaySpeed);

}
