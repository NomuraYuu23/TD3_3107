#include "GameObjectData.h"
#include "../../Engine/2D/ImguiManager.h"

void GameObjectData::Initialize()
{
	globalVariables_ = GlobalVariables::GetInstance();

	const char* groupName = "Player";
	// グループを追加
	globalVariables_->CreateGroup(groupName);
	globalVariables_->AddItem(groupName, "NormalJumpPower", player_.normalJumpPower_);
	globalVariables_->AddItem(groupName, "SpearJumpPower", player_.highJumpPower_);
	globalVariables_->AddItem(groupName, "Gravity", player_.gravity_);

	groupName = "Common";
	// グループを追加
	globalVariables_->CreateGroup(groupName);
	globalVariables_->AddItem(groupName, "Gravity", common_.gravity_);

	ApplyGlobalVariables(); 


}

void GameObjectData::ApplyGlobalVariables()
{
	// 基本的な部分
	const char* groupName = "Common";
	common_.gravity_ = globalVariables_->GetFloatValue(groupName, "Gravity");

	// プレイヤー
	groupName = "Player";
	player_.normalJumpPower_ = globalVariables_->GetFloatValue(groupName, "NormalJumpPower");
	player_.highJumpPower_ = globalVariables_->GetFloatValue(groupName, "SpearJumpPower");
	player_.gravity_ = globalVariables_->GetFloatValue(groupName, "Gravity");

}
