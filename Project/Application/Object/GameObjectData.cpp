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
	globalVariables_->AddItem(groupName, "DeadZone", common_.stickDeadZone_);
	globalVariables_->AddItem(groupName, "DeathHeight", common_.deathHeight_);

	groupName = "Player";
	// グループを追加
	globalVariables_->CreateGroup(groupName);
	globalVariables_->AddItem(groupName, "NormalJumpPower", player_.jumpData_.normalJumpPower_);
	globalVariables_->AddItem(groupName, "Gravity", player_.jumpData_.gravity_);
	globalVariables_->AddItem(groupName, "AerialInActiveDecelerateRatio", player_.jumpData_.aerialInActiveDecelerateRatio_);
	globalVariables_->AddItem(groupName, "AerialActiveDecelerateRatio", player_.jumpData_.aerialActiveDecelerateRatio_);
	globalVariables_->AddItem(groupName, "MoveSpeed", player_.moveData_.moveValue_);

	// 反動用
	globalVariables_->AddItem(groupName, "RecoilFrame", player_.recoil.time_);
	globalVariables_->AddItem(groupName, "RecoilRatio", player_.recoil.ratio_);
	globalVariables_->AddItem(groupName, "RecoilLerpRatio", player_.recoil.lerpRatio_);

	globalVariables_->AddItem(groupName, "HitPoint", player_.hpData_.hp_);
	globalVariables_->AddItem(groupName, "HitInvisibleFrame", player_.hpData_.invTimer_);

	// 補正用
	groupName = "AimCorrection";
	globalVariables_->AddItem(groupName, "InitLength", aimCorrect_.InitLength_);
	globalVariables_->AddItem(groupName, "RotateWidth", aimCorrect_.rotationWidth_);
	globalVariables_->AddItem(groupName, "AssistWidth", aimCorrect_.assistWidth_);

	groupName = "Weapon";
	// グループを追加
	globalVariables_->CreateGroup(groupName);
	globalVariables_->AddItem(groupName, "Gravity", weapon_.gravity_);
	globalVariables_->AddItem(groupName, "SpeedRatio", weapon_.speedRatio_);
	globalVariables_->AddItem(groupName, "ScaleRate", weapon_.scaleRate_);
	globalVariables_->AddItem(groupName, "LocalPosition", weapon_.localPosition_);
	globalVariables_->AddItem(groupName, "AngleDot", weapon_.collisionDot_);
	globalVariables_->AddItem(groupName, "KickBackCooltime", weapon_.kickBackCooltime_);
	globalVariables_->AddItem(groupName, "ReturnRate", weapon_.returnLerpRatio_);

	groupName = "SpearJump";
	// グループを追加
	globalVariables_->CreateGroup(groupName);
	globalVariables_->AddItem(groupName, "HorizontalPower", spearJump_.horizontalPower_);
	globalVariables_->AddItem(groupName, "OnSpearWaitFrame", spearJump_.onSpearWaitFrame_);
	globalVariables_->AddItem(groupName, "SpearJumpPower", spearJump_.highJumpPower_);
	globalVariables_->AddItem(groupName, "AerialAcceleration", spearJump_.aerialAcceleration_);
	globalVariables_->AddItem(groupName, "inverceRatio", spearJump_.invAerialRatio_);
	globalVariables_->AddItem(groupName, "Gravity", spearJump_.jumpGravity_);

	groupName = "Camera";
	// グループを追加
	globalVariables_->CreateGroup(groupName);
	globalVariables_->AddItem(groupName, "Offset", camera_.offset_);
	globalVariables_->AddItem(groupName, "PullOffset", camera_.pullMaxOffset_);
	globalVariables_->AddItem(groupName, "MinFov", camera_.minFov_);
	globalVariables_->AddItem(groupName, "MaxFov", camera_.maxFov_);
	globalVariables_->AddItem(groupName, "MinRange", camera_.minRange_);
	globalVariables_->AddItem(groupName, "MaxRange", camera_.maxRange_);

	groupName = "Dash";
	// グループを追加
	globalVariables_->CreateGroup(groupName);
	globalVariables_->AddItem(groupName, "AcceptFrame", dash_.acceptFrame_);
	globalVariables_->AddItem(groupName, "DashPower", dash_.dashPower_);
	globalVariables_->AddItem(groupName, "SlowFrame", dash_.slowFrame_);
	globalVariables_->AddItem(groupName, "SlowRatio", dash_.slowRatio_);

	groupName = "Enemy";
	globalVariables_->CreateGroup(groupName);
	globalVariables_->AddItem(groupName, "EaseEndFrame", enemy_.easeEndFrame);
	globalVariables_->AddItem(groupName, "ChaseSpeed", enemy_.chaseSpeed_);

	ApplyGlobalVariables();


}

