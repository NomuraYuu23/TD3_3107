#include "ImpaledState.h"
#include "../Weapon.h"

void ImpaledState::Initialize()
{
	// 終了までのカウント開始
	//StartEasing(120);
	SetNowState(this);
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
