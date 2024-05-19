#include "ThrownState.h"
#include "../Weapon.h"
#include "../../../Engine/Math/DeltaTime.h"
#include "../../../Engine/Math/Ease.h"
#include "../../../Engine/GlobalVariables/GlobalVariables.h"
#include "../../Player/Player.h"

void ThrownState::Initialize()
{
	// 親子関係解除処理
	weapon_->ReleaseParent();

	// 槍の保持フラグをfalseに
	weapon_->isHold_ = false;

	// ステート更新
	SetNowState(this);
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	// 速さ
	speedValue_ = globalVariables->GetFloatValue("Weapon", "SpeedRatio");
	// 投げる際の速度
	if (weapon_->IsGravity()) {
		velocity_.x = weapon_->throwDirect_.x * (speedValue_);
		velocity_.y = weapon_->throwDirect_.y * (speedValue_);
	}
	else {
		velocity_.x = weapon_->throwDirect_.x * (speedValue_ * 2.0f);
		velocity_.y = weapon_->throwDirect_.y * (speedValue_ * 2.0f);
	}
	weapon_->worldtransform_.direction_ = weapon_->throwDirect_;
	weapon_->throwInvTimer_.Start(1.0f);
	//weapon_->safeLaunchTimer_.Start(2.0f);

	// 槍投げアニメーション開始
	//weapon_->GetAnimManager()->PlayAnimation(SpearAnimManager::SpearThrow);

	// 投げ効果音を再生
	//weapon_->GetPlayer()->gameAudioManager_->PlayWave(GameAudioNameIndex::kThrowSpear);
}

void ThrownState::Update()
{
	// 終了
	if (this->isEnd_) {
		weapon_->ChangeRequest(Weapon::StateName::kImpaled);
		return;
	}

	//weapon_->safeLaunchTimer_.Update();

	// 武器の重力処理
	if (weapon_->IsGravity()) {
		float g = GlobalVariables::GetInstance()->GetFloatValue("Weapon", "Gravity");
		velocity_.y += (kGravity * g) * kDeltaTime_;
	}

	else {

	}

	// 移動処理
	weapon_->worldtransform_.transform_.translate += (velocity_ * kDeltaTime_) * (1.0f / IObject::sPlaySpeed);
	weapon_->worldtransform_.direction_ = Vector3::Normalize(velocity_);
}

void ThrownState::ImGuiUpdate()
{
	// 投げられている状態のImGui
	ImGui::Begin("ThrownState");

	ImGui::DragFloat3("Velocity", &velocity_.x);

	ImGui::End();

}
