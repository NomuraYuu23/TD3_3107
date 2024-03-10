#include "GroundState.h"
#include "../Player.h"

void GroundState::Initialize()
{
	moveSpeed_ = 6.0f;
}

void GroundState::Update()
{
	//// ステート変更
	//if (player_->input_->TriggerKey(DIK_SPACE)) {
	//	player_->ChangeState(std::make_unique<AerialState>());
	//	return;
	//}

	//// 移動入力
	//if (player_->input_->PushKey(DIK_A)) {
	//	player_->velocity_.x = -moveSpeed_;
	//}
	//else if (player_->input_->PushKey(DIK_D)) {
	//	player_->velocity_.x = moveSpeed_;
	//}
	//else {
	//	player_->velocity_.x = 0;
	//}
	//player_->worldtransform_.transform_.translate.x += player_->velocity_.x * kDeltaTime_;

}