void GameObjectData::ApplyGlobalVariables()
{
	// 基本的な部分
	const char* groupName = "Common";
	common_.gravity_ = globalVariables_->GetFloatValue(groupName, "Gravity");
	common_.slowMotionFactor_ = globalVariables_->GetFloatValue(groupName, "SlowFactor");
	common_.stickDeadZone_ = globalVariables_->GetFloatValue(groupName, "DeadZone");
	common_.deathHeight_ = globalVariables_->GetFloatValue(groupName, "DeathHeight");

	// プレイヤー
	groupName = "Player";
	player_.jumpData_.normalJumpPower_ = globalVariables_->GetFloatValue(groupName, "NormalJumpPower");
	player_.jumpData_.gravity_ = globalVariables_->GetFloatValue(groupName, "Gravity");
	player_.jumpData_.aerialInActiveDecelerateRatio_ = globalVariables_->GetFloatValue(groupName, "AerialInActiveDecelerateRatio");
	player_.jumpData_.aerialActiveDecelerateRatio_ = globalVariables_->GetFloatValue(groupName, "AerialActiveDecelerateRatio");
	player_.moveData_.moveValue_ = globalVariables_->GetFloatValue(groupName, "MoveSpeed");

	player_.recoil.time_ = globalVariables_->GetFloatValue(groupName, "RecoilFrame");
	player_.recoil.ratio_ = globalVariables_->GetFloatValue(groupName, "RecoilRatio");
	player_.recoil.lerpRatio_ = globalVariables_->GetFloatValue(groupName, "RecoilLerpRatio");

	player_.hpData_.hp_ = globalVariables_->GetIntValue(groupName, "HitPoint");
	player_.hpData_.invTimer_ = globalVariables_->GetFloatValue(groupName, "HitInvisibleFrame");

	// 補正用
	groupName = "AimCorrection";
	aimCorrect_.InitLength_ = globalVariables_->GetFloatValue(groupName, "InitLength");
	aimCorrect_.rotationWidth_ = globalVariables_->GetFloatValue(groupName, "RotateWidth");
	aimCorrect_.assistWidth_ = globalVariables_->GetFloatValue(groupName, "AssistWidth");

	groupName = "Weapon";
	weapon_.gravity_ = globalVariables_->GetFloatValue(groupName, "Gravity");
	weapon_.speedRatio_ = globalVariables_->GetFloatValue(groupName, "SpeedRatio");
	weapon_.scaleRate_ = globalVariables_->GetFloatValue(groupName, "ScaleRate");
	weapon_.localPosition_ = globalVariables_->GetVector3Value(groupName, "LocalPosition");
	weapon_.collisionDot_ = globalVariables_->GetFloatValue(groupName, "AngleDot");
	weapon_.kickBackCooltime_ = globalVariables_->GetFloatValue(groupName, "KickBackCooltime");
	weapon_.returnLerpRatio_ = globalVariables_->GetFloatValue(groupName, "ReturnRate");

	groupName = "SpearJump";
	spearJump_.horizontalPower_ = globalVariables_->GetFloatValue(groupName, "HorizontalPower");
	spearJump_.onSpearWaitFrame_ = globalVariables_->GetFloatValue(groupName, "OnSpearWaitFrame");
	spearJump_.highJumpPower_ = globalVariables_->GetFloatValue(groupName, "SpearJumpPower");
	spearJump_.aerialAcceleration_ = globalVariables_->GetFloatValue(groupName, "AerialAcceleration");
	spearJump_.invAerialRatio_ = globalVariables_->GetFloatValue(groupName, "inverceRatio");
	spearJump_.jumpGravity_ = globalVariables_->GetFloatValue(groupName, "Gravity");

	groupName = "Camera";
	// グループを追加
	camera_.offset_ = globalVariables_->GetVector3Value(groupName, "Offset");
	camera_.pullMaxOffset_ = globalVariables_->GetFloatValue(groupName, "PullOffset");
	camera_.minFov_ = globalVariables_->GetFloatValue(groupName, "MinFov");
	camera_.maxFov_ = globalVariables_->GetFloatValue(groupName, "MaxFov");
	camera_.minRange_ = globalVariables_->GetFloatValue(groupName, "MinRange");
	camera_.maxRange_ = globalVariables_->GetFloatValue(groupName, "MaxRange");

	groupName = "Dash";
	// グループを追加
	dash_.acceptFrame_ = globalVariables_->GetFloatValue(groupName, "AcceptFrame");
	dash_.dashPower_ = globalVariables_->GetFloatValue(groupName, "DashPower");
	dash_.slowFrame_ = globalVariables_->GetFloatValue(groupName, "SlowFrame");
	dash_.slowRatio_ = globalVariables_->GetFloatValue(groupName, "SlowRatio");

	groupName = "Enemy";
	enemy_.easeEndFrame = globalVariables_->GetFloatValue(groupName, "EaseEndFrame");
	enemy_.chaseSpeed_ = globalVariables_->GetFloatValue(groupName, "ChaseSpeed");
}
