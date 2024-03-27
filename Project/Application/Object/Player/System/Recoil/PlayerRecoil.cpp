#include "PlayerRecoil.h"
#include "../../Player.h"
#include "../../../Engine/Math/Ease.h"
#include "../../../GameUtility/MathUtility.h"
#include "../../../Engine/GlobalVariables/GlobalVariables.h"

void PlayerRecoil::Initialize(Player* player)
{
	player_ = player;
	recoilFrame_ = GlobalVariables::GetInstance()->GetFloatValue("Player", "RecoilFrame");
	recoilRatio_ = GlobalVariables::GetInstance()->GetFloatValue("Player", "RecoilRatio");
	lerpRatio_ = GlobalVariables::GetInstance()->GetFloatValue("Player", "RecoilLerpRatio");
}

void PlayerRecoil::CreateRecoil(const Vector3& direction)
{
	// デバック時のみ
#ifdef _DEBUG
	recoilFrame_ = GlobalVariables::GetInstance()->GetFloatValue("Player", "RecoilFrame");
	recoilRatio_ = GlobalVariables::GetInstance()->GetFloatValue("Player", "RecoilRatio");
	lerpRatio_ = GlobalVariables::GetInstance()->GetFloatValue("Player", "RecoilLerpRatio");
#endif // _DEBUG

	isAccept_ = false;
	// 反動の値設定・速度設定
	player_->velocity_ = Vector3::Normalize(direction) * recoilRatio_;

	// 開始
	timer_.Start(recoilFrame_);
}

void PlayerRecoil::Update()
{
	// 早期終了
	if (!timer_.IsActive()) {
		return;
	}

	// 反動の速度ベクトルを計算	
	player_->velocity_.x = MathUtility::Lerp(player_->velocity_.x, 0, lerpRatio_);
	player_->velocity_.y = MathUtility::Lerp(player_->velocity_.y, 0, lerpRatio_);

	// 座標計算
	player_->worldtransform_.transform_.translate += player_->velocity_ * kDeltaTime_;

	timer_.Update();

}

void PlayerRecoil::CancelRecoil()
{
	// タイマーの終了
	timer_.End();

}
