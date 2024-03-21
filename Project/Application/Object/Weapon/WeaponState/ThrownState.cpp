#include "ThrownState.h"
#include "../Weapon.h"
#include "../../../Engine/Math/DeltaTime.h"
#include "../../../Engine/Math/Ease.h"
#include "../../../Engine/GlobalVariables/GlobalVariables.h"

void ThrownState::Initialize()
{
	// 親子関係解除処理
	weapon_->ReleaseParent();

	// ステート更新
	SetNowState(this);

	// 速さ
	speedValue_ = weapon_->throwSpeedRate_;
	// 投げる際の速度
	//velocity_ = weapon_->throwDirect_ * speedValue_;
	velocity_.x = weapon_->throwDirect_.x * (speedValue_ * 2.0f);
	velocity_.y = weapon_->throwDirect_.y * (speedValue_ * 1.5f);

	// 加速しきるまでの時間
	float maxFrame = 50.0f;
	acceleratorTimer_.Start(maxFrame);
}

void ThrownState::Update()
{
	// 終了
	if (this->isEnd_) {
		weapon_->ChangeRequest(Weapon::StateName::kImpaled);
		return;
	}

	// 武器の重力処理
	if (weapon_->GetIsGravity()) {
		float g = GlobalVariables::GetInstance()->GetFloatValue("Common", "Gravity");
		velocity_.y += (-g) * kDeltaTime_;
		velocity_.x += weapon_->throwDirect_.x * speedValue_ * kDeltaTime_;
	}

	else {
		velocity_.x += weapon_->throwDirect_.x * (speedValue_ * 2.0f) * kDeltaTime_;
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
