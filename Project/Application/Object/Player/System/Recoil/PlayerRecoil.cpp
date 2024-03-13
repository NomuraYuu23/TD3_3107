#include "PlayerRecoil.h"
#include "../../Player.h"
#include "../../../Engine/Math/Ease.h"

void PlayerRecoil::Initialize(Player* player)
{
	player_ = player;
	recoilFrame_ = 10.0f;
}

void PlayerRecoil::CreateRecoil(const Vector3& direction)
{
	float kPower = 80.0f;
	this->startValue_ = direction;
	startValue_ *= kPower;
	// フレーム数
	timer_.Start(this->recoilFrame_);
}

void PlayerRecoil::Update()
{
	// 早期終了
	if (!timer_.IsActive()) {
		return;
	}


	velocity_ = Ease::Easing(Ease::EaseName::EaseInOutCubic, startValue_, { 0,0,0 }, timer_.GetNowFrame());
	player_->worldtransform_.transform_.translate += velocity_ * kDeltaTime_;

	timer_.Update();


}
