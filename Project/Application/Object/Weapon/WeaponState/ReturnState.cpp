#include "ReturnState.h"
#include "../Weapon.h"
#include "../../../Engine/Input/Input.h"
#include "../../../Engine/Math/Ease.h"

void ReturnState::Initialize()
{
	// 親子関係解除
	weapon_->ReleaseParent();

	StartEasing(60);
	startPosition_ = weapon_->worldtransform_.transform_.translate;

}

void ReturnState::Update()
{
	// 終了処理
	if (isEnd_) {
		weapon_->ChangeRequest(Weapon::StateName::kHold);
		//weapon_->ChangeState(std::make_unique<HoldState>());
		return;
	}

	// イージング更新
	EaseUpdate();

}

void ReturnState::ImGuiUpdate()
{
	ImGui::Begin("ReturnState");

	ImGui::DragFloat("easeT", &ease_t_);
	ImGui::DragFloat("EndFrame", &easingEndFrame_);
	ImGui::Text("%d : end", isEnd_);

	ImGui::End();

}

void ReturnState::StartEasing(int frame)
{
	// 開始関数
	IWeaponState::StartEasing(frame);

}

void ReturnState::EaseUpdate()
{
	// イージングの値更新
	IWeaponState::EaseUpdate();
	// 線形補間による座標更新
	weapon_->worldtransform_.transform_.translate = Ease::Easing(Ease::EaseName::EaseOutSine, startPosition_, weapon_->GetTargetPosition(), ease_t_);
}
