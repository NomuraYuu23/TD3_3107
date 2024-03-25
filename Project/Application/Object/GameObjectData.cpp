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
	globalVariables_->AddItem(groupName, "NormalJumpPower", player_.normalJumpPower_);
	globalVariables_->AddItem(groupName, "SpearJumpPower", player_.highJumpPower_);
	globalVariables_->AddItem(groupName, "Gravity", player_.gravity_);
	globalVariables_->AddItem(groupName, "MoveSpeed", player_.moveValue_);
	globalVariables_->AddItem(groupName, "AerialAcceleration", player_.aerialAcceleration_);

	groupName = "Weapon";
	// グループを追加
	globalVariables_->CreateGroup(groupName);
	globalVariables_->AddItem(groupName, "Gravity", weapon_.gravity_);
	globalVariables_->AddItem(groupName, "SpeedRatio", weapon_.speedRatio_);
	globalVariables_->AddItem(groupName, "ScaleRate", weapon_.scaleRate_);

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
	player_.normalJumpPower_ = globalVariables_->GetFloatValue(groupName, "NormalJumpPower");
	player_.highJumpPower_ = globalVariables_->GetFloatValue(groupName, "SpearJumpPower");
	player_.gravity_ = globalVariables_->GetFloatValue(groupName, "Gravity");
	player_.moveValue_ = globalVariables_->GetFloatValue(groupName, "MoveSpeed");
	player_.aerialAcceleration_ = globalVariables_->GetFloatValue(groupName, "AerialAcceleration");

	groupName = "Weapon";
	weapon_.gravity_ = globalVariables_->GetFloatValue(groupName, "Gravity");
	weapon_.speedRatio_ = globalVariables_->GetFloatValue(groupName, "SpeedRatio");
	weapon_.scaleRate_ = globalVariables_->GetFloatValue(groupName, "ScaleRate");

}
