#include "GroundState.h"
#include "../Player.h"

void GroundState::Initialize()
{
}

void GroundState::Update()
{

	float tmpSpeed = 0.05f;
	if (player_->input_->PushKey(DIK_A)) {
		player_->worldtransform_.transform_.translate.x -= tmpSpeed;
	}
	if (player_->input_->PushKey(DIK_D)) {
		player_->worldtransform_.transform_.translate.x += tmpSpeed;
	}

	if (player_->input_->TriggerKey(DIK_SPACE)) {
		player_->ChangeState(std::make_unique<AerialState>());
	}

}
