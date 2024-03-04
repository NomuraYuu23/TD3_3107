#include "AerialState.h"
#include "../Player.h"

void AerialState::Initialize()
{
}

void AerialState::Update()
{
	if (player_->input_->TriggerKey(DIK_SPACE)) {
		player_->ChangeState(std::make_unique<GroundState>());
	}
}
