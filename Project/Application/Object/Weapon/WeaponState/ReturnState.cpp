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
	// 角度から方向
	weapon_->worldtransform_.usedDirection_ = true;

	//StartEasing(30);
	startPosition_ = weapon_->worldtransform_.transform_.translate;
	lerp_t = 0.05f;
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

	ImGui::DragFloat("lerpRatio", &lerp_t, 0.01f, 0, 1);

	ImGui::End();

}

void ReturnState::EaseUpdate()
{
	// 線形補間による座標更新
	weapon_->worldtransform_.transform_.translate.x = MathUtility::Lerp(weapon_->worldtransform_.transform_.translate.x, weapon_->GetTargetPosition().x, lerp_t);
	weapon_->worldtransform_.transform_.translate.y = MathUtility::Lerp(weapon_->worldtransform_.transform_.translate.y, weapon_->GetTargetPosition().y, lerp_t);

	weapon_->worldtransform_.direction_ = weapon_->worldtransform_.transform_.translate - weapon_->GetTargetPosition();

}
