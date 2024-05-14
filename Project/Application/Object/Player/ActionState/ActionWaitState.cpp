#include "ActionWaitState.h"
#include "../Player.h"

void ActionWaitState::Initialize()
{
	// ステートの設定
	player_->SetNowState(this);
	
	float waitFrame = GlobalVariables::GetInstance()->GetFloatValue("SpearJump", "OnSpearWaitFrame");
	changeTimer_.Start(waitFrame);

}

void ActionWaitState::Update()
{
	// 時間切れ
	if (changeTimer_.IsEnd()) {
		player_->spearJumpAccepter_.Start(10.0f);
		player_->ChangeState(std::make_unique<SpearAerialState>());

		return;
	}

	changeTimer_.Update();

}
