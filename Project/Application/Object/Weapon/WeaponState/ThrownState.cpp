#include "ThrownState.h"
#include "../Weapon.h"
#include "../../../Engine/Math/DeltaTime.h"

void ThrownState::Initialize()
{
	// 親子関係解除処理
	weapon_->ReleaseParent();
	// ステート更新
	SetNowState(this);
	// 開始設定
	this->StartEasing(90);
	// 速さ
	speedValue_ = 10.0f;
	velocity_ = {};
}

void ThrownState::Update()
{
	// 終了
	if (this->isEnd_) {
		weapon_->ChangeRequest(Weapon::StateName::kImpaled);
		return;
	}
	// 更新
	this->EaseUpdate();

	// 移動処理
	velocity_ = weapon_->throwDirect_ * speedValue_;
	weapon_->worldtransform_.transform_.translate += velocity_ * kDeltaTime_;

}

void ThrownState::ImGuiUpdate()
{
	// 投げられている状態のImGui
	ImGui::Begin("ThrownState");



	ImGui::End();

}
