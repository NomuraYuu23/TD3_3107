#include "ReturnState.h"
#include "../Weapon.h"
#include "../../../Engine/Input/Input.h"
#include "../../../Engine/Math/Ease.h"
#include "../../../Engine/Math/Math.h"
#include "../../GameUtility/MathUtility.h"

void ReturnState::Initialize()
{
	SetNowState(this);

	// 親子関係解除
	weapon_->ReleaseParent();

	//StartEasing(30);
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
	weapon_->worldtransform_.transform_.translate.x = MathUtility::Lerp(weapon_->worldtransform_.transform_.translate.x, weapon_->GetTargetPosition().x, 0.01f);
	weapon_->worldtransform_.transform_.translate.y = MathUtility::Lerp(weapon_->worldtransform_.transform_.translate.y, weapon_->GetTargetPosition().y, 0.01f);
}
