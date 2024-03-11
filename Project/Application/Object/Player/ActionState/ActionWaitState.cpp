#include "ActionWaitState.h"
#include "../Player.h"

void ActionWaitState::Initialize()
{
	// ステートの設定
	player_->SetNowState(this);
	
	changeTimer_.Start(30.0f);

}

void ActionWaitState::Update()
{
	// 時間切れ
	if (changeTimer_.IsEnd()) {
		player_->ChangeState(std::make_unique<GroundState>());
		return;
	}

	changeTimer_.Update();

}
