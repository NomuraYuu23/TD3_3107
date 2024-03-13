#include "ReturnWaitState.h"
#include "../../GameUtility/MathUtility.h"
#include "../../Weapon/Weapon.h"
#include "../../../Engine/Input/Input.h"

void ReturnWaitState::Initialize()
{

	// ステート更新
	SetNowState(this);
	// 
	//weapon_->worldtransform_.usedDirection_ = false;
	//weapon_->worldtransform_.transform_.rotate.x = 1.57f;
	endTimer_.Start(90.0f);
	pressTimer_.Start(15.0f);
	rotateVector_ = weapon_->worldtransform_.direction_;

	// 移動座標
	float moveValue = 2.5f;
	// 待機座標の生成
	highestPoint_.x = weapon_->worldtransform_.GetWorldPosition().x + weapon_->invDirect_.x * moveValue;
	highestPoint_.y = weapon_->worldtransform_.GetWorldPosition().y + weapon_->invDirect_.y * moveValue;

}

void ReturnWaitState::Update()
{
	// 上下のスティック入力
	Vector2 rightStick = Input::GetInstance()->GetRightAnalogstick();

	if (rightStick.y < 0) {
		weapon_->returnDirect_.y = 1.0f;
	}
	else if (rightStick.y > 0) {
		weapon_->returnDirect_.y = -1.0f;
	}
	else {
		weapon_->returnDirect_.y = 0.0f;
	}

	//weapon_->worldtransform_.direction_.x = std::cosf(1.0f / 360.0f);
	//weapon_->worldtransform_.direction_.y = std::sinf(1.0f / 360.0f);

	weapon_->worldtransform_.transform_.translate.x = MathUtility::Lerp(weapon_->worldtransform_.GetWorldPosition().x, highestPoint_.x, 0.05f);
	weapon_->worldtransform_.transform_.translate.y = MathUtility::Lerp(weapon_->worldtransform_.GetWorldPosition().y, highestPoint_.y, 0.05f);

	// 終了タイミング
	if (endTimer_.IsEnd() || pressTimer_.IsEnd()) {
		weapon_->ChangeRequest(Weapon::StateName::kReturn);
	}
	// 長押し
	if (Input::GetInstance()->PushJoystick(kJoystickButtonRB)) {
		pressTimer_.Update();
	}

	// タイマー更新
	endTimer_.Update();

}

void ReturnWaitState::ImGuiUpdate()
{



}
