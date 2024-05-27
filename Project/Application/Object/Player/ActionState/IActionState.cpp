#include "IActionState.h"
#include "../Player.h"

void IActionState::PreInitialize(Player* player)
{
	// プレイヤー
	player_ = player;
}

void IActionState::Update()
{
	// 座標更新
	player_->worldtransform_.transform_.translate += player_->velocity_ * kDeltaTime_ * (1.0f / GameSystemManager::sGameSpeed);
}
