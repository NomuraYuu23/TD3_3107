#include "AttractState.h"
#include "../Player.h"
#include "../../../Engine/Math/Ease.h"

void AttractState::Initialize()
{
	// ステート設定
	player_->SetNowState(this);
	// 初期座標
	startPosition_ = player_->worldtransform_.GetWorldPosition();

	// 時間を距離で計算するように変更しろ
	int lerpRatio = (int)Vector3::Length(player_->weapon_->worldtransform_.GetWorldPosition() - startPosition_);
	attractTimer_.Start((float)lerpRatio);

	// 移動方向ベクトル
	if (player_->weapon_->worldtransform_.GetWorldPosition().x > startPosition_.x) {
		jumpDirection_.x = -1.0f;
	}
	else {
		jumpDirection_.x = 1.0f;
	}


}

void AttractState::Update()
{
	// 更新
	attractTimer_.Update();

	// 引き寄せ終了分岐
	if (attractTimer_.IsEnd()) {
		float ratio = 15.0f;
		player_->velocity_.x = jumpDirection_.x * (ratio);
		player_->ChangeState(std::make_unique<SpearAerialState>());
	}
	// 継続
	else {
		player_->worldtransform_.transform_.translate = Ease::Easing(Ease::EaseName::EaseOutQuad,
			startPosition_, player_->weapon_->worldtransform_.GetWorldPosition(),
			attractTimer_.GetNowFrame());
	}
}
