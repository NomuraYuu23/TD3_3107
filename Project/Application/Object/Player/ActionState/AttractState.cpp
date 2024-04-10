#include "AttractState.h"
#include "../Player.h"
#include "../../../Engine/Math/Ease.h"

void AttractState::Initialize()
{
	attractTimer_.Start(15.0f);
	startPosition_ = player_->worldtransform_.GetWorldPosition();
}

void AttractState::Update()
{
	attractTimer_.Update();

	if (attractTimer_.IsEnd()) {
		player_->ChangeState(std::make_unique<SpearAerialState>());
	}
	else {
		player_->worldtransform_.transform_.translate = Ease::Easing(Ease::EaseName::Lerp,
			startPosition_, player_->weapon_->worldtransform_.GetWorldPosition(),
			attractTimer_.GetNowFrame());
	}
}
