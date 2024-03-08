#include "HoldState.h"
#include "../Weapon.h"

void HoldState::Initialize()
{
	// 親子付け
	weapon_->SettingParent();
	SetNowState(this);
	this->weapon_->throwDirect_ = { 0,1,0 };
}

void HoldState::Update()
{

	//this->weapon_->throwDirect_

}
