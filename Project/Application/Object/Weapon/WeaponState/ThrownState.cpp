#include "ThrownState.h"
#include "../Weapon.h"
#include "../../../Engine/Math/DeltaTime.h"

void ThrownState::Initialize()
{
	// 親子関係解除処理
	weapon_->ReleaseParent();

	this->StartEasing(120);
	speedValue_ = 10.0f;
	velocity_ = {};
}

void ThrownState::Update()
{

	if (this->isEnd_) {
		this->weapon_->ChangeState(std::make_unique<ReturnState>());
		return;
	}

	this->EaseUpdate();

	velocity_ = weapon_->throwDirect_ * speedValue_;
	weapon_->worldtransform_.transform_.translate += velocity_ * kDeltaTime_;

}

void ThrownState::ImGuiUpdate()
{
	// 投げられている状態のImGui
	ImGui::Begin("ThrownState");



	ImGui::End();

}
