#include "PlayerRecoil.h"
#include "../../Player.h"
#include "../../../Engine/Math/Ease.h"
#include "../../../GameUtility/MathUtility.h"

void PlayerRecoil::Initialize(Player* player)
{
	player_ = player;
	recoilFrame_ = 15.0f;
}

void PlayerRecoil::CreateRecoil(const Vector3& direction)
{
	//float kPower = 30.0f;
	// 正規化
	recoilValue_ = Vector3::Normalize(direction);
	//// 値を
	//recoilValue_.x *= kPower;
	//recoilValue_.y *= kPower;
	player_->velocity_ = recoilValue_ * 20.0f;
	recoilValue_ *= 20.0f;
	// フレーム数
	timer_.Start(this->recoilFrame_);
}

void PlayerRecoil::Update()
{
	// 早期終了
	if (!timer_.IsActive()) {
		return;
	}
	// タイムスケールに合わせる処理
	slowValue_++;
	float timeScale = slowValue_ / IObject::sPlaySpeed;
	if (timeScale == 1.0f) {
		slowValue_ = 0;
	}
	else {
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

	//float speed = 15.0f;
	//velocity_ += (recoilValue_ * speed) * kDeltaTime_;

	//player_->worldtransform_.transform_.translate += velocity_ * kDeltaTime_;
	player_->worldtransform_.transform_.translate += player_->velocity_ * kDeltaTime_;
	//player_->worldtransform_.transform_.translate.x += player_->velocity_.x;

	timer_.Update();

	if (timer_.IsEnd()) {
		//player_->velocity_;
	}

}

void PlayerRecoil::CancelRecoil()
{
	// タイマーの終了
	timer_.End();

}
