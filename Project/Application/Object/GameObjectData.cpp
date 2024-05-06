#include "GameObjectData.h"
#include "../../Engine/2D/ImguiManager.h"

void GameObjectData::Initialize()
{
	globalVariables_ = GlobalVariables::GetInstance();
	const char* groupName = "Common";
	// グループを追加
	globalVariables_->CreateGroup(groupName);
	globalVariables_->AddItem(groupName, "Gravity", common_.gravity_);
	globalVariables_->AddItem(groupName, "SlowFactor", common_.slowMotionFactor_);

	groupName = "Player";
	// グループを追加
	globalVariables_->CreateGroup(groupName);
	globalVariables_->AddItem(groupName, "NormalJumpPower", player_.jumpData_.normalJumpPower_);
	globalVariables_->AddItem(groupName, "SpearJumpPower", player_.jumpData_.highJumpPower_);
	globalVariables_->AddItem(groupName, "Gravity", player_.jumpData_.gravity_);
	globalVariables_->AddItem(groupName, "HorizontalPower", player_.jumpData_.horizontalPower_);


	globalVariables_->AddItem(groupName, "MoveSpeed", player_.moveData_.moveValue_);
	globalVariables_->AddItem(groupName, "AerialAcceleration", player_.moveData_.aerialAcceleration_);
	globalVariables_->AddItem(groupName, "inverceRatio", player_.moveData_.invAerialRatio_);

	// 反動用
	globalVariables_->AddItem(groupName, "RecoilFrame", player_.recoil.time_);
	globalVariables_->AddItem(groupName, "RecoilRatio", player_.recoil.ratio_);
	globalVariables_->AddItem(groupName, "RecoilLerpRatio", player_.recoil.lerpRatio_);

	globalVariables_->AddItem(groupName, "HitPoint", player_.hpData_.hp_);
	globalVariables_->AddItem(groupName, "HitInvisibleFrame", player_.hpData_.invTimer_);

	// 補正用
	globalVariables_->AddItem(groupName, "InitLength", player_.correctData_.InitLength_);
	globalVariables_->AddItem(groupName, "RotateWidth", player_.correctData_.rotationWidth_);

	groupName = "Weapon";
	// グループを追加
	globalVariables_->CreateGroup(groupName);
	globalVariables_->AddItem(groupName, "Gravity", weapon_.gravity_);
	globalVariables_->AddItem(groupName, "SpeedRatio", weapon_.speedRatio_);
	globalVariables_->AddItem(groupName, "ScaleRate", weapon_.scaleRate_);
	globalVariables_->AddItem(groupName, "LocalPosition", weapon_.localPosition_);
	globalVariables_->AddItem(groupName, "AngleDot", weapon_.collisionDot_);
	globalVariables_->AddItem(groupName, "AssistWidth", weapon_.assistWidth_);

	ApplyGlobalVariables(); 


}

void GameObjectData::ApplyGlobalVariables()
{
	// 基本的な部分
	const char* groupName = "Common";
	common_.gravity_ = globalVariables_->GetFloatValue(groupName, "Gravity");
	common_.slowMotionFactor_ = globalVariables_->GetFloatValue(groupName, "SlowFactor");

	// プレイヤー
	groupName = "Player";
	player_.jumpData_.normalJumpPower_ = globalVariables_->GetFloatValue(groupName, "NormalJumpPower");
	player_.jumpData_.highJumpPower_ = globalVariables_->GetFloatValue(groupName, "SpearJumpPower");
	player_.jumpData_.gravity_ = globalVariables_->GetFloatValue(groupName, "Gravity");
	player_.jumpData_.horizontalPower_ = globalVariables_->GetFloatValue(groupName, "HorizontalPower");

	player_.moveData_.moveValue_ = globalVariables_->GetFloatValue(groupName, "MoveSpeed");
	player_.moveData_.aerialAcceleration_ = globalVariables_->GetFloatValue(groupName, "AerialAcceleration");
	player_.moveData_.invAerialRatio_ = globalVariables_->GetFloatValue(groupName, "inverceRatio");

	player_.recoil.time_ = globalVariables_->GetFloatValue(groupName, "RecoilFrame");
	player_.recoil.ratio_ = globalVariables_->GetFloatValue(groupName, "RecoilRatio");
	player_.recoil.lerpRatio_ = globalVariables_->GetFloatValue(groupName, "RecoilLerpRatio");

	player_.hpData_.hp_ = globalVariables_->GetIntValue(groupName, "HitPoint");
	player_.hpData_.invTimer_ = globalVariables_->GetFloatValue(groupName, "HitInvisibleFrame");

	// 補正用
	player_.correctData_.InitLength_ = globalVariables_->GetFloatValue(groupName, "InitLength");
	player_.correctData_.rotationWidth_ = globalVariables_->GetFloatValue(groupName, "RotateWidth");

	groupName = "Weapon";
	weapon_.gravity_ = globalVariables_->GetFloatValue(groupName, "Gravity");
	weapon_.speedRatio_ = globalVariables_->GetFloatValue(groupName, "SpeedRatio");
	weapon_.scaleRate_ = globalVariables_->GetFloatValue(groupName, "ScaleRate");
	weapon_.localPosition_ = globalVariables_->GetVector3Value(groupName, "LocalPosition");
	weapon_.collisionDot_ = globalVariables_->GetFloatValue(groupName, "AngleDot"); 
	weapon_.assistWidth_ = globalVariables_->GetFloatValue(groupName, "AssistWidth");

}
