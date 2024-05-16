#include "HoldState.h"
#include "../Weapon.h"
#include "../../Player/Player.h"
#include "../../GameUtility/MathUtility.h"

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

	// オフセット
	offset_ = GlobalVariables::GetInstance()->GetVector3Value("Weapon", "LocalPosition");
	interTarget_ = weapon_->GetTargetPosition() + offset_;
}

void HoldState::Update()
{

	//this->weapon_->throwDirect_
	LerpUpdate();
}

void HoldState::LerpUpdate()
{
	float delay = 0.2f;
	//Vector3 noww = weapon_->worldtransform_.GetWorldPosition();
	Vector3 noww = weapon_->GetTargetPosition();
	if (interTarget_.x != noww.x || interTarget_.y != noww.y) {
		interTarget_ = MathUtility::Lerp(interTarget_, noww, delay);
	}
	if (/*interTarget_.x < noww.x && */weapon_->GetPlayer()->worldtransform_.direction_.x > 0) {
		offset_ = GlobalVariables::GetInstance()->GetVector3Value("Weapon", "LocalPosition");
		offset_.x *= -1.0f;
	}
	else if (/*interTarget_.x > noww.x && */weapon_->GetPlayer()->worldtransform_.direction_.x < 0) {
		offset_ = GlobalVariables::GetInstance()->GetVector3Value("Weapon", "LocalPosition");
	}
	weapon_->worldtransform_.transform_.translate = Vector3::Add(interTarget_, offset_);
}
