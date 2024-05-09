#include "AerialState.h"
#include "../Player.h"
#include "../../GameUtility/MathUtility.h"

void AerialState::Initialize()
{
	//player_->worldtransform_.transform_.translate.y += 5.0f;
	// 落下処理
	// jsonデータ
	const char* groupName = "Player";
	player_->velocity_.y = GlobalVariables::GetInstance()->GetFloatValue(groupName, "NormalJumpPower");
	//player_->velocity_.x *= 0.5f;
	
	//groupName = "Common";
	
	gravity_ = GlobalVariables::GetInstance()->GetFloatValue(groupName, "Gravity");


	player_->SetNowState(this);
	player_->isGround_ = true;

	// 速度をこちらの変数に
	velocity_ = player_->velocity_;
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

	//if(pvelocity_.x)
	//velocity_.x = MathUtility::Lerp(velocity_.x, 0, 0.005f);
	//velocity_.y += mass * (kGravity * gravity_) * kDeltaTime_ * (1.0f / IObject::sPlaySpeed);

	//// プレイヤーに渡す
	//player_->velocity_ = velocity_;
	float activeRatio = GlobalVariables::GetInstance()->GetFloatValue("Player", "AerialActiveDecelerateRatio");
	float inActiveRatio = GlobalVariables::GetInstance()->GetFloatValue("Player", "AerialInActiveDecelerateRatio");
	Vector2 leftStick = Input::GetInstance()->GetLeftAnalogstick();
	// X速度
	if (leftStick.x != 0) {
		player_->velocity_.x = MathUtility::Lerp(player_->velocity_.x, 0, activeRatio);
	}
	else {
		player_->velocity_.x = MathUtility::Lerp(player_->velocity_.x, 0, inActiveRatio);
	}

	// Y速度
	if (player_->IsNowAssistDash()) {
		float ratio = GlobalVariables::GetInstance()->GetFloatValue("Dash", "SlowRatio");
		player_->velocity_.y += mass * (kGravity * (gravity_ / ratio)) * kDeltaTime_ * (1.0f / IObject::sPlaySpeed);
	}
	else {
		player_->velocity_.y += mass * (kGravity * gravity_) * kDeltaTime_ * (1.0f / IObject::sPlaySpeed);
	}

	// 移動処理
	player_->worldtransform_.transform_.translate.x += (player_->velocity_.x) * kDeltaTime_ * (1.0f / IObject::sPlaySpeed);
	player_->worldtransform_.transform_.translate.y += (player_->velocity_.y) * kDeltaTime_ * (1.0f / IObject::sPlaySpeed);

}
