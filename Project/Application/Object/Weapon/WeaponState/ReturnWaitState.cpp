#include "ReturnWaitState.h"
#include "../../Weapon/Weapon.h"

void ReturnWaitState::Initialize()
{

	// ステート更新
	SetNowState(this);
	// 
	//weapon_->worldtransform_.usedDirection_ = false;
	//weapon_->worldtransform_.transform_.rotate.x = 1.57f;
	//endTimer_.Start(150.0f);

	rotateVector_ = weapon_->worldtransform_.direction_;

}

void ReturnWaitState::Update()
{

	//weapon_->worldtransform_.direction_;

	//weapon_->worldtransform_.transform_.rotate.z += 1.0f / 60.0f;
	//weapon_->worldtransform_.transform_.rotate.y += 1.0f / 60.0f;

	// タイマー更新
	endTimer_.Update();

}

void ReturnWaitState::ImGuiUpdate()
{



}
