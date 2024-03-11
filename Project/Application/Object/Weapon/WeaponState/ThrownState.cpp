#include "ThrownState.h"
#include "../Weapon.h"
#include "../../../Engine/Math/DeltaTime.h"

void ThrownState::Initialize()
{
	// 親子関係解除処理
	weapon_->ReleaseParent();
	// ステート更新
	SetNowState(this);

	// 速さ
	speedValue_ = 10.0f;
	//velocity_ = {};
	velocity_ = weapon_->throwDirect_ * speedValue_;
}

void ThrownState::Update()
{
	// 終了
	if (this->isEnd_) {
		weapon_->ChangeRequest(Weapon::StateName::kImpaled);
		return;
	}

	if (weapon_->GetIsGravity()) {
		velocity_.y -= 9.8f * kDeltaTime_;
	}

	// 移動処理
	weapon_->worldtransform_.transform_.translate += velocity_ * kDeltaTime_;
	weapon_->worldtransform_.direction_ = Vector3::Normalize(velocity_);
}

void ThrownState::ImGuiUpdate()
{
	// 投げられている状態のImGui
	ImGui::Begin("ThrownState");

	ImGui::DragFloat3("Velocity", &velocity_.x);

	ImGui::End();

}
