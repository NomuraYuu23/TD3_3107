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
	weapon_->worldtransform_.transform_.translate.z = 0.0f;
	// 戻ってくる方向用のベクトル初期化
	weapon_->returnDirect_ = {};

	// 槍の保持フラグをtrueに
	weapon_->isHold_ = true;
}

void HoldState::Update()
{

	//this->weapon_->throwDirect_

}
