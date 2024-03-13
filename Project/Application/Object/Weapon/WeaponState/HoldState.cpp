#include "HoldState.h"
#include "../Weapon.h"

void HoldState::Initialize()
{
	// 親子付け
	weapon_->SettingParent();
	SetNowState(this);
	// 投げる向きの初期化
	this->weapon_->throwDirect_ = { 0,1,0 };
	// 向きの初期化
	weapon_->worldtransform_.direction_ = Vector3::Normalize(weapon_->throwDirect_);
	// 戻ってくる方向用のベクトル初期化
	weapon_->returnDirect_ = {};
}

void HoldState::Update()
{

	//this->weapon_->throwDirect_

}
