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

	// ジャンプ開始アニメーション
	//player_->GetAnimManager()->PlayAnimation(PlayerAnimManager::JumpStart);
}

void AerialState::Update()
{
	// Y軸更新処理
	float mass = 1.0f;

	//// プレイヤーに渡す
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
		if (player_->velocity_.y < 0) {
			player_->velocity_.y += mass * (kGravity * (gravity_ / ratio)) * kDeltaTime_ * (1.0f / IObject::sPlaySpeed);
		}
		else {
			player_->velocity_.y += mass * (kGravity * gravity_) * kDeltaTime_ * (1.0f / IObject::sPlaySpeed);
		}
	}
	else {
		player_->velocity_.y += mass * (kGravity * gravity_) * kDeltaTime_ * (1.0f / IObject::sPlaySpeed);
	}

	// 移動処理
	player_->worldtransform_.transform_.translate.x += (player_->velocity_.x) * kDeltaTime_ * (1.0f / IObject::sPlaySpeed);
	player_->worldtransform_.transform_.translate.y += (player_->velocity_.y) * kDeltaTime_ * (1.0f / IObject::sPlaySpeed);

}
