#include "ImpaledState.h"
#include "../Weapon.h"

void ImpaledState::Initialize()
{
	// 終了までのカウント開始
	//StartEasing(120);
	SetNowState(this);
	// 親子関係を解消し、ワールド座標を適応
	weapon_->worldtransform_.transform_.translate = weapon_->worldtransform_.GetWorldPosition();
	weapon_->worldtransform_.parent_ = nullptr;
	weapon_->worldtransform_.UpdateMatrix();

	// 待機アニメーション開始
	weapon_->GetAnimManager()->PlayAnimation(SpearAnimManager::SpearIdle, true);

}

void ImpaledState::Update()
{
	// 変更処理
	if (isEnd_) {
		weapon_->ChangeRequest(Weapon::StateName::kReturn);
		return;
	}

}

void ImpaledState::ImGuiUpdate()
{

	ImGui::Begin("Impaled");

	if (ImGui::Button("ReturnButton")) {
		isEnd_ = true;
	}

	ImGui::Separator();

	ImGui::DragInt("easeTime", &easeTime_, 1, 1, 600);

	ImGui::End();

}
