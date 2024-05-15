#include "ShockEffectSystem.h"

void ShockEffectSystem::Initialize(Weapon* weapon)
{
	weapon_ = weapon;
	// 衝撃波マネージャの初期化
	shockWaveManager_ = std::make_unique<ShockWaveManager>();
	shockWaveManager_->Initialize();
}

void ShockEffectSystem::Update()
{
	shockWaveManager_->Update();
}

void ShockEffectSystem::ImGuiDraw()
{
	ImGui::DragFloat2("ScreenPos", &screenPosition_.x);
	ImGui::DragFloat2("ScreenRatio", &screenPositionRatio_.x);
}
