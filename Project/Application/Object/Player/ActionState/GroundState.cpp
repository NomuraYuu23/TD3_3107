#include "GroundState.h"
#include "../Player.h"

void GroundState::Initialize()
{
}

void GroundState::Update()
{
	// ステート変更
	if (player_->input_->TriggerKey(DIK_SPACE)) {
		player_->ChangeState(std::make_unique<AerialState>());
		return;
	}

	// 移動入力
	float tmpSpeed = 2.0f;
	if (player_->input_->PushKey(DIK_A)) {
		player_->velocity_.x = -tmpSpeed;
	}
	else if (player_->input_->PushKey(DIK_D)) {
		player_->velocity_.x = tmpSpeed;
	}
	else {
		player_->velocity_.x = 0;
	}
	player_->worldtransform_.transform_.translate.x += player_->velocity_.x * kDeltaTime_;

}
