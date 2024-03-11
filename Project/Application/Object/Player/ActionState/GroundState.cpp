#include "GroundState.h"
#include "../Player.h"

void GroundState::Initialize()
{
	moveSpeed_ = 6.0f;
	player_->SetNowState(this);
}

void GroundState::Update()
{
	// 仮の重力	
	player_->worldtransform_.transform_.translate.y += (-9.0f) * kDeltaTime_;

	if (player_->worldtransform_.transform_.translate.y <= 0) {
		//isFall_
		player_->worldtransform_.transform_.translate.y = 0;
	}

}
