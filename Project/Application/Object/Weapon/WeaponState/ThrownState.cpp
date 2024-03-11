#include "ThrownState.h"
#include "../Weapon.h"
#include "../../../Engine/Math/DeltaTime.h"
#include "../../../Engine/Math/Ease.h"

void ThrownState::Initialize()
{
	// 親子関係解除処理
	weapon_->ReleaseParent();
	// ステート更新
	SetNowState(this);

	// 速さ
	speedValue_ = 10.0f;
	// 投げる際の速度
	velocity_ = weapon_->throwDirect_ * speedValue_;

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


	//if (acceleratorTimer_.IsActive()) {
	//	float max_v = 25.0f;
	//	float min_v = 8.0f;
	//	speedValue_ = Ease::Easing(Ease::EaseName::EaseOutQuad, min_v, max_v, acceleratorTimer_.GetNowFrame());
	//}

	//acceleratorTimer_.Update();

	velocity_.x += weapon_->throwDirect_.x * speedValue_ * kDeltaTime_;

	// 武器の重力処理
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
