#include "PlayerAnimManager.h"
#include "../Player.h"

void PlayerAnimManager::Init(Player* player)
{
	// プレイヤー取得
	player_ = player;

	// アニメーション取得と初期化
	anim_.Initialize(
		player_->model_->GetNodeAnimationData(),
		player_->localMatrixManager_->GetInitTransform(),
		player_->localMatrixManager_->GetNodeNames());

	// アニメーション開始
	anim_.StartAnimation(0, true);

	// 角度の初期設定
	player_->worldtransform_.transform_.rotate.y = (static_cast<float>(std::numbers::pi) / 2.0f) * -1.0f;
}

void PlayerAnimManager::Update()
{
	// アニメーションの更新
	player_->localMatrixManager_->SetNodeLocalMatrix(anim_.AnimationUpdate());
	player_->localMatrixManager_->Map();
}
