#include "ImpaledState.h"
#include "../Weapon.h"

void ImpaledState::Initialize()
{
	// 終了までのカウント開始
	StartEasing(120);

}

void ImpaledState::Update()
{
	// 変更処理
	if (isEnd_) {
		weapon_->ChangeRequest(Weapon::StateName::kReturn);
		return;
	}

	// タイマー処理
	EaseUpdate();

}

void ImpaledState::ImGuiUpdate()
{

	ImGui::Begin("Impaled");

	ImGui::End();

}
