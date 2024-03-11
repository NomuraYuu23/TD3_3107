#include "ReturnWaitState.h"
#include "../../Weapon/Weapon.h"

void ReturnWaitState::Initialize()
{

	// ステート更新
	SetNowState(this);

	//endTimer_.Start(150.0f);
}

void ReturnWaitState::Update()
{

	//weapon_->worldtransform_.direction_;

	// タイマー更新
	endTimer_.Update();

}

void ReturnWaitState::ImGuiUpdate()
{



}
