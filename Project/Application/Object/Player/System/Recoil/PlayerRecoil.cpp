#include "PlayerRecoil.h"
#include "../../Player.h"
#include "../../../Engine/Math/Ease.h"
#include "../../../GameUtility/MathUtility.h"

void PlayerRecoil::Initialize(Player* player)
{
	player_ = player;
	recoilFrame_ = 10.0f;
}

void PlayerRecoil::CreateRecoil(const Vector3& direction)
{
	float kPower = 10.0f;
	// 正規化
	recoilValue_ = Vector3::Normalize(direction);
	// 値を
	recoilValue_.x *= kPower * 20.0f;
	recoilValue_.y *= kPower * 25.0f;
	// フレーム数
	timer_.Start(this->recoilFrame_);
}

void PlayerRecoil::Update()
{
	// 早期終了
	if (!timer_.IsActive()) {
		return;
	}


	//velocity_ = Ease::Easing(Ease::EaseName::EaseInOutCubic, startValue_, { 0,0,0 }, timer_.GetNowFrame());
	//player_->worldtransform_.transform_.translate += velocity_ * kDeltaTime_;
	//player_->velocity_ = MathUtility::LerpT(player_->velocity_, {}, 0.5f * kDeltaTime_);
	// 反動の速度ベクトルを計算	
	float lerpRate = 0.2f;
	recoilValue_.x = MathUtility::Lerp(recoilValue_.x, 0, lerpRate);
	recoilValue_.y = MathUtility::Lerp(recoilValue_.y, 0, lerpRate);
	// 速度に計算
	player_->velocity_.x += recoilValue_.x * kDeltaTime_;
	player_->velocity_.y += recoilValue_.y * kDeltaTime_;


	player_->worldtransform_.transform_.translate += player_->velocity_ * kDeltaTime_;
	//player_->worldtransform_.transform_.translate.x += player_->velocity_.x;

	timer_.Update();


}
