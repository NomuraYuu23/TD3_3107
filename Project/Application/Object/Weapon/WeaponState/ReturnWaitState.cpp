#include "ReturnWaitState.h"
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

}

void ReturnWaitState::Update()
{

	//weapon_->worldtransform_.direction_;

	//weapon_->worldtransform_.transform_.rotate.z += 1.0f / 60.0f;
	//weapon_->worldtransform_.transform_.rotate.y += 1.0f / 60.0f;
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
	weapon_->worldtransform_.direction_.x += 1.0f / 30.0f;
	weapon_->worldtransform_.direction_.y += 1.0f / 30.0f;
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
