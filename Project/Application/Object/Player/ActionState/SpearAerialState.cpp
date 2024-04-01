#include "SpearAerialState.h"
#include "../Player.h"
#include "../../GameUtility/MathUtility.h"

void SpearAerialState::Initialize()
{

	// 落下処理
	// jsonデータ
	const char* groupName = "Player";
	player_->velocity_.y = GlobalVariables::GetInstance()->GetFloatValue(groupName, "SpearJumpPower");

	//groupName = "Common";

	gravity_ = GlobalVariables::GetInstance()->GetFloatValue(groupName, "Gravity");


	player_->SetNowState(this);
	player_->isGround_ = true;
	// コンボ加算
	player_->jumpCombo.Add();

}

void SpearAerialState::Update()
{
	// 速度計算
	float mass = 1.0f;

	player_->velocity_.x = MathUtility::Lerp(player_->velocity_.x, 0, 0.005f);
	player_->velocity_.y += mass * (kGravity * gravity_) * kDeltaTime_ * (1.0f / IObject::sPlaySpeed);

	// 移動処理
	player_->worldtransform_.transform_.translate.x += (player_->velocity_.x * 2.0f) * kDeltaTime_ * (1.0f / IObject::sPlaySpeed);
	player_->worldtransform_.transform_.translate.y += player_->velocity_.y * kDeltaTime_ * (1.0f / IObject::sPlaySpeed);
}
