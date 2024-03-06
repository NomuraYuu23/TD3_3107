#include "ThrownState.h"
#include "../Weapon.h"

void ThrownState::Initialize()
{
	// 親子関係解除処理
	weapon_->ReleaseParent();

}

void ThrownState::Update()
{

}

void ThrownState::ImGuiUpdate()
{
	// 投げられている状態のImGui
	ImGui::Begin("ThrownState");



	ImGui::End();

}
